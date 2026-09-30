/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_Widget.h"
#include "MyGUI_GeometryUtility.h"
#include <array>
#include <algorithm>
#include <cmath>

namespace MyGUI
{
	namespace
	{

		float edgeDistance(const FloatPoint& _a, const FloatPoint& _b, const FloatPoint& _point)
		{
			return (_b.left - _a.left) * (_point.top - _a.top) - (_b.top - _a.top) * (_point.left - _a.left);
		}

		Vertex interpolate(const Vertex& _a, const Vertex& _b, float _t)
		{
			Vertex result = _a;
			result.x += (_b.x - _a.x) * _t;
			result.y += (_b.y - _a.y) * _t;
			result.z += (_b.z - _a.z) * _t;
			result.u += (_b.u - _a.u) * _t;
			result.v += (_b.v - _a.v) * _t;
			uint32 colour = 0;
			for (unsigned shift = 0; shift < 32; shift += 8)
			{
				const float a = float((_a.colour >> shift) & 0xFF);
				const float b = float((_b.colour >> shift) & 0xFF);
				colour |= uint32(std::lround(a + (b - a) * _t)) << shift;
			}
			result.colour = colour;
			return result;
		}

		std::array<FloatPoint, 4> clipQuad(const Widget* _widget, const RenderTargetInfo& _info)
		{
			const float width = (float)_widget->getWidth();
			const float height = (float)_widget->getHeight();
			std::array<FloatPoint, 4> corners = {{{0, 0}, {width, 0}, {width, height}, {0, height}}};
			_widget->localToLayer(corners.data(), corners.size());
			geometry_utility::toRenderTargetInPlace(corners, _info);
			return corners;
		}

		void appendClippedTriangles(
			std::vector<Vertex>& _output,
			const Vertex* _input,
			size_t _count,
			const Widget* _owner,
			const RenderTargetInfo& _info)
		{
			std::vector<std::array<FloatPoint, 4>> quads;
			for (const ICroppedRectangle* crop = _owner; crop != nullptr; crop = crop->getCroppedParent())
				quads.push_back(clipQuad(static_cast<const Widget*>(crop), _info));
			std::vector<Vertex> polygon;
			std::vector<Vertex> clipped;
			for (size_t base = 0; base + 2 < _count; base += 3)
			{
				polygon.assign(_input + base, _input + base + 3);
				for (const auto& quad : quads)
				{
					const float sign = edgeDistance(quad[0], quad[1], quad[2]) >= 0.0f ? 1.0f : -1.0f;
					for (size_t side = 0; side < quad.size() && !polygon.empty(); ++side)
					{
						clipped.clear();
						const FloatPoint& a = quad[side];
						const FloatPoint& b = quad[(side + 1) % quad.size()];
						for (size_t i = 0; i < polygon.size(); ++i)
						{
							const Vertex& current = polygon[i];
							const Vertex& next = polygon[(i + 1) % polygon.size()];
							const float first = sign * edgeDistance(a, b, {current.x, current.y});
							const float second = sign * edgeDistance(a, b, {next.x, next.y});
							if (first >= 0.0f)
								clipped.push_back(current);
							if ((first < 0.0f && second > 0.0f) || (first > 0.0f && second < 0.0f))
								clipped.push_back(interpolate(current, next, first / (first - second)));
						}
						polygon.swap(clipped);
					}
				}
				for (size_t i = 1; i + 1 < polygon.size(); ++i)
				{
					_output.push_back(polygon[0]);
					_output.push_back(polygon[i]);
					_output.push_back(polygon[i + 1]);
				}
			}
		}

	}

	RenderItem::RenderItem()
	{
		mVertexBuffer = RenderManager::getInstance().createVertexBuffer();
	}

	RenderItem::~RenderItem()
	{
		RenderManager::getInstance().destroyVertexBuffer(mVertexBuffer);
		mVertexBuffer = nullptr;
	}

	void RenderItem::rebuildGeometry(IRenderTarget* _target)
	{
		mCountVertex = 0;
		bool rotated = false;
		for (const auto& item : mDrawItems)
		{
			if (const auto* owner = item.first->getCroppedParent())
				rotated |= owner->_hasRotation();
		}
		if (rotated)
		{
			// Rotated triangles can gain vertices when clipped by ancestor bounds.
			// Generate in CPU storage first, then size the render buffer to fit.
			std::vector<Vertex> scratch(mNeedVertexCount);
			std::vector<Vertex> vertices;
			for (auto& item : mDrawItems)
			{
				mCurrentVertex = scratch.data();
				mLastVertexCount = 0;
				item.first->doRender();
				MYGUI_DEBUG_ASSERT(mLastVertexCount <= item.second, "It is too much vertexes");
				MYGUI_DEBUG_ASSERT(mLastVertexCount <= scratch.size(), "It is too much vertexes");
				if (mLastVertexCount == 0)
					continue;
				if (const auto* owner = item.first->getCroppedParent(); owner && owner->_hasRotation())
				{
					const auto* widget = static_cast<const Widget*>(owner);
					widget->_transformVertices(scratch.data(), mLastVertexCount, _target->getInfo());
					appendClippedTriangles(vertices, scratch.data(), mLastVertexCount, widget, _target->getInfo());
				}
				else
					vertices.insert(vertices.end(), scratch.begin(), scratch.begin() + mLastVertexCount);
			}
			mVertexBuffer->setVertexCount(std::max(mNeedVertexCount, vertices.size()));
			if (Vertex* buffer = mVertexBuffer->lock())
			{
				std::copy(vertices.begin(), vertices.end(), buffer);
				mVertexBuffer->unlock();
				mCountVertex = vertices.size();
			}
		}
		else if (Vertex* buffer = mVertexBuffer->lock())
		{
			for (auto& item : mDrawItems)
			{
				mCurrentVertex = buffer;
				mLastVertexCount = 0;
				item.first->doRender();
				MYGUI_DEBUG_ASSERT(mLastVertexCount <= item.second, "It is too much vertexes");
				buffer += mLastVertexCount;
				mCountVertex += mLastVertexCount;
			}
			mVertexBuffer->unlock();
		}

		mOutOfDate = false;
	}

	void RenderItem::renderToTarget(IRenderTarget* _target, bool _update)
	{
		if (mTexture == nullptr)
			return;

		mRenderTarget = _target;

		mCurrentUpdate = _update;

		if (mOutOfDate || _update)
			rebuildGeometry(_target);

		// batch doesn't render with 0 count, but still avoid changing state
		if (0 != mCountVertex)
		{
#if MYGUI_DEBUG_MODE == 1
			if (!RenderManager::getInstance().checkTexture(mTexture))
			{
				auto textureName = mTexture->getName();
				mTexture = nullptr;
				MYGUI_EXCEPT("texture pointer is not valid, texture name '" << textureName << "'");
				return;
			}
#endif
			if (mManualRender)
			{
				for (auto& item : mDrawItems)
					item.first->doManualRender(mVertexBuffer, mTexture, mCountVertex);
			}
			else
			{
				_target->doRender(mVertexBuffer, mTexture, mCountVertex);
			}
		}
	}

	void RenderItem::removeDrawItem(ISubWidget* _item)
	{
		for (VectorDrawItem::iterator iter = mDrawItems.begin(); iter != mDrawItems.end(); ++iter)
		{
			if ((*iter).first == _item)
			{
				mNeedVertexCount -= (*iter).second;
				mDrawItems.erase(iter);
				mOutOfDate = true;

				mVertexBuffer->setVertexCount(mNeedVertexCount);

				// if all detached, notify parent
				if (mDrawItems.empty())
				{
					mTexture = nullptr;
					mNeedCompression = true;
				}

				return;
			}
		}
		MYGUI_EXCEPT("DrawItem not found");
	}

	void RenderItem::addDrawItem(ISubWidget* _item, size_t _count)
	{
#if MYGUI_DEBUG_MODE == 1
		for (const auto& item : mDrawItems)
		{
			MYGUI_ASSERT(item.first != _item, "DrawItem exist");
		}
#endif

		mDrawItems.emplace_back(_item, _count);
		mNeedVertexCount += _count;
		mOutOfDate = true;

		mVertexBuffer->setVertexCount(mNeedVertexCount);
	}

	void RenderItem::reallockDrawItem(ISubWidget* _item, size_t _count)
	{
		for (auto& item : mDrawItems)
		{
			if (item.first == _item)
			{
				// if smaller, keep as is
				if (item.second < _count)
				{
					mNeedVertexCount -= item.second;
					mNeedVertexCount += _count;
					item.second = _count;
					mOutOfDate = true;

					mVertexBuffer->setVertexCount(mNeedVertexCount);
				}
				return;
			}
		}
		MYGUI_EXCEPT("DrawItem not found");
	}

	void RenderItem::setTexture(ITexture* _value)
	{
		if (mTexture == _value)
			return;

		MYGUI_DEBUG_ASSERT(mVertexBuffer->getVertexCount() == 0, "change texture only empty buffer");
		MYGUI_DEBUG_ASSERT(mNeedVertexCount == 0, "change texture only empty buffer");

		mTexture = _value;
	}

	ITexture* RenderItem::getTexture() const
	{
		return mTexture;
	}

	void RenderItem::setNeedCompression(bool _compression)
	{
		mNeedCompression = _compression;
	}

	bool RenderItem::getNeedCompression() const
	{
		return mNeedCompression;
	}

	void RenderItem::setManualRender(bool _value)
	{
		mManualRender = _value;
	}

	bool RenderItem::getManualRender() const
	{
		return mManualRender;
	}

	void RenderItem::outOfDate()
	{
		mOutOfDate = true;
	}

	bool RenderItem::isOutOfDate() const
	{
		return mOutOfDate;
	}

	size_t RenderItem::getNeedVertexCount() const
	{
		return mNeedVertexCount;
	}

	size_t RenderItem::getVertexCount() const
	{
		return mCountVertex;
	}

	bool RenderItem::getCurrentUpdate() const
	{
		return mCurrentUpdate;
	}

	Vertex* RenderItem::getCurrentVertexBuffer() const
	{
		return mCurrentVertex;
	}

	void RenderItem::setLastVertexCount(size_t _count)
	{
		mLastVertexCount = _count;
	}

	IRenderTarget* RenderItem::getRenderTarget()
	{
		return mRenderTarget;
	}

} // namespace MyGUI

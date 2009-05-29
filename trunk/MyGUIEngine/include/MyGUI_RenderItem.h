/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_RENDER_ITEM_H__
#define __MYGUI_RENDER_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_IDrawItem.h"
#include "MyGUI_IRenderItem.h"
#include "MyGUI_IVertexBuffer.h"

namespace MyGUI
{

	struct Vertex
	{
		enum
		{
			CornerLT = 0,
			CornerRT = 2,
			CornerLB = 1,
			CornerRB = 5,
			CornerRT2 = 3,
			CornerLB2 = 4,
		};

		void set(float _x, float _y, float _z, float _u, float _v, uint32 _colour)
		{
			x = _x;
			y = _y;
			z = _z;
			u = _u;
			v = _v;
			colour = _colour;
		}

		float x, y, z;
		uint32 colour;
		float u, v;
	};

	class LayerNode;
	typedef std::pair<IDrawItem*, size_t> DrawItemInfo;
	typedef std::vector<DrawItemInfo> VectorDrawItem;

	class MYGUI_EXPORT RenderItem : public IRenderItem
	{
		MYGUI_RTTI_CHILD_HEADER ( RenderItem, IRenderItem );

	public:
		RenderItem(const std::string& _texture, LayerNode * _parent);
		virtual ~RenderItem();

		void _render(bool _update);

		void setTextureName(const std::string& _texture);
		const std::string& getTextureName() { return mTextureName; }

		void addDrawItem(IDrawItem* _item, size_t _count);
		void removeDrawItem(IDrawItem* _item);
		void reallockDrawItem(IDrawItem* _item, size_t _count);

		void outOfDate() { mOutDate = true; }

		size_t getNeedVertexCount() { return mNeedVertexCount; }

		bool getCurrentUpdate() { return mCurrentUpdate; }
		Vertex* getCurrentVertextBuffer() { return mCurrentVertext; }

		void setLastVertexCount(size_t _count) { mLastVertextCount = _count; }

		/** Get maximum depth */
		float getMaximumDepth() { return mMaximumDepth; }

		/** Get X pixel scale */
		float getPixScaleX() { return mPixScaleX; }
		/** Get Y pixel scale */
		float getPixScaleY() { return mPixScaleY; }

		/** Get horisontal texel offset divided by window width */
		float getHOffset() { return mHOffset; }
		/** Get vertical texel offset divided by window height */
		float getVOffset() { return mVOffset; }

		/** Get aspect coefficient */
		float getAspectCoef() { return mAspectCoef; }

	private:
		std::string mTextureName;

		size_t mNeedVertexCount;

		bool mOutDate;
		VectorDrawItem mDrawItems;

		LayerNode * mParent;
		LayerManager * mLayerManager;

		// колличество отрендренных реально вершин
		size_t mCountVertex;

		bool mCurrentUpdate;
		Vertex* mCurrentVertext;
		size_t mLastVertextCount;

		IVertexBuffer* mVertexBuffer;

		//FIXME
		// координата зю
		static float mMaximumDepth;

		// размер пикселя в относительных координатах
		static float mPixScaleX;
		static float mPixScaleY;

		// смещение для того, чтобы тексель попал в пиксель
        static float mHOffset;
        static float mVOffset;

		static float mAspectCoef;

		IntSize mViewSize;
	};

} // namespace MyGUI

#endif // __MYGUI_RENDER_ITEM_H__

/*!
	@file
	@author		Albert Semenov
	@date		06/2009
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_ManualFont.h"
#include "MyGUI_Common.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	const unsigned char FONT_MASK_SELECT = 0x88;
	const unsigned char FONT_MASK_SELECT_DEACTIVE = 0x60;
	const unsigned char FONT_MASK_SPACE = 0x00;
	const unsigned char FONT_MASK_CHAR = 0xFF;
	const size_t FONT_TEXTURE_WIDTH = 1024;

	ManualFont::ManualFont() :
		mDefaultHeight(0),
		mTexture(nullptr)
	{
	}

	ManualFont::~ManualFont()
	{
	}

	GlyphInfo * ManualFont::getGlyphInfo(Char _id)
	{
		for (VectorRangeInfo::iterator iter=mVectorRangeInfo.begin(); iter!=mVectorRangeInfo.end(); ++iter)
		{
			GlyphInfo * info = iter->getInfo(_id);
			if (info == nullptr) continue;
			return info;
		}
		// ��� ������� ���������� ������
		return & mSpaceGlyphInfo;
	}

	void ManualFont::checkTexture()
	{
		if (mTexture == nullptr)
		{
			RenderManager& render = RenderManager::getInstance();
			mTexture = render.getByName(mSource);
			if (mTexture == nullptr)
			{
				mTexture = render.createTexture( mSource , ResourceManager::getInstance().getResourceGroup() );
				mTexture->loadFromFile(mSource);
			}
		}
	}

	void ManualFont::addGlyph(GlyphInfo * _info, Char _index, int _left, int _top, int _right, int _bottom, int _finalw, int _finalh, float _aspect, int _addHeight)
	{
		_info->codePoint = _index;
		_info->uvRect.left = (float)_left / (float)_finalw;  // u1
		_info->uvRect.top = (float)(_top + _addHeight) / (float)_finalh;  // v1
		_info->uvRect.right = (float)( _right ) / (float)_finalw; // u2
		_info->uvRect.bottom = ( _bottom + _addHeight ) / (float)_finalh; // v2
		_info->aspectRatio = _aspect * (_info->uvRect.right - _info->uvRect.left)  / (_info->uvRect.bottom - _info->uvRect.top);
	}

	void ManualFont::addGlyph(Char _code, const IntCoord& _coord)
	{
		mVectorPairCodeCoord.push_back(PairCodeCoord(_code, _coord));
	}

	void ManualFont::initialise()
	{
		if (mVectorPairCodeCoord.empty()) return;

		std::sort(mVectorPairCodeCoord.begin(), mVectorPairCodeCoord.end());

		const IntSize& size = SkinManager::getTextureSize(mSource);
		float aspect = (float)size.width / (float)size.height;

		Char code = mVectorPairCodeCoord.front().code;
		size_t count = mVectorPairCodeCoord.size();
		size_t first = 0;

		for (size_t pos=1; pos<count; ++pos)
		{
			// �������� ���������
			if (code + 1 != mVectorPairCodeCoord[pos].code)
			{
				addRange(mVectorPairCodeCoord, first, pos-1, size.width, size.height, aspect);
				code = mVectorPairCodeCoord[pos].code;
				first = pos;
			}
			else
			{
				code ++;
			}
		}

		addRange(mVectorPairCodeCoord, first, count-1, size.width, size.height, aspect);

		// ���������� �����
		VectorPairCodeCoord tmp;
		std::swap(tmp, mVectorPairCodeCoord);

		checkTexture();
	}

	void ManualFont::addRange(VectorPairCodeCoord & _info, size_t _first, size_t _last, int _width, int _height, float _aspect)
	{
		RangeInfo range = RangeInfo(_info[_first].code, _info[_last].code);

		for (size_t pos=_first; pos<=_last; ++pos)
		{
			GlyphInfo * info = range.getInfo(_info[pos].code);
			const IntCoord & coord = _info[pos].coord;
			addGlyph(info, _info[pos].code, coord.left, coord.top, coord.right(), coord.bottom(), _width, _height, _aspect);
		}

		mVectorRangeInfo.push_back(range);
	}

	void ManualFont::deserialization(xml::ElementPtr _node, Version _version)
	{
		mName = _node->findAttribute("name");
		mSource = _node->findAttribute("source");
		mDefaultHeight = utility::parseInt(_node->findAttribute("default_height"));

		xml::ElementEnumerator range = _node->getElementEnumerator();
		while (range.next("Code"))
		{
			std::string range_value;
			std::vector<std::string> parse_range;

			// ������� ������
			if (range->findAttribute("index", range_value))
			{
				addGlyph(utility::parseUInt(range_value), utility::parseValue<IntCoord>(range->findAttribute("coord")));
			}
		}

		// ��������������
		initialise();
	}

} // namespace MyGUI

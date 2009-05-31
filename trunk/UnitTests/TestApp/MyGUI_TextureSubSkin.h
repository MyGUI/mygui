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
#ifndef __MYGUI_TEXTURE_SUB_SKIN_H__
#define __MYGUI_TEXTURE_SUB_SKIN_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ISubWidgetRect.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_TextureLayerNode.h"

namespace MyGUI
{

	struct SubSkinStateData : public StateInfo
	{
		IntCoord coord;
		IntSize size;
	};

	class RenderItem;

	class /*MYGUI_EXPORT */TextureSubSkin : public ISubWidgetRect
	{
		MYGUI_RTTI_CHILD_HEADER(TextureSubSkin, ISubWidgetRect);

	public:
		TextureSubSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent);
		virtual ~TextureSubSkin();

		void setAlpha(float _alpha);

		virtual void setVisible(bool _visible);

		void _updateView();
		void _correctView();

		void _setAlign(const IntSize& _oldsize, bool _update);
		void _setAlign(const IntCoord& _oldcoord, bool _update);


		virtual void _setUVSet(const FloatRect& _rect);
		virtual void setStateData(StateInfo * _data);

		virtual void createDrawItem(const std::string& _texture, ILayerNode * _keeper);
		virtual void destroyDrawItem();

		// ����� ��� ��������� ����
		virtual void doRender();

		// ����� ��� ��������� ������ �� �������� xml
		static StateInfo * createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version);

	protected:
		FloatRect mRectTexture;
		bool mEmptyView;

		uint32 mCurrentAlpha;

		FloatRect mCurrentTexture;
		IntCoord mCurrentCoord;

		RenderItem* mRenderItem;
		TextureLayerNode* mNode;
		ITexture* mTexture;
		SubSkinStateData * mData;
	};

} // namespace MyGUI

#endif // __MYGUI_TEXTURE_SUB_SKIN_H__

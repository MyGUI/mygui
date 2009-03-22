/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
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
#include "MyGUI_SubSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"

namespace MyGUI
{

	struct SubSkinStateData : public StateInfo
	{
		FloatRect rect;
	};

	const size_t SUBSKIN_COUNT_VERTEX = VERTEX_IN_QUAD;

	SubSkin::SubSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		ISubWidgetRect(_info.coord, _info.align, _parent),
		mEmptyView(false),
		mCurrentAlpha(0xFFFFFFFF),
		mCurrentCoord(_info.coord),
		mRenderItem(nullptr)
	{
		mManager = LayerManager::getInstancePtr();
	}

	SubSkin::~SubSkin()
	{
	}

	void SubSkin::setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::setAlpha(float _alpha)
	{
		mCurrentAlpha = 0x00FFFFFF | ((uint8)(_alpha*255) << 24);
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::_correctView()
	{
		//mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void SubSkin::_setAlign(const IntSize& _size, bool _update)
	{

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (mAlign.isHStretch()) {
			// растягиваем
			mCoord.width = mCoord.width + (mCroppedParent->getWidth() - _size.width);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isRight()) {
			// двигаем по правому краю
			mCoord.left = mCoord.left + (mCroppedParent->getWidth() - _size.width);
			need_update = true;
		}
		else if (mAlign.isHCenter()) {
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (mAlign.isVStretch()) {
			// растягиваем
			mCoord.height = mCoord.height + (mCroppedParent->getHeight() - _size.height);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isBottom()) {
			// двигаем по нижнему краю
			mCoord.top = mCoord.top + (mCroppedParent->getHeight() - _size.height);
			need_update = true;
		}
		else if (mAlign.isVCenter()) {
			// выравнивание по вертикали без растяжения
			mCoord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) {
			mCurrentCoord = mCoord;
			_updateView();
		}

	}

	void SubSkin::_updateView()
	{

		//mAbsolutePosition = mCroppedParent->getAbsolutePosition() + mCoord.point();

		bool margin = _checkMargin();

		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// скрываем
				//mEmptyView = true;
				//mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));

				// запоминаем текущее состояние
				mIsMargin = margin;

				// обновить перед выходом
				if (nullptr != mRenderItem) mRenderItem->outOfDate();
				return;

			}
		}

		if ((mIsMargin) || (margin)) { // мы обрезаны или были обрезаны

			mCurrentCoord.width = _getViewWidth();
			mCurrentCoord.height = _getViewHeight();

			if ((mCurrentCoord.width > 0) && (mCurrentCoord.height > 0)) {

				// теперь смещаем текстуру
				float UV_lft = mMargin.left / (float)mCoord.width;
				float UV_top = mMargin.top / (float)mCoord.height;
				float UV_rgt = (mCoord.width - mMargin.right) / (float)mCoord.width;
				float UV_btm = (mCoord.height - mMargin.bottom) / (float)mCoord.height;

				float UV_sizeX = mRectTexture.right - mRectTexture.left;
				float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

				float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
				float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
				float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
				float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

				mCurrentTexture.set(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);
			}
		}

		if ((mIsMargin) && (false == margin)) {
			// мы не обрезаны, но были, ставим базовые координаты
			mCurrentTexture = mRectTexture;
		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		// если скин был скрыт, то покажем
		//mEmptyView = false;
		//mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	size_t SubSkin::_drawItem(Vertex * _vertex, bool _update)
	{
		if ((false == mVisible) || mEmptyView) return 0;

		float vertex_z = mManager->getMaximumDepth();
		//vertex_z = 0;

		float vertex_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float vertex_right = vertex_left + (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float vertex_bottom = vertex_top - (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);

		// first triangle - left top
		_vertex[0].x = vertex_left;
		_vertex[0].y = vertex_top;
		_vertex[0].z = vertex_z;
		_vertex[0].colour = mCurrentAlpha;
		_vertex[0].u = mCurrentTexture.left;
		_vertex[0].v = mCurrentTexture.top;


		// first triangle - left bottom
		_vertex[1].x = vertex_left;
		_vertex[1].y = vertex_bottom;
		_vertex[1].z = vertex_z;
		_vertex[1].colour = mCurrentAlpha;
		_vertex[1].u = mCurrentTexture.left;
		_vertex[1].v = mCurrentTexture.bottom;

		// first triangle - right top
		_vertex[2].x = vertex_right;
		_vertex[2].y = vertex_top;
		_vertex[2].z = vertex_z;
		_vertex[2].colour = mCurrentAlpha;
		_vertex[2].u = mCurrentTexture.right;
		_vertex[2].v = mCurrentTexture.top;

		// second triangle - right top
		_vertex[3].x = vertex_right;
		_vertex[3].y = vertex_top;
		_vertex[3].z = vertex_z;
		_vertex[3].colour = mCurrentAlpha;
		_vertex[3].u = mCurrentTexture.right;
		_vertex[3].v = mCurrentTexture.top;

		// second triangle = left bottom
		_vertex[4].x = vertex_left;
		_vertex[4].y = vertex_bottom;
		_vertex[4].z = vertex_z;
		_vertex[4].colour = mCurrentAlpha;
		_vertex[4].u = mCurrentTexture.left;
		_vertex[4].v = mCurrentTexture.bottom;

		// second triangle - right botton
		_vertex[5].x = vertex_right;
		_vertex[5].y = vertex_bottom;
		_vertex[5].z = vertex_z;
		_vertex[5].colour = mCurrentAlpha;
		_vertex[5].u = mCurrentTexture.right;
		_vertex[5].v = mCurrentTexture.bottom;

		return SUBSKIN_COUNT_VERTEX;
	}

	void SubSkin::_createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item)
	{
		MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");
		mRenderItem = _item;
		mRenderItem->addDrawItem(this, SUBSKIN_COUNT_VERTEX);
	}

	void SubSkin::_destroyDrawItem()
	{
		MYGUI_ASSERT(mRenderItem, "mRenderItem must be not nullptr");
		mRenderItem->removeDrawItem(this);
		mRenderItem = nullptr;
	}

	void SubSkin::_setStateData(StateInfo * _data)
	{
		SubSkinStateData * data = (SubSkinStateData*)_data;
		_setUVSet(data->rect);
	}

	void SubSkin::_setUVSet(const FloatRect& _rect)
	{
		if (mRectTexture == _rect) return;
		mRectTexture = _rect;

		// если обрезаны, то просчитываем с учето обрезки
		if (mIsMargin) {
			float UV_lft = mMargin.left / (float)mCoord.width;
			float UV_top = mMargin.top / (float)mCoord.height;
			float UV_rgt = (mCoord.width - mMargin.right) / (float)mCoord.width;
			float UV_btm = (mCoord.height - mMargin.bottom) / (float)mCoord.height;

			float UV_sizeX = mRectTexture.right - mRectTexture.left;
			float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

			float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
			float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
			float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
			float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

			mCurrentTexture.set(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);
		}

		// мы не обрезаны, базовые координаты
		else {
			mCurrentTexture = mRectTexture;
		}

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	StateInfo * SubSkin::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		std::string texture = _root->findAttribute("texture");

		// поддержка замены тегов в скинах
		if (_version >= Version(1, 1)) {
			texture = LanguageManager::getInstance().replaceTags(texture);
		}

		const IntSize & size = SkinManager::getInstance().getTextureSize(texture);
		SubSkinStateData * data = new SubSkinStateData();
		const FloatRect & source = FloatRect::parse(_node->findAttribute("offset"));
		data->rect = SkinManager::getInstance().convertTextureCoord(source, size);
		return data;
	}

} // namespace MyGUI

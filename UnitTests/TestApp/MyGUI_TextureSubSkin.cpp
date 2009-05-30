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
#include "MyGUI_Precompiled.h"
#include "MyGUI_TextureSubSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_TextureLayerNode.h"
#include "MyGUI_RenderManager.h"
#include <Ogre.h>

#include <windows.h>

namespace MyGUI
{

	/*struct PixelBox
	{
		uint32* ptr;
		int texture_width, texture_height;
		int x, y, w, h;
	};*/


	const size_t VERTEX_IN_QUAD = 6;
	const size_t SUBSKIN_COUNT_VERTEX = VERTEX_IN_QUAD;

	TextureSubSkin::TextureSubSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		ISubWidgetRect(_info.coord, _info.align, _parent),
		mEmptyView(false),
		mCurrentAlpha(0xFFFFFFFF),
		mCurrentCoord(_info.coord),
		mRenderItem(nullptr),
		mData(nullptr)
	{
	}

	TextureSubSkin::~TextureSubSkin()
	{
	}

	void TextureSubSkin::setVisible(bool _visible)
	{
		if (mVisible == _visible) return;
		mVisible = _visible;

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void TextureSubSkin::setAlpha(float _alpha)
	{
		mCurrentAlpha = 0x00FFFFFF | ((uint8)(_alpha*255) << 24);
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void TextureSubSkin::_correctView()
	{
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void TextureSubSkin::_setAlign(const IntCoord& _oldcoord, bool _update)
	{
		_setAlign(_oldcoord.size(), _update);
	}

	void TextureSubSkin::_setAlign(const IntSize& _oldsize, bool _update)
	{

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (mAlign.isHStretch())
		{
			// растягиваем
			mCoord.width = mCoord.width + (mCroppedParent->getWidth() - _oldsize.width);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isRight())
		{
			// двигаем по правому краю
			mCoord.left = mCoord.left + (mCroppedParent->getWidth() - _oldsize.width);
			need_update = true;
		}
		else if (mAlign.isHCenter())
		{
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (mAlign.isVStretch())
		{
			// растягиваем
			mCoord.height = mCoord.height + (mCroppedParent->getHeight() - _oldsize.height);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isBottom())
		{
			// двигаем по нижнему краю
			mCoord.top = mCoord.top + (mCroppedParent->getHeight() - _oldsize.height);
			need_update = true;
		}
		else if (mAlign.isVCenter())
		{
			// выравнивание по вертикали без растяжения
			mCoord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update)
		{
			mCurrentCoord = mCoord;
			_updateView();
		}

	}

	void TextureSubSkin::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// вьюпорт стал битым
		if (margin)
		{
			// проверка на полный выход за границу
			if (_checkOutside())
			{
				// запоминаем текущее состояние
				mIsMargin = margin;

				// обновить перед выходом
				if (nullptr != mRenderItem) mRenderItem->outOfDate();
				return;
			}
		}

		// мы обрезаны или были обрезаны
		if ((mIsMargin) || (margin))
		{
			mCurrentCoord.width = _getViewWidth();
			mCurrentCoord.height = _getViewHeight();

			if ((mCurrentCoord.width > 0) && (mCurrentCoord.height > 0))
			{
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

		if ((mIsMargin) && (false == margin))
		{
			// мы не обрезаны, но были, ставим базовые координаты
			mCurrentTexture = mRectTexture;
		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void TextureSubSkin::createDrawItem(const std::string& _texture, ILayerNode * _node)
	{
		//MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

		//IRenderItem* item = _keeper->addToRenderItem(_texture, this);
		//mRenderItem = item->castType<RenderItem>();
		//mRenderItem->addDrawItem(this, SUBSKIN_COUNT_VERTEX);
		mNode = _node->castType<TextureLayerNode>();
		mNode->addToRenderItem(_texture, this);
		mTexture = RenderManager::getInstance().getByName(_texture);
	}

	void TextureSubSkin::destroyDrawItem()
	{
		mNode = nullptr;
		//MYGUI_ASSERT(mRenderItem, "mRenderItem must be not nullptr");

		//mRenderItem->removeDrawItem(this);
		//mRenderItem = nullptr;
	}

	void TextureSubSkin::setStateData(StateInfo * _data)
	{
		mData = (SubSkinStateData*)_data;
		//_setUVSet(data->rect);
	}

	void TextureSubSkin::_setUVSet(const FloatRect& _rect)
	{
		if (mRectTexture == _rect) return;
		mRectTexture = _rect;

		// если обрезаны, то просчитываем с учето обрезки
		if (mIsMargin)
		{
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
		else
		{
			mCurrentTexture = mRectTexture;
		}

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	StateInfo * TextureSubSkin::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		std::string texture = _root->findAttribute("texture");

		// поддержка замены тегов в скинах
		if (_version >= Version(1, 1))
		{
			texture = LanguageManager::getInstance().replaceTags(texture);
		}

		SubSkinStateData * data = new SubSkinStateData();

		data->rect = IntRect::parse(_node->findAttribute("offset"));
		data->size = SkinManager::getInstance().getTextureSize(texture);

		return data;
	}

	uint32 blend(uint32 _col1, uint32 _col2)
	{
		uint8 a1 = _col1 >> 24;
		uint8 a2 = _col2 >> 24;
		uint32 rgb1 = _col1 & 0xFFFFFF;
		uint32 rgb2 = _col2 & 0xFFFFFF;

		 uint32 rgb = _col1 + _col2 * (0xFF - a1)/0xFF;
        uint32 a = a1 + a2 * (0xFF - a1)/0xFF;

		return a1 << 24 & rgb;
	}

	void TextureSubSkin::doRender()
	{
		if (!mVisible || mEmptyView || mNode == nullptr || mData == nullptr) return;

		TextureLayerNode* node = static_cast<TextureLayerNode*>(mNode);

		uint32* dest = node->getTextureLock();
		int dest_width = node->getWidth();

		uint32* data = new uint32[mData->size.width * mData->size.height];
		memcpy(data, mTexture->lock(false), mData->size.width * mData->size.height * 4);
		Ogre::Image source_image;
		source_image.loadDynamicImage((Ogre::uchar*)data, mData->size.width, mData->size.height, 1, Ogre::PF_A8R8G8B8, true);
		source_image.resize(mCoord.width, mCoord.height, Ogre::Image::FILTER_LINEAR);
		uint32* source = (uint32*)source_image.getData();

		for (int y=0; y<mCoord.height; ++y)
		{
			for (int x=0; x<mCoord.width; ++x)
			{
				uint32& pixdest = dest[(y + mCoord.top) * dest_width + (x + mCoord.left)];
				pixdest = blend(pixdest, source[y * mCoord.width + x]);
			}
		}

		/*PixelBox destbox;
		destbox.ptr = dest;
		destbox.texture_width = 1024;
		destbox.texture_height = 1024;

		PixelBox sourcebox;
		sourcebox.ptr = source;
		sourcebox.texture_width = mData->size.width;
		sourcebox.texture_height = mData->size.height;*/

		//RasterAlphaBlend(destbox, sourcebox);//1024, 1024, mData->size.width, mData->size.height, (DWORD*)dest, (DWORD*)source);


		//node->mTexture->unlock();
		mTexture->unlock();

	}

} // namespace MyGUI

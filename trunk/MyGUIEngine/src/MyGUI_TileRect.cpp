/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_TileRect.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	const size_t TILERECT_COUNT_VERTEX = 16 * VERTEX_IN_QUAD;

	TileRect::TileRect(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		CroppedRectangleInterface(_info.coord, _info.align, _parent),
		mEmptyView(false),
		mRenderItem(null),
		mCurrentCoord(_info.coord),
		mCurrentAlpha(0xFFFFFFFF),
		mTileSize(_info.coord.size()),
		mCountVertex(TILERECT_COUNT_VERTEX)
	{
		mManager = LayerManager::getInstancePtr();
		updateTextureData();
	}

	TileRect::~TileRect()
	{
	}

	void TileRect::show()
	{
		if (mShow) return;
		mShow = true;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void TileRect::hide()
	{
		if (false == mShow) return;
		mShow = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void TileRect::setAlpha(float _alpha)
	{
		mCurrentAlpha = 0x00FFFFFF | ((uint8)(_alpha*255) << 24);
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void TileRect::_correctView()
	{
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void TileRect::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void TileRect::_setAlign(const IntSize& _size, bool _update)
	{

		// растягиваем
		mCoord.width = mCoord.width + (mParent->getWidth() - _size.width);
		mCoord.height = mCoord.height + (mParent->getHeight() - _size.height);
		mIsMargin = true; // при изменении размеров все пересчитывать

		mCurrentCoord = mCoord;
		_updateView();

	}

	void TileRect::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;
		mCurrentCoord.width = getViewWidth();
		mCurrentCoord.height = getViewHeight();

		// подсчитываем необходимое колличество тайлов
		if (false == mEmptyView) {
			size_t count_x = mCoord.width / mTileSize.width;
			if ((mCoord.width % mTileSize.width) > 0) count_x ++;
			size_t count = mCoord.height / mTileSize.height;
			if ((mCoord.height % mTileSize.height) > 0) count ++;
			count = count * count_x * VERTEX_IN_QUAD;

			// нужно больше вершин
			if (count > mCountVertex) {
				mCountVertex = count + TILERECT_COUNT_VERTEX;
				if (null != mRenderItem) mRenderItem->reallockDrawItem(this, mCountVertex);
			}
		}

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// запоминаем текущее состояние
				mIsMargin = margin;

				// обновить перед выходом
				if (null != mRenderItem) mRenderItem->outOfDate();
				return;

			}
		}

		// запоминаем текущее состояние
		mIsMargin = margin;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void TileRect::_setUVSet(const FloatRect& _rect)
	{
		mCurrentTexture = _rect;
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	size_t TileRect::_drawItem(Vertex * _vertex, bool _update)
	{
		if ((false == mShow) || mEmptyView) return 0;

		if (_update) updateTextureData();

		float vertex_z = mManager->getMaximumDepth();

		// абсолютный размер окна
		float window_left = ((mManager->getPixScaleX() * (float)(mCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float window_right = window_left + (mManager->getPixScaleX() * (float)mCoord.width * 2);
		float window_top = -(((mManager->getPixScaleY() * (float)(mCoord.top + mParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float window_bottom = window_top - (mManager->getPixScaleY() * (float)mCoord.height * 2);

		// размер вьюпорта
		float real_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float real_right = real_left + (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float real_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float real_bottom = real_top - (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);

		size_t count = 0;

		float left = window_left;
		float right = window_left;
		float top = window_top;
		float bottom = window_top;

		for (int y=0; y<mCoord.height; y+=mTileSize.height) {
			top = bottom;
			bottom -= mRealTileHeight;
			right = window_left;

			float vertex_top = top;
			float vertex_bottom = bottom;
			bool texture_crop_height  = false;

			if (vertex_top > real_top) {
				// проверка на полный выход
				if (vertex_bottom > real_top) {
					continue;
				}
				// обрезаем
				vertex_top = real_top;
				texture_crop_height = true;
			}
			if (vertex_bottom < real_bottom) {
				// вообще вниз ушли
				if (vertex_top < real_bottom) {
					continue;
				}
				// обрезаем
				vertex_bottom = real_bottom;
				texture_crop_height = true;
			}

			for (int x=0; x<mCoord.width; x+=mTileSize.width) {
				left = right;
				right += mRealTileWidth;

				float vertex_left = left;
				float vertex_right = right;
				bool texture_crop_width  = false;


				if (vertex_left < real_left) {
					// проверка на полный выход
					if (vertex_right < real_left) {
						continue;
					}
					// обрезаем
					vertex_left = real_left;
					texture_crop_width = true;
				}
				if (vertex_right > real_right) {
					// вообще строку до конца не нуна
					if (vertex_left > real_right) {
						continue;
					}
					// обрезаем
					vertex_right = real_right;
					texture_crop_width = true;
				}

				// текущие текстурные координаты
				float texture_left = mCurrentTexture.left;
				float texture_right = mCurrentTexture.right;
				float texture_top = mCurrentTexture.top;
				float texture_bottom = mCurrentTexture.bottom;

				// смещение текстуры по вертикили
				if (texture_crop_height) {
					// прибавляем размер смещения в текстурных координатах
					texture_top += (top - vertex_top) * mTextureHeightOne;
					// отнимаем размер смещения в текстурных координатах
					texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
				}

				// смещение текстуры по горизонтали
				if (texture_crop_width) {
					// прибавляем размер смещения в текстурных координатах
					texture_left += (vertex_left - left) * mTextureWidthOne;
					// отнимаем размер смещения в текстурных координатах
					texture_right -= (right - vertex_right) * mTextureWidthOne;
				}

				// first triangle - left top
				_vertex[count].x = vertex_left;
				_vertex[count].y = vertex_top;
				_vertex[count].z = vertex_z;
				_vertex[count].colour = mCurrentAlpha;
				_vertex[count].u = texture_left;
				_vertex[count].v = texture_top;
				count++;
				
				// first triangle - left bottom
				_vertex[count].x = vertex_left;
				_vertex[count].y = vertex_bottom;
				_vertex[count].z = vertex_z;
				_vertex[count].colour = mCurrentAlpha;
				_vertex[count].u = texture_left;
				_vertex[count].v = texture_bottom;
				count++;

				// first triangle - right top
				_vertex[count].x = vertex_right;
				_vertex[count].y = vertex_top;
				_vertex[count].z = vertex_z;
				_vertex[count].colour = mCurrentAlpha;
				_vertex[count].u = texture_right;
				_vertex[count].v = texture_top;
				count++;

				// second triangle - right top
				_vertex[count].x = vertex_right;
				_vertex[count].y = vertex_top;
				_vertex[count].z = vertex_z;
				_vertex[count].colour = mCurrentAlpha;
				_vertex[count].u = texture_right;
				_vertex[count].v = texture_top;
				count++;

				// second triangle = left bottom
				_vertex[count].x = vertex_left;
				_vertex[count].y = vertex_bottom;
				_vertex[count].z = vertex_z;
				_vertex[count].colour = mCurrentAlpha;
				_vertex[count].u = texture_left;
				_vertex[count].v = texture_bottom;
				count++;

				// second triangle - right botton
				_vertex[count].x = vertex_right;
				_vertex[count].y = vertex_bottom;
				_vertex[count].z = vertex_z;
				_vertex[count].colour = mCurrentAlpha;
				_vertex[count].u = texture_right;
				_vertex[count].v = texture_bottom;
				count++;
			}
		}

		return count;
	}

	void TileRect::_createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item)
	{
		mRenderItem = _item;
		mRenderItem->addDrawItem(this, mCountVertex);
	}

	void TileRect::_destroyDrawItem()
	{
		mRenderItem->removeDrawItem(this);
	}

	void TileRect::updateTextureData()
	{
		// размер одного тайла
		mRealTileWidth = mManager->getPixScaleX() * (float)(mTileSize.width) * 2;
		mRealTileHeight = mManager->getPixScaleY() * (float)(mTileSize.height) * 2;

		mTextureHeightOne = (mCurrentTexture.bottom - mCurrentTexture.top) / mRealTileHeight;
		mTextureWidthOne = (mCurrentTexture.right - mCurrentTexture.left) / mRealTileWidth;
	}

} // namespace MyGUI

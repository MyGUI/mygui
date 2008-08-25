/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_MainSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	//const size_t SUBSKIN_COUNT_VERTEX = VERTEX_IN_QUAD;

	MainSkin::MainSkin(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		SubSkin(_info, _parent)
		// корректируем под маин
		/*CroppedRectangleInterface(IntCoord(IntPoint(), _parent->getSize()), Align::Stretch, _parent),
		mEmptyView(false),
		mRenderItem(null),
		mCurrentCoord(IntCoord(IntPoint(), _parent->getSize())),
		mCurrentAlpha(0xFFFFFFFF)*/
	{
		mAlign = Align::Stretch;
		setPosition(IntCoord(0, 0, _parent->getWidth(), _parent->getHeight()));
		//mCurrentCoord.set(0, 0, _parent->getWidth(), _parent->getHeight());
		/*mManager = LayerManager::getInstancePtr();

		mCoord = IntCoord(IntPoint(), _parent->getSize());
		mAlign = Align::Stretch;*/
	}

	MainSkin::~MainSkin()
	{
	}

	/*void MainSkin::show()
	{
		if (mShow) return;
		mShow = true;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::hide()
	{
		if (false == mShow) return;
		mShow = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::setAlpha(float _alpha)
	{
		mCurrentAlpha = 0x00FFFFFF | ((uint8)(_alpha*255) << 24);
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::_correctView()
	{
		//mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void MainSkin::_setAlign(const IntSize& _size, bool _update)
	{

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (mAlign.isRight()) {
			if (mAlign.isLeft()) {
				// растягиваем
				mCoord.width = mCoord.width + (mParent->getWidth() - _size.width);
				need_update = true;
				mIsMargin = true; // при изменении размеров все пересчитывать
			}
			else {
				// двигаем по правому краю
				mCoord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_update = true;
			}

		}
		else if (false == mAlign.isLeft()) {
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (mAlign.isBottom()) {
			if (mAlign.isTop()) {
				// растягиваем
				mCoord.height = mCoord.height + (mParent->getHeight() - _size.height);
				need_update = true;
				mIsMargin = true; // при изменении размеров все пересчитывать
			}
			else {
				mCoord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_update = true;
			}
		}
		else if (false == mAlign.isTop()) {
			// выравнивание по вертикали без растяжения
			mCoord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) {
			mCurrentCoord = mCoord;
			_updateView();
		}

	}

	void MainSkin::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= getViewWidth()) || (0 >= getViewHeight()));

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
				if (null != mRenderItem) mRenderItem->outOfDate();
				return;

			}
		}

		if ((mIsMargin) || (margin)) { // мы обрезаны или были обрезаны

			mCurrentCoord.width = getViewWidth();
			mCurrentCoord.height = getViewHeight();

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

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::_setUVSet(const FloatRect& _rect)
	{
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

		if (null != mRenderItem) mRenderItem->outOfDate();

	}

	size_t MainSkin::_drawItem(Vertex * _vertex, bool _update)
	{
		if ((false == mShow) || (mEmptyView)) return 0;
		//if ((0 >= getViewWidth()) || (0 >= getViewHeight())) return 0;

		float vertex_z = mManager->getMaximumDepth();

		float vertex_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float vertex_right = vertex_left + (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
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

	void MainSkin::_createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item)
	{
		mRenderItem = _item;
		mRenderItem->addDrawItem(this, SUBSKIN_COUNT_VERTEX);
	}

	void MainSkin::_destroyDrawItem()
	{
		mRenderItem->removeDrawItem(this);
	}*/

} // namespace MyGUI

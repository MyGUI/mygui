/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_LAYER_INFO_H__
#define __MYGUI_LAYER_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerItemInfo.h"
#include "MyGUI_PanelAlphaOverlayElement.h"

namespace MyGUI
{

	class _MyGUIExport LayerInfo
	{
		friend class LayerManager;

	private:
		LayerInfo(const std::string & _name, Ogre::ushort _start, Ogre::ushort _count, Ogre::ushort _height) :
			mStart(_start), mCount(_count?_count:1), mHeight(_height?_height:1), mName(_name)
		{
		}

		~LayerInfo()
		{
			// нуно отсоединить
			for (size_t pos=0; pos<mItems.size(); pos++) {

				// защита от удаления после завершения
				Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
				if (manager != null) {
					// отсоединяем
					mItems[pos]->detachToOverlay(mItems[pos]->mOverlayInfo);
					// и удаляем оверлей
					manager->destroy(mItems[pos]->mOverlayInfo);
				}
				mItems[pos]->mOverlayInfo = 0;
				mItems[pos]->mLayerInfo = null;
			}
		}

		inline LayerItemInfoPtr findItem(int _left, int _top, LayerItemInfoPtr & rootItem)
		{
			// проверяем сверху вниз
			for (VectorLayerItemInfo::reverse_iterator iter=mItems.rbegin(); iter!=mItems.rend(); iter++) {
				LayerItemInfoPtr item = (*iter)->findItem(_left, _top);
				if (item != 0) {
					rootItem = (*iter);
					return item;
				}
			}
			return 0;
		}

		bool addItem(LayerItemInfoPtr _item)
		{
			// это чтоб по два раза не коннектили
			MYGUI_ASSERT(null == _item->mOverlayInfo, "Only one pointer set must be load");
			// создаем оверлей и присоединяем к нему
			static long num=0;
			Ogre::Overlay * overlay = Ogre::OverlayManager::getSingleton().create(util::toString(num++, "_LayerInfo"));
			overlay->show();
			_item->attachToOverlay(overlay);
			// инициализируем
			_item->mOverlayInfo = overlay;
			_item->mLayerInfo = this;
			// если достигли максимального, то лепим в верхний
			Ogre::ushort pos = (mItems.size() <= mCount) ? (Ogre::ushort)mItems.size() : (mCount-1);
			// добавляем и ставим высоту
			mItems.push_back(_item);
			_item->mOverlayInfo->setZOrder(mStart + pos * mHeight);
			return true;
		}

		inline void upItem(LayerItemInfoPtr _item)
		{
			// один слой только, выходим
			if (mCount < 2) return;
			// поднимаем, но не удаляем
			_upItem(_item, false);
		}

		inline bool removeItem(LayerItemInfoPtr _item)
		{
			if (_item->mOverlayInfo == null) return false;
			// отсоединить и удалить оверлей
			_item->detachToOverlay(_item->mOverlayInfo);
			// и удаляем оверлей
			Ogre::OverlayManager::getSingleton().destroy(_item->mOverlayInfo);
			_item->mOverlayInfo = 0;
			_item->mLayerInfo = 0;
			// поднимаем, и удаляем
			_upItem(_item, true);
			return true;
		}

		inline void _upItem(LayerItemInfoPtr _item, bool _destroy)
		{
			// ищем наш элемент
			bool find = false;
			size_t count_check = mItems.size()-1;
			Ogre::ushort current = mStart;
			for (size_t pos=0; pos<count_check; pos++) {

				if ((find) || (mItems[pos] == _item)) {
					find = true;
					// сдвигаем все элементы вниз
					if (pos < count_check) mItems[pos] = mItems[pos+1];
					if (mItems[pos]->mOverlayInfo->getZOrder() != current) mItems[pos]->mOverlayInfo->setZOrder(current);
				}

				// следующая высота
				if (pos < (Ogre::ushort)(mCount-1)) current += mHeight;
			}

			if (_destroy) {
				// удаляем самый верхний
				mItems.pop_back();
			} else {
				// в верхний записываем тот что мы затерли
				mItems[count_check] = _item;
				if (_item->mOverlayInfo->getZOrder() != current) _item->mOverlayInfo->setZOrder(current);
			}
		}

	private:
		VectorLayerItemInfo mItems;
		Ogre::ushort mStart, mCount, mHeight;
		std::string mName;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_INFO_H__

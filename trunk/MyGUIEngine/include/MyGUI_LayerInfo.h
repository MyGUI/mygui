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

namespace MyGUI
{

	class _MyGUIExport LayerInfo
	{
		friend class LayerManager;

	private:
		LayerInfo(const std::string & _name, Ogre::ushort _start, Ogre::ushort _count, Ogre::ushort _height) :
			mStart(_start), mCount(_count?_count:1), mHeight(_height?_height:1),
			mCountSharedItem(0), mName(_name),
			mOverlay(null)
		{
		}

		~LayerInfo()
		{
			// ���� �����������
			for (size_t pos=0; pos<mItems.size(); pos++) {

				// ������ �� �������� ����� ����������
				Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
				if (manager != null) {
					// �����������
					mItems[pos]->detachToOverlay(mItems[pos]->mOverlayInfo);
					// � ������� �������
					manager->destroy(mItems[pos]->mOverlayInfo);
				}
				mItems[pos]->mOverlayInfo = 0;
				mItems[pos]->mLayerInfo = null;
			}
		}

		inline LayerItemInfoPtr findItem(int _left, int _top, LayerItemInfoPtr & rootItem)
		{
			// ��������� ������ ����
			for (VectorLayerItemInfo::reverse_iterator iter=mItems.rbegin(); iter!=mItems.rend(); iter++) {
				LayerItemInfoPtr item = (*iter)->findItem(_left, _top);
				if (item != 0) {
					rootItem = (*iter);
					return item;
				}
			}
			return 0;
		}

		inline void upItem(LayerItemInfoPtr _item)
		{
			// ���� ���� ������, �������
			if (mCount < 2) return;
			// ���������, �� �� �������
			_upItem(_item, false);
		}

		bool addItem(LayerItemInfoPtr _item)
		{
			// ��� ���� �� ��� ���� �� ����������
			MYGUI_ASSERT(null == _item->mOverlayInfo, "Only one mouse pointer set must be load");

			static long num=0;

			// ���� ���� ����, �� ������ ��� � ���� �������
			if ((1 != mCount) || (0 == mCountSharedItem)) {
				// ������� ������� � ������������ � ����
				mOverlay = Ogre::OverlayManager::getSingleton().create(utility::toString(num++, "_LayerInfo"));
				mOverlay->show();
			}

			// ��������� ������ ��� ������������ ������
			mCountSharedItem++;

			_item->attachToOverlay(mOverlay);
			// ��������������
			_item->mOverlayInfo = mOverlay;
			_item->mLayerInfo = this;
			// ���� �������� �������������, �� ����� � �������
			Ogre::ushort pos = (mItems.size() <= mCount) ? (Ogre::ushort)mItems.size() : (mCount-1);
			// ��������� � ������ ������
			mItems.push_back(_item);
			_item->mOverlayInfo->setZOrder(mStart + pos * mHeight);
			return true;
		}

		inline bool removeItem(LayerItemInfoPtr _item)
		{
			if (_item->mOverlayInfo == null) return false;
			// ����������� � ������� �������
			_item->detachToOverlay(_item->mOverlayInfo);

			// ��������� ������ ��� ������������ ������
			mCountSharedItem--;

			if ((1 != mCount) || (0 == mCountSharedItem)) {
				// � ������� �������
				Ogre::OverlayManager::getSingleton().destroy(_item->mOverlayInfo);
				mOverlay = null;
			}

			_item->mOverlayInfo = 0;
			_item->mLayerInfo = 0;
			// ���������, � �������
			_upItem(_item, true);
			return true;
		}

		inline void _upItem(LayerItemInfoPtr _item, bool _destroy)
		{
			// ���� ��� �������
			bool find = false;
			size_t count_check = mItems.size()-1;
			Ogre::ushort current = mStart;
			for (size_t pos=0; pos<count_check; pos++) {

				if ((find) || (mItems[pos] == _item)) {
					find = true;
					// �������� ��� �������� ����
					if (pos < count_check) mItems[pos] = mItems[pos+1];
					if (mItems[pos]->mOverlayInfo->getZOrder() != current) mItems[pos]->mOverlayInfo->setZOrder(current);
				}

				// ��������� ������
				if (pos < (Ogre::ushort)(mCount-1)) current += mHeight;
			}

			if (_destroy) {
				// ������� ����� �������
				mItems.pop_back();
			} else {
				// � ������� ���������� ��� ��� �� �������
				mItems[count_check] = _item;
				if (_item->mOverlayInfo->getZOrder() != current) _item->mOverlayInfo->setZOrder(current);
			}
		}
	public:
		inline const std::string & getName() {return mName;}

	private:
		VectorLayerItemInfo mItems;
		Ogre::ushort mStart, mCount, mHeight;
		size_t mCountSharedItem;
		std::string mName;

		Ogre::Overlay * mOverlay;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_INFO_H__

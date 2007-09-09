#pragma once

#include <Ogre.h>
#include "Common.h"
#include "LayerItemInfo.h"
#include "PanelAlphaOverlayElement.h"

namespace widget
{

	class LayerManager;

	class LayerInfo
	{
		friend LayerManager;

	private:
		LayerInfo(const std::string & _name, Ogre::ushort _start, Ogre::ushort _count, Ogre::ushort _height) :
			m_name(_name), m_start(_start), m_count(_count?_count:1), m_height(_height?_height:1)
		{
		}

		~LayerInfo()
		{
			// ���� �����������
			for (size_t pos=0; pos<m_items.size(); pos++) {

				// ������ �� �������� ����� ����������
				Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
				if (manager != null) {
					// �����������
					m_items[pos]->detachToOverlay(m_items[pos]->m_overlayInfo);
					// � ������� �������
					manager->destroy(m_items[pos]->m_overlayInfo);
				}
				m_items[pos]->m_overlayInfo = 0;
				m_items[pos]->m_layerInfo = null;
			}
		}

		inline LayerItemInfoPtr findItem(int _x, int _y)
		{
			for (size_t i=0; i<m_items.size(); i++) {
				LayerItemInfoPtr item = m_items[i]->findItem(_x, _y);
				if (item != 0) return item;
			}
			return 0;
		}

		bool addItem(LayerItemInfoPtr _item)
		{
//			Ogre::OverlayContainer * container = _item->getItemContainer();
//			if (container == null) return false;
			// ��� ���� �� ��� ���� �� ����������
			assert(!_item->m_overlayInfo);
			// ������� ������� � ������������ � ����
			static long num=0;
			Ogre::Overlay * overlay = Ogre::OverlayManager::getSingleton().create(Ogre::StringConverter::toString(num++) + "_LayerInfo");
			overlay->show();
			_item->attachToOverlay(overlay);
//			overlay->add2D(container);
			// ��������������
			_item->m_overlayInfo = overlay;
			_item->m_layerInfo = this;
			// ���� �������� �������������, �� ����� � �������
			Ogre::ushort pos = (m_items.size() <= m_count) ? (Ogre::ushort)m_items.size() : (m_count-1);
			// ��������� � ������ ������
			m_items.push_back(_item);
			_item->m_overlayInfo->setZOrder(m_start + pos * m_height);
			return true;
		}

		inline void upItem(LayerItemInfoPtr _item)
		{
			// ���� ���� ������, �������
			if (m_count < 2) return;
			// ���������, �� �� �������
			_upItem(_item, false);
		}

		inline bool removeItem(LayerItemInfoPtr _item)
		{
			if (_item->m_overlayInfo == null) return false;
			// ����������� � ������� �������
			_item->detachToOverlay(_item->m_overlayInfo);
//			Ogre::OverlayContainer * container = _item->getItemContainer();
//			if (container) {
//				_item->m_overlayInfo->remove2D(container);
//				((PanelAlphaOverlayElement*)container)->setOverlay(0);
//			}
			// � ������� �������
			Ogre::OverlayManager::getSingleton().destroy(_item->m_overlayInfo);
			_item->m_overlayInfo = 0;
			_item->m_layerInfo = 0;
			// ���������, � �������
			_upItem(_item, true);
			return true;
		}

		inline void _upItem(LayerItemInfoPtr _item, bool _destroy)
		{
			// ���� ��� �������
			bool find = false;
			size_t count_check = m_items.size()-1;
			Ogre::ushort current = m_start;
			for (size_t pos=0; pos<count_check; pos++) {

				if ((find) || (m_items[pos] == _item)) {
					find = true;
					// �������� ��� �������� ����
					if (pos < count_check) m_items[pos] = m_items[pos+1];
					if (m_items[pos]->m_overlayInfo->getZOrder() != current) m_items[pos]->m_overlayInfo->setZOrder(current);
				}

				// ��������� ������
				if (pos < (Ogre::ushort)(m_count-1)) current += m_height;
			}

			if (_destroy) {
				// ������� ����� �������
				m_items.pop_back();
			} else {
				// � ������� ���������� ��� ��� �� �������
				m_items[count_check] = _item;
				if (_item->m_overlayInfo->getZOrder() != current) _item->m_overlayInfo->setZOrder(current);
			}
		}

	private:
		std::vector<LayerItemInfoPtr> m_items;
		Ogre::ushort m_start, m_count, m_height;
		std::string m_name;

	};

} // namespace widget
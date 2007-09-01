#pragma once

#include <Ogre.h>
#include <vector>
#include "LayerItemInfo.h"

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
			// очистить свое присутствие во всех виджетах и скрыть их
			for (size_t pos=0; pos<m_items.size(); pos++) {
				m_items[pos]->m_layerInfo = 0;
				m_items[pos]->m_overlayInfo->hide();
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

		void addItem(LayerItemInfoPtr _item)
		{
			_item->m_overlayInfo->show();
			_item->m_layerInfo = this;
			// если достигли максимального, то лепим в верхний
			Ogre::ushort pos = (m_items.size() <= m_count) ? (Ogre::ushort)m_items.size() : (m_count-1);
			// добавляем и ставим высоту
			m_items.push_back(_item);
			_item->m_overlayInfo->setZOrder(m_start + pos * m_height);
		}

		inline void upItem(LayerItemInfoPtr _item)
		{
			// один слой только, выходим
			if (m_count < 2) return;
			// поднимаем, но не удаляем
			_upItem(_item, false);
		}

		inline void removeItem(LayerItemInfoPtr _item)
		{
			_item->m_overlayInfo->hide();
			_item->m_layerInfo = 0;
			// все украдено до нас
			if (m_items.empty()) return;
			// поднимаем, и удаляем
			_upItem(_item, true);
		}

		void _upItem(LayerItemInfoPtr _item, bool _destroy)
		{
			// ищем наш элемент
			bool find = false;
			size_t count_check = m_items.size()-1;
			Ogre::ushort current = m_start;
			for (size_t pos=0; pos<count_check; pos++) {

				if ((find) || (m_items[pos] == _item)) {
					find = true;
					// сдвигаем все элементы вниз
					if (pos < count_check) m_items[pos] = m_items[pos+1];
					if (m_items[pos]->m_overlayInfo->getZOrder() != current) m_items[pos]->m_overlayInfo->setZOrder(current);
				}

				// следующая высота
				if (pos < (Ogre::ushort)(m_count-1)) current += m_height;
			}

			if (_destroy) {
				// удаляем самый верхний
				m_items.pop_back();
			} else {
				// в верхний записываем тот что мы затерли
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
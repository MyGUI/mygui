/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_LayerManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

	const std::string XML_TYPE("Layer");

	INSTANCE_IMPLEMENT(LayerManager);

	void LayerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().registerUnlinker(this);
		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LayerManager::_load);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LayerManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		WidgetManager::getInstance().unregisterUnlinker(this);
		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);
		clear();

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool LayerManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void LayerManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator layer = _node->getNodeIterator();
		while (layer.nextNode(XML_TYPE)) {

			std::string name, tmp;
			Ogre::ushort start = 0, count = 1, height = 1;

			if ( false == layer->findAttribute("name", name)) {
				MYGUI_LOG(Warning, "Attribute 'name' not found {file : " << _file << "}");
				continue;
			}

			if (layer->findAttribute("height", tmp)) height = utility::parseUShort(tmp);
			else {
				MYGUI_LOG(Warning, "Attribute 'Height' not found {file : '" << _file << "' , name : " << name << "}");
			}

			if (layer->findAttribute("count", tmp)) count = utility::parseUShort(tmp);
			else {
				MYGUI_LOG(Warning, "Attribute 'Count' not found {file : '" << _file << "' , name : " << name << "}");
			}

			if (layer->findAttribute("start", tmp)) start = utility::parseUShort(tmp);
			else {
				MYGUI_LOG(Warning, "Attribute 'Start' not found {file : '" << _file << "' , name : " << name << "}");
			}

			// а вот теперь добавляем слой
			mMapLayer[name] = new LayerInfo(name, start, count, height);
		};

	}

	void LayerManager::clear()
	{
		for (MapLayer::iterator iter=mMapLayer.begin();iter!=mMapLayer.end(); iter++) delete iter->second;
		mMapLayer.clear();
	}

	void LayerManager::attachItem(LayerItemInfoPtr _item, const std::string & _layer, bool _attachToSearch)
	{
		// это наш уровень
		LayerInfoPtr layer = mMapLayer[_layer];
		MYGUI_ASSERT(null != layer, "layer '" << _layer << "' not found");
		// запоминаем созданный айтем в виджете
		layer->addItem(_item);
		// добавляем уровень в карту поиска
		if (_attachToSearch) {
			MapLayerSearch::iterator iter = mMapLayerSearch.find(layer->mStart);
			// если не нашли такого то добавляем
			if (iter == mMapLayerSearch.end()) mMapLayerSearch[layer->mStart] = layer;
		}
	}

	void LayerManager::detachItem(LayerItemInfoPtr _item)
	{
		// удаляем айтем если он приаттачен
		if (_item->mLayerInfo) _item->mLayerInfo->removeItem(_item);
	}

	void LayerManager::upItem(LayerItemInfoPtr _item)
	{
		// берем итем уровня и поднимаем
		if (_item->mLayerInfo) _item->mLayerInfo->upItem(_item);
	}

	LayerItemInfoPtr LayerManager::findWidgetItem(int _left, int _top, LayerItemInfoPtr & _rootItem)
	{
		MapLayerSearch::reverse_iterator iter = mMapLayerSearch.rbegin();
		while (iter != mMapLayerSearch.rend()) {
			LayerItemInfoPtr item = iter->second->findItem(_left, _top, _rootItem);
			if (item != null) return item;
			iter++;
		}
		return null;
	}

	void LayerManager::_unlinkWidget(WidgetPtr _widget)
	{
		detachItem(_widget);
	}

} // namespace MyGUI

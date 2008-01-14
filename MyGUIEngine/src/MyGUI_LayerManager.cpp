/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_LayerManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{

	const std::string XML_TYPE("Layer");

	INSTANCE_IMPLEMENT(LayerManager);

	void LayerManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, "initialise already");
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

	bool LayerManager::load(const std::string & _file, bool _resource)
	{

		xml::xmlDocument doc;
		std::string file = (_resource ? helper::getResourcePath(_file) : _file).c_str();
		if (file.empty()) {
			MYGUI_LOG(Error, INSTANCE_TYPE_NAME << " : " << _file << " not found");
			return false;
		}
		if (false == doc.open(file)) {
			MYGUI_LOG(Error, INSTANCE_TYPE_NAME << " : " << doc.getLastError());
			return false;
		}

		xml::xmlNodePtr root = doc.getRoot();
		if ( (null == root) || (root->getName() != "MyGUI") ) {
			MYGUI_LOG(Error, INSTANCE_TYPE_NAME << " : " << _file << " root tag 'MyGUI' not found");
			return false;
		}

		std::string type;
		if ( (false == root->findAttribute("type", type)) || (type != XML_TYPE) ) {
			MYGUI_LOG(Error, INSTANCE_TYPE_NAME << " : " << _file << " root type " << XML_TYPE << "not found");
			return false;
		}

		_load(root, file);

		return true;
	}

	void LayerManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator layer = _node->getNodeIterator();
		while (layer.nextNode(XML_TYPE)) {

			std::string name, tmp;
			Ogre::ushort start = 0, count = 1, height = 1;

			if ( false == layer->findAttribute("name", name)) {
				MYGUI_LOG(Warning, "Attribute 'name' not find {file : " << _file << "}");
				continue;
			}

			if (layer->findAttribute("height", tmp)) height = util::parseUShort(tmp);
			else {
				MYGUI_LOG(Warning, "Attribute 'Height' not find {file : '" << _file << "' , name : " << name << "}");
			}

			if (layer->findAttribute("count", tmp)) count = util::parseUShort(tmp);
			else {
				MYGUI_LOG(Warning, "Attribute 'Count' not find {file : '" << _file << "' , name : " << name << "}");
			}

			if (layer->findAttribute("start", tmp)) start = util::parseUShort(tmp);
			else {
				MYGUI_LOG(Warning, "Attribute 'Start' not find {file : '" << _file << "' , name : " << name << "}");
			}

			// � ��� ������ ��������� ����
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
		// ��� ��� �������
		LayerInfoPtr layer = mMapLayer[_layer];
		MYGUI_ASSERT(null != layer, "leyer '" << _layer << "' is not find");
		// ���������� ��������� ����� � �������
		layer->addItem(_item);
		// ��������� ������� � ����� ������
		if (_attachToSearch) {
			MapLayerSearch::iterator iter = mMapLayerSearch.find(layer->mStart);
			// ���� �� ����� ������ �� ���������
			if (iter == mMapLayerSearch.end()) mMapLayerSearch[layer->mStart] = layer;
		}
	}

	void LayerManager::detachItem(LayerItemInfoPtr _item)
	{
		// ������� ����� ���� �� ����������
		if (_item->mLayerInfo) _item->mLayerInfo->removeItem(_item);
	}

	void LayerManager::upItem(LayerItemInfoPtr _item)
	{
		// ����� ���� ������ � ���������
		if (_item->mLayerInfo) _item->mLayerInfo->upItem(_item);
	}

	LayerItemInfoPtr LayerManager::findWidgetItem(int _left, int _top, LayerItemInfoPtr & _rootItem)
	{
		MapLayerSearch::reverse_iterator iter = mMapLayerSearch.rbegin();
		while (iter != mMapLayerSearch.rend()) {
			LayerItemInfoPtr item = iter->second->findItem(_left, _top, _rootItem);
			if (item != null) {
				if ( ! item->isWidget() ) {
					_rootItem = null;
					return null;
				}
				return item;
			}
			iter++;
		}
		return null;
	}

	void LayerManager::_unlinkWidget(WidgetPtr _widget)
	{
		detachItem(_widget);
	}

} // namespace MyGUI

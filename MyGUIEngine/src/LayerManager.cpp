
#include "LayerManager.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(LayerManager);

	bool LayerManager::load(const std::string & _file)
	{
		clear();

		xml::xmlDocument doc;
		if (!doc.open(helper::getResourcePath(_file))) MYGUI_EXCEPT(0, doc.getLastError(), "");

		xml::xmlNodePtr xml_root = doc.getRoot();
		if ( (xml_root == 0) || (xml_root->getName() != "MyGUI_LayerInfo") ) return false;


		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator layer = xml_root->getNodeIterator();
		while (layer.nextNode("Layer")) {

			std::string name, tmp;
			Ogre::ushort start = 0, count = 1, height = 1;

			if ( false == layer->findAttribute("Name", name)) {
				LOG("Attribute 'Name' not find {file : ", _file, "}");
				continue;
			}

			if (layer->findAttribute("Height", tmp)) height = util::parseUShort(tmp);
			else LOG("Attribute 'Height' not find {file : '", _file, "' , Name : ", name, "}");

			if (layer->findAttribute("Count", tmp)) count = util::parseUShort(tmp);
			else LOG("Attribute 'Count' not find {file : '", _file, "' , Name : ", name, "}");

			if (layer->findAttribute("Start", tmp)) start = util::parseUShort(tmp);
			else LOG("Attribute 'Start' not find {file : '", _file, "' , Name : ", name, "}");

			// � ��� ������ ��������� ����
			mMapLayer[name] = new LayerInfo(name, start, count, height);

		};

		return true;
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
		if (layer == null) MYGUI_EXCEPT(0, _layer + " - no find level", "LayerManager::attachItem");
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

} // namespace MyGUI
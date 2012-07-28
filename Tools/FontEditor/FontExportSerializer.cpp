/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "FontExportSerializer.h"
#include "FactoryManager.h"
#include "DataManager.h"
#include "DataTypeManager.h"
#include "PropertyUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(FontExportSerializer)

	FontExportSerializer::FontExportSerializer()
	{
	}

	FontExportSerializer::~FontExportSerializer()
	{
	}

	void FontExportSerializer::serialization(pugi::xml_document& _doc)
	{
		pugi::xml_node root = _doc.append_child("MyGUI");
		root.append_attribute("type").set_value("Resource");
		root.append_attribute("version").set_value("1.1");

		DataPtr data = DataManager::getInstance().getRoot();
		for (Data::VectorData::const_iterator child = data->getChilds().begin(); child != data->getChilds().end(); child ++)
			writeFont(root, (*child));
	}

	bool FontExportSerializer::deserialization(pugi::xml_document& _doc)
	{
		if (_doc.select_single_node("MyGUI[@type=\"Resource\"]").node().empty())
			return false;

		pugi::xpath_node_set nodes = _doc.select_nodes("MyGUI/Resource[@type=\"ResourceTrueTypeFont\"]");
		for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			parseFont((*node).node());

		return true;
	}

	void FontExportSerializer::parseFont(pugi::xml_node _node)
	{
		DataPtr data = Data::CreateInstance();
		data->setType(DataTypeManager::getInstance().getType("Font"));
		data->setPropertyValue("Name", _node.attribute("name").value());

		std::string value = _node.select_single_node("Property[@key=\"Source\"]/@value").attribute().value();
		data->setPropertyValue("Source", value);

		value = _node.select_single_node("Property[@key=\"Size\"]/@value").attribute().value();
		data->setPropertyValue("Size", value);

		value = _node.select_single_node("Property[@key=\"Hinting\"]/@value").attribute().value();
		data->setPropertyValue("Hinting", value);

		DataManager::getInstance().getRoot()->addChild(data);
	}

	void FontExportSerializer::writeFont(pugi::xml_node _parent, DataPtr _data)
	{
		pugi::xml_node node = _parent.append_child("Resource");
		node.append_attribute("type").set_value("ResourceTrueTypeFont");
		node.append_attribute("name").set_value(_data->getPropertyValue("Name").c_str());

		pugi::xml_node nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("Source");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("Source").c_str());

		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("Size");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("Size").c_str());

		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("Hinting");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("Hinting").c_str());
	}

}

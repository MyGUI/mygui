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
		data->setPropertyValue("Size", MyGUI::utility::parseValue<int>(value));

		value = _node.select_single_node("Property[@key=\"Hinting\"]/@value").attribute().value();
		if (value.empty())
			value = "use_native";
		data->setPropertyValue("Hinting", value);

		value = _node.select_single_node("Property[@key=\"Resolution\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("Resolution", MyGUI::utility::parseValue<int>(value));

		value = _node.select_single_node("Property[@key=\"Antialias\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("Antialias", MyGUI::utility::parseValue<bool>(value));

		value = _node.select_single_node("Property[@key=\"TabWidth\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("TabWidth", MyGUI::utility::parseValue<int>(value));

		value = _node.select_single_node("Property[@key=\"OffsetHeight\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("OffsetHeight", MyGUI::utility::parseValue<int>(value));

		value = _node.select_single_node("Property[@key=\"SubstituteCode\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("SubstituteCode", MyGUI::utility::parseValue<int>(value));

		value = "";
		pugi::xpath_node_set codes = _node.select_nodes("Codes/Code/@range");
		for (pugi::xpath_node_set::const_iterator code = codes.begin(); code != codes.end(); code ++)
		{
			if (!value.empty())
				value += "|";

			std::vector<std::string> values = MyGUI::utility::split((*code).attribute().value());
			if (values.size() == 1)
				value += MyGUI::utility::toString(values[0], " ", values[0]);
			else if (values.size() == 2)
				value += MyGUI::utility::toString(values[0], " ", values[1]);
		}
		data->setPropertyValue("FontCodeRanges", value);

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

		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("Resolution");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("Resolution").c_str());

		std::string value = MyGUI::utility::toString(MyGUI::utility::parseValue<bool>(_data->getPropertyValue("Antialias")));
		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("Antialias");
		nodeProperty.append_attribute("value").set_value(value.c_str());

		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("TabWidth");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("TabWidth").c_str());

		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("OffsetHeight");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("OffsetHeight").c_str());

		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("SubstituteCode");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("SubstituteCode").c_str());

		pugi::xml_node nodeCodes = node.append_child("Codes");
		value = _data->getPropertyValue("FontCodeRanges");
		std::vector<std::string> values = MyGUI::utility::split(value, "|");
		for (size_t index = 0; index < values.size(); index ++)
			nodeCodes.append_child("Code").append_attribute("range").set_value(values[index].c_str());
	}

}

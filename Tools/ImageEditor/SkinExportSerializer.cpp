/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SkinExportSerializer.h"
#include "FactoryManager.h"
#include "DataManager.h"
#include "DataTypeManager.h"
#include "PropertyUtility.h"
#include "SkinDataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SkinExportSerializer)

	SkinExportSerializer::SkinExportSerializer()
	{
		#define FILL_EXPORT_NAME(value1, value2) \
			mEditorToExportNames[value1] = value2; \
			mExportToEditorNames[value2] = value1;

		FILL_EXPORT_NAME("Disabled", "disabled")
		FILL_EXPORT_NAME("Normal", "normal")
		FILL_EXPORT_NAME("Over", "highlighted")
		FILL_EXPORT_NAME("Pressed", "pushed")
		FILL_EXPORT_NAME("Selected Disabled", "disabled_checked")
		FILL_EXPORT_NAME("Selected Normal", "normal_checked")
		FILL_EXPORT_NAME("Selected Over", "highlighted_checked")
		FILL_EXPORT_NAME("Selected Pressed", "pushed_checked")

		#undef FILL_EXPORT_NAME
	}

	SkinExportSerializer::~SkinExportSerializer()
	{
	}

	void SkinExportSerializer::serialization(pugi::xml_document& _doc)
	{
		pugi::xml_node root = _doc.append_child("MyGUI");
		root.append_attribute("type").set_value("Resource");
		root.append_attribute("version").set_value("1.1");

		Data* data = DataManager::getInstance().getRoot();
		for (Data::VectorData::const_iterator child = data->getChilds().begin(); child != data->getChilds().end(); child ++)
			writeSkin(root, (*child));
	}

	bool SkinExportSerializer::deserialization(pugi::xml_document& _doc)
	{
		if (_doc.select_single_node("MyGUI[@type=\"Resource\"]").node().empty())
			return false;

		pugi::xpath_node_set nodes = _doc.select_nodes("MyGUI/Resource[@type=\"ResourceSkin\"]");
		for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			parseSkin((*node).node());

		//updateImageProperty(DataManager::getInstance().getRoot());
		return true;
	}

	void SkinExportSerializer::parseSkin(pugi::xml_node _node)
	{
		Data* data = new Data();
		data->setType(DataTypeManager::getInstance().getType("Skin"));
		data->setPropertyValue("Name", _node.attribute("name").value());
		data->setPropertyValue("Texture", _node.attribute("texture").value());

		DataManager::getInstance().getRoot()->addChild(data);

		SkinDataUtility::CreateSkinData(data);
		FillStateData(data, _node);

		std::string value = GetStateValue(data, "Normal", "Point");
		MyGUI::IntPoint point = MyGUI::IntPoint::parse(value);
		MyGUI::IntSize size = MyGUI::IntSize::parse(_node.attribute("size").value());

		data->setPropertyValue("Size", MyGUI::IntCoord(point.left, point.top, size.width, size.height).print());

		/*pugi::xpath_node_set nodes = _node.select_nodes("Group");
		for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			parseGroup((*node).node(), data);*/
	}

	/*void SkinExportSerializer::parseGroup(pugi::xml_node _node, Data* _parent)
	{
		Data* data = new Data();
		data->setType(DataTypeManager::getInstance().getType("Group"));
		std::string value = _node.attribute("name").value();
		if (value.empty())
			value = "unnamed";
		data->setPropertyValue("Name", value);
		data->setPropertyValue("Texture", _node.attribute("texture").value());
		data->setPropertyValue("Size", _node.attribute("size").value());

		_parent->addChild(data);

		pugi::xpath_node_set nodes = _node.select_nodes("Index");
		for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			parseIndex((*node).node(), data);
	}

	void SkinExportSerializer::parseIndex(pugi::xml_node _node, Data* _parent)
	{
		Data* data = new Data();
		data->setType(DataTypeManager::getInstance().getType("Index"));
		std::string value = _node.attribute("name").value();
		if (value.empty())
			value = "unnamed";
		data->setPropertyValue("Name", value);
		data->setPropertyValue("Rate", _node.attribute("rate").value());

		_parent->addChild(data);

		pugi::xpath_node_set nodes = _node.select_nodes("Frame");
		for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			parseFrame((*node).node(), data);
	}

	void SkinExportSerializer::parseFrame(pugi::xml_node _node, Data* _parent)
	{
		Data* data = new Data();
		data->setType(DataTypeManager::getInstance().getType("Frame"));
		data->setPropertyValue("Point", _node.attribute("point").value());
		std::string value = _node.attribute("count").value();
		if (value.empty())
			value = "1";
		data->setPropertyValue("Count", value);

		_parent->addChild(data);
	}*/

	void SkinExportSerializer::writeSkin(pugi::xml_node _parent, Data* _data)
	{
		pugi::xml_node node = _parent.append_child("Resource");
		node.append_attribute("type").set_value("ResourceSkin");
		node.append_attribute("name").set_value(_data->getPropertyValue("Name").c_str());
		node.append_attribute("texture").set_value(_data->getPropertyValue("Texture").c_str());
		node.append_attribute("size").set_value(MyGUI::IntCoord::parse(_data->getPropertyValue("Size")).size().print().c_str());

		/*for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
			writeGroup(node, (*child));*/
	}

	/*void SkinExportSerializer::writeGroup(pugi::xml_node _parent, Data* _data)
	{
		pugi::xml_node node = _parent.append_child("Group");
		node.append_attribute("name").set_value(_data->getPropertyValue("Name").c_str());
		node.append_attribute("texture").set_value(_data->getPropertyValue("Texture").c_str());
		node.append_attribute("size").set_value(MyGUI::IntCoord::parse(_data->getPropertyValue("Size")).size().print().c_str());

		for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
			writeIndex(node, (*child));
	}

	void SkinExportSerializer::writeIndex(pugi::xml_node _parent, Data* _data)
	{
		pugi::xml_node node = _parent.append_child("Index");
		node.append_attribute("name").set_value(_data->getPropertyValue("Name").c_str());

		std::string value = _data->getPropertyValue("Rate");
		if (!value.empty())
			node.append_attribute("rate").set_value(value.c_str());

		for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
			writeFrame(node, (*child));
	}
	
	void SkinExportSerializer::writeFrame(pugi::xml_node _parent, Data* _data)
	{
		pugi::xml_node node = _parent.append_child("Frame");
		node.append_attribute("point").set_value(_data->getPropertyValue("Point").c_str());

		size_t count = MyGUI::utility::parseValue<size_t>(_data->getPropertyValue("Count"));
		if (count > 1)
			node.append_attribute("count").set_value(MyGUI::utility::toString(count).c_str());
	}

	void SkinExportSerializer::updateImageProperty(Data* _data)
	{
		const Data::VectorData& childs = _data->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			bool unique = PropertyUtility::isUniqueName((*child), "Name");
			(*child)->setPropertyValue("UniqueName", unique);
			updateGroupProperty(*child);
		}
	}

	void SkinExportSerializer::updateGroupProperty(Data* _data)
	{
		const Data::VectorData& childs = _data->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			bool unique = PropertyUtility::isUniqueName((*child), "Name");
			(*child)->setPropertyValue("UniqueName", unique);
			updateIndexProperty(*child);
		}
	}

	void SkinExportSerializer::updateIndexProperty(Data* _data)
	{
		const Data::VectorData& childs = _data->getChilds();
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child++)
		{
			bool unique = PropertyUtility::isUniqueName((*child), "Name");
			(*child)->setPropertyValue("UniqueName", unique);
		}

		MyGUI::IntPoint point = getFirstFramePoint(_data);
		MyGUI::IntSize size = _data->getPropertyValue<MyGUI::IntSize>("Size");
		MyGUI::IntCoord coord(point, size);
		_data->setPropertyValue("Size", coord);
	}

	MyGUI::IntPoint SkinExportSerializer::getFirstFramePoint(Data* _data)
	{
		if (_data->getType()->getName() != "Group")
			return MyGUI::IntPoint();

		if (_data->getChilds().size() != 0)
		{
			Data* child = _data->getChildByIndex(0);
			if (child->getChilds().size() != 0)
			{
				return child->getChildByIndex(0)->getPropertyValue<MyGUI::IntPoint>("Point");
			}
		}


		return MyGUI::IntPoint();
	}*/

	void SkinExportSerializer::FillStateData(Data* _data, pugi::xml_node _node)
	{
		typedef std::map<std::string, MyGUI::IntPoint> MapPoint;
		MapPoint values;

		pugi::xpath_node_set states = _node.select_nodes("BasisSkin/State");
		for (pugi::xpath_node_set::const_iterator state = states.begin(); state != states.end(); state ++)
		{
			pugi::xml_attribute attribute = (*state).node().attribute("offset");
			if (!attribute.empty())
			{
				MyGUI::IntCoord coord = MyGUI::IntCoord::parse(attribute.value());
				std::string name = (*state).node().attribute("name").value();
				MapPoint::iterator valuesIterator = values.find(name);
				if (valuesIterator != values.end())
				{
					(*valuesIterator).second = MyGUI::IntPoint(
						(std::min)((*valuesIterator).second.left, coord.left),
						(std::min)((*valuesIterator).second.top, coord.top));
				}
				else
				{
					values[name] = coord.point();
				}
			}
		}

		for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
		{
			Data* childData = (*child);
			MapPoint::iterator result = values.find(convertEditorToExportStateName(childData->getPropertyValue("Name")));
			if (result != values.end())
			{
				childData->setPropertyValue("Visible", "True");
				childData->setPropertyValue("Point", (*result).second);
			}
		}
	}

	std::string SkinExportSerializer::convertEditorToExportStateName(const std::string& _value)
	{
		return mEditorToExportNames.find(_value)->second;
	}

	std::string SkinExportSerializer::convertExportToEditorStateName(const std::string& _value)
	{
		return mExportToEditorNames.find(_value)->second;
	}

	std::string SkinExportSerializer::GetStateValue(Data* _data, const std::string& _name, const std::string& _propertyName)
	{
		for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
		{
			if ((*child)->getPropertyValue("Name") == _name)
				return (*child)->getPropertyValue(_propertyName);
		}

		return "";
	}

}

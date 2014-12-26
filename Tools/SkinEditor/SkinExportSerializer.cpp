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
#include "DataUtility.h"

namespace tools
{

	FACTORY_ITEM_ATTRIBUTE(SkinExportSerializer)

	SkinExportSerializer::SkinExportSerializer()
	{
		registerMapName("Disabled", "disabled");
		registerMapName("Normal", "normal");
		registerMapName("Over", "highlighted");
		registerMapName("Pressed", "pushed");
		registerMapName("Selected Disabled", "disabled_checked");
		registerMapName("Selected Normal", "normal_checked");
		registerMapName("Selected Over", "highlighted_checked");
		registerMapName("Selected Pressed", "pushed_checked");
	}

	SkinExportSerializer::~SkinExportSerializer()
	{
	}

	void SkinExportSerializer::serialization(pugi::xml_document& _doc)
	{
		pugi::xml_node root = _doc.append_child("MyGUI");
		root.append_attribute("type").set_value("Resource");
		root.append_attribute("version").set_value("1.1");

		DataPtr data = DataManager::getInstance().getRoot();
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

		return true;
	}

	void SkinExportSerializer::parseSkin(pugi::xml_node _node)
	{
		DataPtr data = Data::CreateInstance();
		data->setType(DataTypeManager::getInstance().getType("Skin"));
		data->setPropertyValue("Name", _node.attribute("name").value());
		data->setPropertyValue("Texture", _node.attribute("texture").value());

		DataManager::getInstance().getRoot()->addChild(data);

		SkinDataUtility::CreateSkinData(data);
		fillStateData(data, _node);

		DataPtr state = getChildData(data, "State", "Normal");
		std::string value = state != nullptr ? state->getPropertyValue("Point") : "";
		MyGUI::IntPoint point = MyGUI::IntPoint::parse(value);
		MyGUI::IntSize size = MyGUI::IntSize::parse(_node.attribute("size").value());

		data->setPropertyValue("Size", MyGUI::IntCoord(point.left, point.top, size.width, size.height).print());

		fillSeparatorData(data, _node);

		MyGUI::IntRect separators = SkinDataUtility::getSeparatorsOffset(data);
		SkinDataUtility::VectorCoord coords = SkinDataUtility::getRegions(size, separators);
		SkinDataUtility::fillRegionCoords(data, coords);

		SkinDataUtility::RectVisible visible = SkinDataUtility::getSeparatorsVisible(data);
		SkinDataUtility::fillRegionEnable(data, visible);

		fillRegionData(data, _node);
	}

	void SkinExportSerializer::writeSkin(pugi::xml_node _parent, DataPtr _data)
	{
		pugi::xml_node node = _parent.append_child("Resource");
		node.append_attribute("type").set_value("ResourceSkin");
		node.append_attribute("name").set_value(_data->getPropertyValue("Name").c_str());
		node.append_attribute("size").set_value(MyGUI::IntCoord::parse(_data->getPropertyValue("Size")).size().print().c_str());
		std::string textureName = _data->getPropertyValue("Texture");
		if (!textureName.empty())
			node.append_attribute("texture").set_value(textureName.c_str());

		Data::VectorData childs = DataUtility::getChildsByType(_data, "Region", false);
		sortByAlign(childs);
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child ++)
			writeRegion(node, _data, (*child), false);

		childs = DataUtility::getChildsByType(_data, "RegionText", false);
		for (Data::VectorData::const_iterator child = childs.begin(); child != childs.end(); child ++)
			writeRegion(node, _data, (*child), true);
	}

	void SkinExportSerializer::writeRegion(pugi::xml_node _parent, DataPtr _parentData, DataPtr _data, bool _text)
	{
		bool isVisible = MyGUI::utility::parseValue<bool>(_data->getPropertyValue("Visible")) &&
			MyGUI::utility::parseValue<bool>(_data->getPropertyValue("Enable"));

		if (!isVisible)
			return;

		MyGUI::IntCoord coord = MyGUI::IntCoord::parse(_data->getPropertyValue("Coord"));

		std::string type = _data->getPropertyValue("Type");
		bool tileVert = true;
		bool tileHorz = true;

		if (type == "TileRect Vert")
		{
			type = "TileRect";
			tileHorz = false;
		}
		else if (type == "TileRect Horz")
		{
			type = "TileRect";
			tileVert = false;
		}

		pugi::xml_node node = _parent.append_child("BasisSkin");
		node.append_attribute("type").set_value(type.c_str());
		node.append_attribute("offset").set_value(coord.print().c_str());
		node.append_attribute("align").set_value(convertEditorToExportAlign(_data->getPropertyValue("Name")).c_str());

		for (Data::VectorData::const_iterator child = _parentData->getChilds().begin(); child != _parentData->getChilds().end(); child ++)
		{
			if ((*child)->getType()->getName() != "State")
				continue;

			bool visible = MyGUI::utility::parseValue<bool>((*child)->getPropertyValue("Visible"));
			if (!visible)
				continue;

			if (_text)
			{
				writeStateText(node, (*child), coord);
			}
			else
			{
				pugi::xml_node stateNode = writeState(node, (*child), coord);
				if (type == "TileRect")
				{
					pugi::xml_node propertyNode = stateNode.append_child("Property");
					propertyNode.append_attribute("key").set_value("TileSize");
					propertyNode.append_attribute("value").set_value(coord.size().print().c_str());

					propertyNode = stateNode.append_child("Property");
					propertyNode.append_attribute("key").set_value("TileH");
					propertyNode.append_attribute("value").set_value(MyGUI::utility::toString(tileHorz).c_str());

					propertyNode = stateNode.append_child("Property");
					propertyNode.append_attribute("key").set_value("TileV");
					propertyNode.append_attribute("value").set_value(MyGUI::utility::toString(tileVert).c_str());
				}
			}
		}
	}

	pugi::xml_node SkinExportSerializer::writeState(pugi::xml_node _parent, DataPtr _data, const MyGUI::IntCoord& _value)
	{
		MyGUI::IntPoint point = MyGUI::IntPoint::parse(_data->getPropertyValue("Point"));
		MyGUI::IntCoord coord = _value + point;

		pugi::xml_node node = _parent.append_child("State");
		node.append_attribute("name").set_value(convertEditorToExportStateName(_data->getPropertyValue("Name")).c_str());
		node.append_attribute("offset").set_value(coord.print().c_str());

		return node;
	}

	void SkinExportSerializer::writeStateText(pugi::xml_node _parent, DataPtr _data, const MyGUI::IntCoord& _value)
	{
		pugi::xml_node node = _parent.append_child("State");
		node.append_attribute("name").set_value(convertEditorToExportStateName(_data->getPropertyValue("Name")).c_str());
		node.append_attribute("colour").set_value(_data->getPropertyValue("TextColour").c_str());
		node.append_attribute("shift").set_value(_data->getPropertyValue("TextShift").c_str());
	}

	void SkinExportSerializer::fillStateData(DataPtr _data, pugi::xml_node _node)
	{
		typedef std::map<std::string, MyGUI::IntPoint> MapPoint;
		MapPoint values;

		pugi::xpath_node_set states = _node.select_nodes("BasisSkin/State");
		for (pugi::xpath_node_set::const_iterator state = states.begin(); state != states.end(); state ++)
		{
			MyGUI::IntCoord coord((std::numeric_limits<int>::max)(), (std::numeric_limits<int>::max)(), 0, 0);

			pugi::xml_attribute attribute = (*state).node().attribute("offset");
			if (!attribute.empty())
				coord = MyGUI::IntCoord::parse(attribute.value());

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

            // create, if there is no data
			name = convertExportToEditorStateName(name);
			DataPtr childData = getChildData(_data, "State", name);
			if (childData == nullptr)
			{
                childData = Data::CreateInstance();
				childData->setType(DataTypeManager::getInstance().getType("State"));
				childData->setPropertyValue("Name", name);
				_data->addChild(childData);
			}
		}

		for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
		{
			if ((*child)->getType()->getName() != "State")
				continue;

			DataPtr childData = (*child);
			MapPoint::iterator result = values.find(convertEditorToExportStateName(childData->getPropertyValue("Name")));
			if (result != values.end())
			{
				childData->setPropertyValue("Visible", "True");
				if ((*result).second.left != (std::numeric_limits<int>::max)() &&
					(*result).second.top != (std::numeric_limits<int>::max)())
					childData->setPropertyValue("Point", (*result).second);
			}
		}

		states = _node.select_nodes("BasisSkin/State[@colour]");
		for (pugi::xpath_node_set::const_iterator state = states.begin(); state != states.end(); state ++)
		{
			std::string name = (*state).node().attribute("name").value();
			int textShift = MyGUI::utility::parseValue<int>((*state).node().attribute("shift").value());
			MyGUI::Colour textColour = MyGUI::utility::parseValue<MyGUI::Colour>((*state).node().attribute("colour").value());

			for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
			{
				if ((*child)->getType()->getName() != "State")
					continue;

				DataPtr childData = (*child);
				if (convertEditorToExportStateName(childData->getPropertyValue("Name")) == name)
				{
					childData->setPropertyValue("TextShift", textShift);
					childData->setPropertyValue("TextColour", MyGUI::utility::toString(textColour.red, " ", textColour.green, " ", textColour.blue));
				}
			}
		}
	}

	std::string SkinExportSerializer::convertEditorToExportStateName(const std::string& _value)
	{
		MyGUI::MapString::const_iterator result = mEditorToExportNames.find(_value);
		if (result != mEditorToExportNames.end())
			return result->second;
		return _value;
	}

	std::string SkinExportSerializer::convertExportToEditorStateName(const std::string& _value)
	{
		MyGUI::MapString::const_iterator result = mExportToEditorNames.find(_value);
		if (result != mExportToEditorNames.end())
			return result->second;
		return _value;
	}

	void SkinExportSerializer::registerMapName(const std::string& _value1, const std::string& _value2)
	{
		mEditorToExportNames[_value1] = _value2;
		mExportToEditorNames[_value2] = _value1;
	}

	void SkinExportSerializer::fillSeparatorData(DataPtr _data, pugi::xml_node _node)
	{
		pugi::xpath_node_set regions = _node.select_nodes("BasisSkin[@type=\"SubSkin\"or@type=\"TileRect\"]");
		for (pugi::xpath_node_set::const_iterator region = regions.begin(); region != regions.end(); region ++)
		{
			MyGUI::IntCoord offset = MyGUI::IntCoord::parse((*region).node().attribute("offset").value());

			MyGUI::Align align = MyGUI::Align::parse((*region).node().attribute("align").value());
			if (align.isLeft())
			{
				DataPtr data = getChildData(_data, "Separator", "Left");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.width));
			}
			else if (align.isRight())
			{
				DataPtr data = getChildData(_data, "Separator", "Right");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.width));
			}
			if (align.isTop())
			{
				DataPtr data = getChildData(_data, "Separator", "Top");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.height));
			}
			else if (align.isBottom())
			{
				DataPtr data = getChildData(_data, "Separator", "Bottom");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.height));
			}
		}
	}

	DataPtr SkinExportSerializer::getChildData(DataPtr _data, const std::string& _dataType, const std::string& _name)
	{
		for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
		{
			if ((*child)->getType()->getName() == _dataType && (*child)->getPropertyValue("Name") == _name)
				return (*child);
		}

		return nullptr;
	}

	void SkinExportSerializer::fillRegionData(DataPtr _data, pugi::xml_node _node)
	{
		pugi::xpath_node_set regions = _node.select_nodes("BasisSkin[@type=\"SubSkin\"or@type=\"TileRect\"]");
		for (pugi::xpath_node_set::const_iterator region = regions.begin(); region != regions.end(); region ++)
		{
			DataPtr regionData = nullptr;

			MyGUI::Align align = MyGUI::Align::parse((*region).node().attribute("align").value());

			if (align.isLeft() && align.isTop())
				regionData = getChildData(_data, "Region", "Left Top");
			else if (align.isLeft() && align.isVStretch())
				regionData = getChildData(_data, "Region", "Left");
			else if (align.isLeft() && align.isBottom())
				regionData = getChildData(_data, "Region", "Left Bottom");

			else if (align.isHStretch() && align.isTop())
				regionData = getChildData(_data, "Region", "Top");
			else if (align.isHStretch() && align.isVStretch())
				regionData = getChildData(_data, "Region", "Center");
			else if (align.isHStretch() && align.isBottom())
				regionData = getChildData(_data, "Region", "Bottom");

			else if (align.isRight() && align.isTop())
				regionData = getChildData(_data, "Region", "Right Top");
			else if (align.isRight() && align.isVStretch())
				regionData = getChildData(_data, "Region", "Right");
			else if (align.isRight() && align.isBottom())
				regionData = getChildData(_data, "Region", "Right Bottom");

			if (regionData == nullptr)
				continue;

			regionData->setPropertyValue("Visible", "True");

			std::string type = (*region).node().attribute("type").value();
			if (type == "TileRect")
			{
				bool vert = MyGUI::utility::parseValue<bool>((*region).node().select_single_node("State/Property[@key=\"TileV\"]/@value").attribute().value());
				bool horz = MyGUI::utility::parseValue<bool>((*region).node().select_single_node("State/Property[@key=\"TileH\"]/@value").attribute().value());

				if (vert && !horz)
					type = "TileRect Vert";
				else if (!vert && horz)
					type = "TileRect Horz";
			}

			regionData->setPropertyValue("Type", type);
		}

		pugi::xpath_node regionText = _node.select_single_node("BasisSkin[@type=\"SimpleText\"or@type=\"EditText\"]");
		if (!regionText.node().empty())
		{
			DataPtr regionData = getChildData(_data, "RegionText", "Text");

			if (regionData != nullptr)
			{
				regionData->setPropertyValue("Visible", "True");

				std::string type = regionText.node().attribute("type").value();
				regionData->setPropertyValue("Type", type);

				MyGUI::IntCoord offset = MyGUI::IntCoord::parse(regionText.node().attribute("offset").value());
				regionData->setPropertyValue("Coord", offset);

				MyGUI::Align align = MyGUI::Align::parse(regionText.node().attribute("align").value());
				regionData->setPropertyValue("Align", align);
			}
		}
	}

	std::string SkinExportSerializer::convertEditorToExportAlign(const std::string& _value)
	{
		MyGUI::Align align = MyGUI::Align::parse(_value);

		if (align.isHCenter())
			align |= MyGUI::Align::HStretch;
		if (align.isVCenter())
			align |= MyGUI::Align::VStretch;

		if (align == MyGUI::Align::Stretch)
			return "Stretch";
		else if (align == MyGUI::Align::Center)
			return "Center";

		return align.print();
	}

	void SkinExportSerializer::sortByAlign(Data::VectorData& childs)
	{
		moveToEnd(childs, findIndex(childs, "Left Top"));
		moveToEnd(childs, findIndex(childs, "Top"));
		moveToEnd(childs, findIndex(childs, "Right Top"));
		moveToEnd(childs, findIndex(childs, "Right"));
		moveToEnd(childs, findIndex(childs, "Right Bottom"));
		moveToEnd(childs, findIndex(childs, "Bottom"));
		moveToEnd(childs, findIndex(childs, "Left Bottom"));
		moveToEnd(childs, findIndex(childs, "Left"));
		moveToEnd(childs, findIndex(childs, "Center"));
	}

	size_t SkinExportSerializer::findIndex(Data::VectorData& childs, const std::string& _name)
	{
		for (size_t index = 0; index < childs.size(); index ++)
		{
			if (childs[index]->getPropertyValue("Name") == _name)
				return index;
		}

		return -1;
	}

	void SkinExportSerializer::moveToEnd(Data::VectorData& childs, size_t _index)
	{
		DataPtr data = childs[_index];
		childs.erase(childs.begin() + _index);
		childs.push_back(data);
	}

}

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
		fillStateData(data, _node);

		Data* state = getChildData(data, "State", "Normal");
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

	void SkinExportSerializer::writeSkin(pugi::xml_node _parent, Data* _data)
	{
		pugi::xml_node node = _parent.append_child("Resource");
		node.append_attribute("type").set_value("ResourceSkin");
		node.append_attribute("name").set_value(_data->getPropertyValue("Name").c_str());
		node.append_attribute("texture").set_value(_data->getPropertyValue("Texture").c_str());
		node.append_attribute("size").set_value(MyGUI::IntCoord::parse(_data->getPropertyValue("Size")).size().print().c_str());
	}

	void SkinExportSerializer::fillStateData(Data* _data, pugi::xml_node _node)
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
			if ((*child)->getType()->getName() != "State")
				continue;

			Data* childData = (*child);
			MapPoint::iterator result = values.find(convertEditorToExportStateName(childData->getPropertyValue("Name")));
			if (result != values.end())
			{
				childData->setPropertyValue("Visible", "True");
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

				Data* childData = (*child);
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
		return mEditorToExportNames.find(_value)->second;
	}

	std::string SkinExportSerializer::convertExportToEditorStateName(const std::string& _value)
	{
		return mExportToEditorNames.find(_value)->second;
	}

	void SkinExportSerializer::registerMapName(const std::string& _value1, const std::string& _value2)
	{
		mEditorToExportNames[_value1] = _value2;
		mExportToEditorNames[_value2] = _value1;
	}

	void SkinExportSerializer::fillSeparatorData(Data* _data, pugi::xml_node _node)
	{
		// по дефолту там может быть что угодно
		Data* data = getChildData(_data, "Separator", "Left");
		data->setPropertyValue("Offset", "0"); 
		data = getChildData(_data, "Separator", "Right");
		data->setPropertyValue("Offset", "0");
		data = getChildData(_data, "Separator", "Top");
		data->setPropertyValue("Offset", "0");
		data = getChildData(_data, "Separator", "Bottom");
		data->setPropertyValue("Offset", "0");

		pugi::xpath_node_set regions = _node.select_nodes("BasisSkin[@type=\"SubSkin\"or@type=\"TileRect\"]");
		for (pugi::xpath_node_set::const_iterator region = regions.begin(); region != regions.end(); region ++)
		{
			MyGUI::IntCoord offset = MyGUI::IntCoord::parse((*region).node().attribute("offset").value());

			MyGUI::Align align = MyGUI::Align::parse((*region).node().attribute("align").value());
			if (align.isLeft())
			{
				Data* data = getChildData(_data, "Separator", "Left");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.width));
			}
			else if (align.isRight())
			{
				Data* data = getChildData(_data, "Separator", "Right");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.width));
			}
			if (align.isTop())
			{
				Data* data = getChildData(_data, "Separator", "Top");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.height));
			}
			else if (align.isBottom())
			{
				Data* data = getChildData(_data, "Separator", "Bottom");
				data->setPropertyValue("Visible", "True");
				data->setPropertyValue("Offset", MyGUI::utility::toString(offset.height));
			}
		}
	}

	Data* SkinExportSerializer::getChildData(Data* _data, const std::string& _dataType, const std::string& _name)
	{
		for (Data::VectorData::const_iterator child = _data->getChilds().begin(); child != _data->getChilds().end(); child ++)
		{
			if ((*child)->getType()->getName() == _dataType && (*child)->getPropertyValue("Name") == _name)
				return (*child);
		}

		return nullptr;
	}

	void SkinExportSerializer::fillRegionData(Data* _data, pugi::xml_node _node)
	{
		pugi::xpath_node_set regions = _node.select_nodes("BasisSkin[@type=\"SubSkin\"or@type=\"TileRect\"]");
		for (pugi::xpath_node_set::const_iterator region = regions.begin(); region != regions.end(); region ++)
		{
			Data* regionData = nullptr;

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
	}

}

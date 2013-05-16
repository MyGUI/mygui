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
#include <MyGUI_ResourceTrueTypeFont.h>
#include "FileSystemInfo/FileSystemInfo.h"

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
		data->setPropertyValue("Size", MyGUI::utility::parseValue<float>(value));

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
			data->setPropertyValue("TabWidth", MyGUI::utility::parseValue<float>(value));

		value = _node.select_single_node("Property[@key=\"OffsetHeight\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("OffsetHeight", MyGUI::utility::parseValue<int>(value));

		value = _node.select_single_node("Property[@key=\"SubstituteCode\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("SubstituteCode", MyGUI::utility::parseValue<int>(value));

		value = _node.select_single_node("Property[@key=\"Distance\"]/@value").attribute().value();
		if (!value.empty())
			data->setPropertyValue("Distance", MyGUI::utility::parseValue<int>(value));

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

		nodeProperty = node.append_child("Property");
		nodeProperty.append_attribute("key").set_value("Distance");
		nodeProperty.append_attribute("value").set_value(_data->getPropertyValue("Distance").c_str());

		pugi::xml_node nodeCodes = node.append_child("Codes");
		value = _data->getPropertyValue("FontCodeRanges");
		std::vector<std::string> values = MyGUI::utility::split(value, "|");
		for (size_t index = 0; index < values.size(); index ++)
			nodeCodes.append_child("Code").append_attribute("range").set_value(values[index].c_str());
	}

	bool FontExportSerializer::exportData(const MyGUI::UString& _folderName, const MyGUI::UString& _fileName)
	{
		MyGUI::xml::Document document;
		document.createDeclaration();
		MyGUI::xml::ElementPtr root = document.createRoot("MyGUI");
		root->addAttribute("type", "Resource");
		root->addAttribute("version", "1.1");

		DataPtr data = DataManager::getInstance().getRoot();
		for (Data::VectorData::const_iterator child = data->getChilds().begin(); child != data->getChilds().end(); child ++)
		{
			generateFont((*child));
			generateFontManualXml(root, _folderName, (*child));
		}

		return document.save(common::concatenatePath(_folderName, _fileName));
	}

	template<typename Type>
	void addProperty(MyGUI::xml::ElementPtr _node, const std::string& _name, Type _value)
	{
		MyGUI::xml::ElementPtr node = _node->createChild("Property");
		node->addAttribute("key", _name);
		node->addAttribute("value", _value);
	}

	static void addCode(MyGUI::xml::Element* _node, MyGUI::Char _code, MyGUI::ResourceTrueTypeFont* _font, bool _isSubstitute)
	{
		MyGUI::xml::Element* codeNode = _node->createChild("Code");

		if (!_isSubstitute)
		{
			switch (_code)
			{
			case MyGUI::FontCodeType::Selected:
				codeNode->addAttribute("index", "selected");
				break;

			case MyGUI::FontCodeType::SelectedBack:
				codeNode->addAttribute("index", "selected_back");
				break;

			case MyGUI::FontCodeType::Cursor:
				codeNode->addAttribute("index", "cursor");
				break;

			default:
				codeNode->addAttribute("index", _code);
				break;
			}
		}
		else
		{
			codeNode->addAttribute("index", "substitute");
		}

		MyGUI::GlyphInfo* info = _font->getGlyphInfo(_code);
		MyGUI::ITexture* texture = _font->getTextureFont();
		MyGUI::FloatCoord coord(info->uvRect.left * (float)texture->getWidth(), info->uvRect.top * (float)texture->getHeight(), info->width, info->height);

		if (!coord.empty())
			codeNode->addAttribute("coord", coord);

		if (info->bearingX != 0.0f || info->bearingY != 0.0f)
			codeNode->addAttribute("bearing", MyGUI::FloatPoint(info->bearingX, info->bearingY));

		if (info->advance != info->width)
			codeNode->addAttribute("advance", info->advance);
	}

	void FontExportSerializer::generateFontManualXml(MyGUI::xml::ElementPtr _root, const MyGUI::UString& _folderName, DataPtr _data)
	{
		MyGUI::IFont* resource = MyGUI::FontManager::getInstance().getByName(_data->getPropertyValue("FontName"));
		MyGUI::ResourceTrueTypeFont* font = resource != nullptr ? resource->castType<MyGUI::ResourceTrueTypeFont>(false) : nullptr;

		if (font != nullptr)
		{
			std::string textureName = _data->getPropertyValue("Name") + ".png";
			MyGUI::ITexture* texture = font->getTextureFont();
			if (texture == nullptr)
				return;
			texture->saveToFile(MyGUI::UString(common::concatenatePath(_folderName, MyGUI::UString(textureName))).asUTF8());

			MyGUI::xml::ElementPtr node = _root->createChild("Resource");
			node->addAttribute("type", "ResourceManualFont");
			node->addAttribute("name", _data->getPropertyValue("Name"));

			addProperty(node, "Source", textureName);
			addProperty(node, "SourceSize", MyGUI::IntSize(texture->getWidth(), texture->getHeight()));
			addProperty(node, "DefaultHeight", font->getDefaultHeight());

			MyGUI::xml::Element* codes = node->createChild("Codes");

			std::vector<std::pair<MyGUI::Char, MyGUI::Char> > codePointRanges = font->getCodePointRanges();
			MyGUI::Char substituteCodePoint = font->getSubstituteCodePoint();
			bool isCustomSubstituteCodePoint = substituteCodePoint != MyGUI::FontCodeType::NotDefined;

			// Add all of the code points. Skip over the substitute code point -- unless it's been customized, in which case it
			// needs to be added here as a regular code point and then at the end as a substitute code point.
			for (std::vector<std::pair<MyGUI::Char, MyGUI::Char> >::const_iterator iter = codePointRanges.begin() ; iter != codePointRanges.end(); ++iter)
				for (MyGUI::Char code = iter->first; code <= iter->second && code >= iter->first; ++code)
					if (code != substituteCodePoint || isCustomSubstituteCodePoint)
						addCode(codes, code, font, false);

			// Always add the substitute code point last, even if it isn't the last one in the range.
			addCode(codes, substituteCodePoint, font, true);
		}
	}

	void FontExportSerializer::generateFont(DataPtr _data)
	{
		std::string fontName = _data->getPropertyValue("FontName");
		removeFont(fontName);

		std::string resourceCategory = MyGUI::ResourceManager::getInstance().getCategoryName();
		MyGUI::ResourceTrueTypeFont* font = MyGUI::FactoryManager::getInstance().createObject<MyGUI::ResourceTrueTypeFont>(resourceCategory);

		font->setResourceName(fontName);

		font->setSource(_data->getPropertyValue("Source"));
		font->setSize(_data->getPropertyValue<float>("Size"));
		font->setResolution(_data->getPropertyValue<int>("Resolution"));
		font->setHinting(_data->getPropertyValue("Hinting"));
		font->setAntialias(_data->getPropertyValue<bool>("Antialias"));
		font->setTabWidth(_data->getPropertyValue<float>("TabWidth"));
		font->setOffsetHeight(_data->getPropertyValue<int>("OffsetHeight"));
		font->setSubstituteCode(_data->getPropertyValue<int>("SubstituteCode"));
		font->setDistance(_data->getPropertyValue<int>("Distance"));

		std::string ranges = _data->getPropertyValue("FontCodeRanges");
		std::vector<std::string> values = MyGUI::utility::split(ranges, "|");
		for (size_t index = 0; index < values.size(); index ++)
		{
			MyGUI::IntSize size = MyGUI::IntSize::parse(values[index]);
			font->addCodePointRange(size.width, size.height); // о да
		}

		font->initialise();

		MyGUI::ResourceManager::getInstance().addResource(font);
	}

	void FontExportSerializer::removeFont(const std::string& _fontName)
	{
		MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
		if (manager.isExist(_fontName))
			manager.removeByName(_fontName);
	}

}

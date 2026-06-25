#include "Precompiled.h"
#include "WidgetTypes.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(WidgetTypes);

	const std::string_view DEFAULT_GROUP_NAME = "Default";

	void WidgetTypes::initialise()
	{
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Widgets") =
			MyGUI::newDelegate(this, &WidgetTypes::loadWidgets);
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Values") =
			MyGUI::newDelegate(this, &WidgetTypes::loadValues);
	}

	void WidgetTypes::shutdown()
	{
		for (auto& widgetType : mWidgetTypes)
			delete widgetType;
		mWidgetTypes.clear();

		for (auto& possibleValue : mPossibleValues)
			delete possibleValue;
		mPossibleValues.clear();
	}

	WidgetStyle* WidgetTypes::findWidgetStyle(std::string_view _type)
	{
		for (auto& widgetType : mWidgetTypes)
		{
			if (widgetType->name == _type)
			{
				return widgetType;
			}
		}
		return findWidgetStyle("Widget");
	}

	WidgetTypes::VectorString WidgetTypes::findPossibleValues(std::string_view _name)
	{
		for (auto& possibleValue : mPossibleValues)
		{
			if (possibleValue->name == _name)
			{
				return possibleValue->values;
			}
		}
		return {};
	}

	WidgetStyle* WidgetTypes::getWidgetType(std::string_view _name)
	{
		// ищем тип, если нет, то создаем
		for (auto& widgetType : mWidgetTypes)
		{
			if (widgetType->name == _name)
				return widgetType;
		}

		WidgetStyle* type = new WidgetStyle(_name);
		mWidgetTypes.push_back(type);

		return type;
	}

	void WidgetTypes::addWidgetSkinType(
		std::string_view _type,
		std::string_view _skin,
		std::string_view _group,
		std::string_view _button_name)
	{
		WidgetStyle* widget_type = getWidgetType(_type);
		if (_group.empty())
			_group = DEFAULT_GROUP_NAME;
		auto it = mSkinGroups.find(_group);
		if (it == mSkinGroups.end())
			it = mSkinGroups.emplace(_group, VectorSkinInfo()).first;
		it->second.emplace_back(_skin, widget_type->name, _button_name);
		widget_type->skin.emplace_back(_skin);
	}

	void WidgetTypes::loadWidgets(pugi::xml_node _node, std::string_view _file, MyGUI::Version _version)
	{
		for (auto widgets : _node.children("Widget"))
		{
			WidgetStyle* widget_type = getWidgetType(widgets.attribute("name").value());

			widget_type->internalType = std::string_view(widgets.attribute("internal").value()) == "true";

			// берем детей и крутимся
			for (auto field : widgets.children())
			{
				std::string key;
				std::string value;
				std::string group;

				if (std::string_view(field.name()) == "Property")
				{
					if (auto attr = field.attribute("key"))
						key = attr.value();
					else
						continue;
					if (auto attr = field.attribute("value"))
						value = attr.value();
					else
						continue;
					if (auto attr = field.attribute("group"))
						group = attr.value();
					if (key == "Skin")
					{
						std::string_view button_name = field.attribute("name").value();
						if (button_name.empty())
							button_name = value;

						if (group.empty())
							group = DEFAULT_GROUP_NAME;
						mSkinGroups[group].emplace_back(value, widget_type->name, button_name);
						widget_type->skin.push_back(value);
					}
					else if (key == "DefaultSkin")
						widget_type->default_skin = value;
					else if (key == "Parent")
						widget_type->parent = MyGUI::utility::parseBool(value);
					else if (key == "Child")
						widget_type->child = MyGUI::utility::parseBool(value);
					else if (key == "Resizeable")
						widget_type->resizeable = MyGUI::utility::parseBool(value);
					else if (key == "ItemManager")
						widget_type->many_items = MyGUI::utility::parseBool(value);
					else if (key == "Base")
						widget_type->base = value;
				}
				else if (std::string_view(field.name()) == "Parameter")
				{
					if (auto attr = field.attribute("key"))
						key = attr.value();
					else
						continue;
					if (auto attr = field.attribute("value"))
						value = attr.value();
					else
						continue;
					widget_type->parameter.emplace_back(key, value);
				}
				else if (std::string_view(field.name()) == "TemplateData")
				{
					if (auto attr = field.attribute("key"))
						key = attr.value();
					else
						continue;
					if (auto attr = field.attribute("value"))
						value = attr.value();
					else
						continue;
					widget_type->templateData.emplace_back(key, value);
				}
				else if (std::string_view(field.name()) == "ParameterData")
				{
					if (auto attr = field.attribute("key"))
						key = attr.value();
					else
						continue;
					if (auto attr = field.attribute("value"))
						value = attr.value();
					else
						continue;
					widget_type->parameterData.emplace_back(key, value);
				}
			}

			if (widget_type->base.empty() && widget_type->name != "Widget")
				widget_type->base = "Widget";
		}

		updateDepth();
	}

	PossibleValue* WidgetTypes::getPossibleValue(std::string_view _name)
	{
		for (const auto& value : mPossibleValues)
		{
			if (value->name == _name)
			{
				return value;
			}
		}

		PossibleValue* possible_value = new PossibleValue();
		possible_value->name = _name;
		mPossibleValues.push_back(possible_value);

		return possible_value;
	}

	void WidgetTypes::loadValues(pugi::xml_node _node, std::string_view _file, MyGUI::Version _version)
	{
		for (auto widgets : _node.children("Value"))
		{
			std::string_view name = widgets.attribute("name").value();
			PossibleValue* possible_value = getPossibleValue(name);

			// тип мерджа переменных
			std::string_view merge = widgets.attribute("merge").value();
			// дополняем своими данными, по дефолту
			if (merge == "add")
			{
			}
			// удаляем и добавляем свои
			else if (merge == "replace")
			{
				possible_value->values.clear();
			}

			// берем детей и крутимся
			for (auto field : widgets.children())
			{
				std::string key;

				if (std::string_view(field.name()) == "Property")
				{
					if (auto attr = field.attribute("key"))
						key = attr.value();
					else
						continue;
					possible_value->values.push_back(key);
				}
			}
		}
	}

	void WidgetTypes::clearAllSkins()
	{
		for (auto& widgetType : mWidgetTypes)
			widgetType->skin.clear();

		mSkinGroups.clear();
	}

	const SkinGroups& WidgetTypes::getSkinGroups() const
	{
		return mSkinGroups;
	}

	VectorWidgetType WidgetTypes::getWidgetTypes() const
	{
		return mWidgetTypes;
	}

	WidgetStyle* WidgetTypes::findWidgetStyleBySkin(std::string_view _skinName)
	{
		for (auto& widgetType : mWidgetTypes)
		{
			for (const auto& skin : widgetType->skin)
			{
				if (skin == _skinName)
					return widgetType;
			}
		}

		return nullptr;
	}

	void WidgetTypes::updateDepth()
	{
		for (auto& widgetType : mWidgetTypes)
			widgetType->depth = updateDepth(widgetType);
	}

	size_t WidgetTypes::updateDepth(WidgetStyle* _style)
	{
		size_t result = 0;

		while (_style != nullptr && !_style->base.empty())
		{
			_style = findWidgetStyle(_style->base);
			++result;
		}

		return result;
	}

}

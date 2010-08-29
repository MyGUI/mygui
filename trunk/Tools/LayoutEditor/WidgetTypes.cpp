#include "precompiled.h"
#include "WidgetTypes.h"

template <> tools::WidgetTypes* MyGUI::Singleton<tools::WidgetTypes>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetTypes>::mClassTypeName("WidgetTypes");

namespace tools
{
	const std::string LogSection = "LayoutEditor";

	void WidgetTypes::initialise()
	{
		//loadTypes();
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Widgets") = MyGUI::newDelegate(this, &WidgetTypes::loadWidgets);
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Values") = MyGUI::newDelegate(this, &WidgetTypes::loadValues);
	}

	void WidgetTypes::shutdown()
	{
		for (std::vector<WidgetStyle*>::iterator iter = widget_types.begin(); iter != widget_types.end(); ++iter) delete *iter;
		widget_types.clear();
		for (std::vector<PossibleValue*>::iterator iter = possible_values.begin(); iter != possible_values.end(); ++iter) delete *iter;
		possible_values.clear();
	}

	WidgetStyle* WidgetTypes::find(std::string _type)
	{
		for (std::vector<WidgetStyle*>::iterator iter = widget_types.begin(); iter != widget_types.end(); ++iter)
		{
			if ((*iter)->name == _type)
			{
				return *iter;
			}
		}
		return find("Widget");
	}

	std::vector<std::string> WidgetTypes::findPossibleValues(std::string _name)
	{
		for (std::vector<PossibleValue*>::iterator iter = possible_values.begin(); iter != possible_values.end(); ++iter)
		{
			if ((*iter)->name == _name)
			{
				return (*iter)->values;
			}
		}
		return std::vector<std::string>();
	}

	WidgetStyle * WidgetTypes::getWidgetType(const std::string& _name)
	{
		// ищем тип, если нет, то создаем
		for (VectorWidgetType::iterator iter=widget_types.begin(); iter!=widget_types.end(); ++iter)
		{
			if ((*iter)->name == _name) return (*iter);
		}

		WidgetStyle * type = new WidgetStyle(_name);
		widget_types.push_back(type);

		return type;
	}

	void WidgetTypes::addWidgetSkinType(const std::string& _type, const std::string& _skin, const std::string& _group, const std::string& _button_name)
	{
		WidgetStyle * widget_type = getWidgetType(_type);

		skin_groups[_group.empty() ? DEFAULT_GOROUP_NAME : _group].push_back(SkinInfo(_skin, widget_type->name, _button_name));
		widget_type->skin.push_back(_skin);
	}

	void WidgetTypes::loadWidgets(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator widgets = _node->getElementEnumerator();
		while (widgets.next("Widget"))
		{
			WidgetStyle * widget_type = getWidgetType(widgets->findAttribute("name"));

			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator field = widgets->getElementEnumerator();
			while (field.next())
			{
				std::string key, value, group;

				if (field->getName() == "Property")
				{
					if (!field->findAttribute("key", key)) continue;
					if (!field->findAttribute("value", value)) continue;
					field->findAttribute("group", group);
					if (key == "Skin")
					{
						std::string button_name = field->findAttribute("name");
						if (button_name.empty())
							button_name = value;

						if (group.empty()) group = DEFAULT_GOROUP_NAME;
						skin_groups[group].push_back(SkinInfo(value, widget_type->name, button_name));
						widget_type->skin.push_back(value);
					}
					else if (key == "DefaultSkin") widget_type->default_skin = value;
					else if (key == "Parent") widget_type->parent = MyGUI::utility::parseBool(value);
					else if (key == "Child") widget_type->child = MyGUI::utility::parseBool(value);
					else if (key == "Resizeable") widget_type->resizeable = MyGUI::utility::parseBool(value);
					else if (key == "ItemManager") widget_type->many_items = MyGUI::utility::parseBool(value);
					else if (key == "Base") widget_type->base = value;
				}
				else if (field->getName() == "Parameter")
				{
					if (!field->findAttribute("key", key)) continue;
					if (!field->findAttribute("value", value)) continue;
					//widget_type->parameter.insert(MyGUI::PairString(key, value));
					widget_type->parameter.push_back(MyGUI::PairString(key, value));
				}
			}

			if (widget_type->base.empty() && widget_type->name != "Widget") widget_type->base = "Widget";

		}
	}

	PossibleValue * WidgetTypes::getPossibleValue(const std::string& _name)
	{

		PossibleValue * possible_value = nullptr;
		for (std::vector<PossibleValue*>::iterator iter=possible_values.begin(); iter!=possible_values.end(); ++iter)
		{
			if ((*iter)->name == _name)
			{
				return (*iter);
			}
		}

		if (possible_value == nullptr)
		{
			possible_value = new PossibleValue();
			possible_value->name = _name;
			possible_values.push_back(possible_value);
		}

		return possible_value;
	}

	void WidgetTypes::loadValues(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator widgets = _node->getElementEnumerator();
		while (widgets.next("Value"))
		{
			std::string name = widgets->findAttribute("name");
			PossibleValue * possible_value = getPossibleValue(name);

			// тип мерджа переменных
			std::string merge = widgets->findAttribute("merge");
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
			MyGUI::xml::ElementEnumerator field = widgets->getElementEnumerator();
			while (field.next())
			{
				std::string key, value;

				if (field->getName() == "Property")
				{
					if (!field->findAttribute("key", key)) continue;
					possible_value->values.push_back(MyGUI::LanguageManager::getInstance().replaceTags(key));
				}

			}
		}

	}

	void WidgetTypes::clearAllSkins()
	{
		for (VectorWidgetType::iterator iter=widget_types.begin(); iter!=widget_types.end(); ++iter)
		{
			(*iter)->skin.clear();
		}

		skin_groups.clear();
	}

} // namespace tools

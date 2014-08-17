#include "Precompiled.h"
#include "WidgetTypes.h"

template <> tools::WidgetTypes* MyGUI::Singleton<tools::WidgetTypes>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::WidgetTypes>::mClassTypeName = "WidgetTypes";

namespace tools
{

	const std::string DEFAULT_GOROUP_NAME = "Default";
	const std::string LogSection = "LayoutEditor";

	void WidgetTypes::initialise()
	{
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Widgets") = MyGUI::newDelegate(this, &WidgetTypes::loadWidgets);
		MyGUI::ResourceManager::getInstance().registerLoadXmlDelegate("Values") = MyGUI::newDelegate(this, &WidgetTypes::loadValues);
	}

	void WidgetTypes::shutdown()
	{
		for (std::vector<WidgetStyle*>::iterator iter = mWidgetTypes.begin(); iter != mWidgetTypes.end(); ++iter)
			delete *iter;
		mWidgetTypes.clear();

		for (VectorPossibleValue::iterator iter = mPossibleValues.begin(); iter != mPossibleValues.end(); ++iter)
			delete *iter;
		mPossibleValues.clear();
	}

	WidgetStyle* WidgetTypes::findWidgetStyle(const std::string& _type)
	{
		for (std::vector<WidgetStyle*>::iterator iter = mWidgetTypes.begin(); iter != mWidgetTypes.end(); ++iter)
		{
			if ((*iter)->name == _type)
			{
				return *iter;
			}
		}
		return findWidgetStyle("Widget");
	}

	WidgetTypes::VectorString WidgetTypes::findPossibleValues(const std::string& _name)
	{
		for (VectorPossibleValue::iterator iter = mPossibleValues.begin(); iter != mPossibleValues.end(); ++iter)
		{
			if ((*iter)->name == _name)
			{
				return (*iter)->values;
			}
		}
		return VectorString();
	}

	WidgetStyle* WidgetTypes::getWidgetType(const std::string& _name)
	{
		// ищем тип, если нет, то создаем
		for (VectorWidgetType::iterator iter = mWidgetTypes.begin(); iter != mWidgetTypes.end(); ++iter)
		{
			if ((*iter)->name == _name)
				return (*iter);
		}

		WidgetStyle* type = new WidgetStyle(_name);
		mWidgetTypes.push_back(type);

		return type;
	}

	void WidgetTypes::addWidgetSkinType(const std::string& _type, const std::string& _skin, const std::string& _group, const std::string& _button_name)
	{
		WidgetStyle* widget_type = getWidgetType(_type);

		mSkinGroups[_group.empty() ? DEFAULT_GOROUP_NAME : _group].push_back(SkinInfo(_skin, widget_type->name, _button_name));
		widget_type->skin.push_back(_skin);
	}

	void WidgetTypes::loadWidgets(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator widgets = _node->getElementEnumerator();
		while (widgets.next("Widget"))
		{
			WidgetStyle* widget_type = getWidgetType(widgets->findAttribute("name"));

			widget_type->internalType = widgets->findAttribute("internal") == "true";

			// берем детей и крутимся
			MyGUI::xml::ElementEnumerator field = widgets->getElementEnumerator();
			while (field.next())
			{
				std::string key, value, group;

				if (field->getName() == "Property")
				{
					if (!field->findAttribute("key", key))
						continue;
					if (!field->findAttribute("value", value))
						continue;
					field->findAttribute("group", group);
					if (key == "Skin")
					{
						std::string button_name = field->findAttribute("name");
						if (button_name.empty())
							button_name = value;

						if (group.empty())
							group = DEFAULT_GOROUP_NAME;
						mSkinGroups[group].push_back(SkinInfo(value, widget_type->name, button_name));
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
				else if (field->getName() == "Parameter")
				{
					if (!field->findAttribute("key", key))
						continue;
					if (!field->findAttribute("value", value))
						continue;
					widget_type->parameter.push_back(MyGUI::PairString(key, value));
				}
				else if (field->getName() == "TemplateData")
				{
					if (!field->findAttribute("key", key))
						continue;
					if (!field->findAttribute("value", value))
						continue;
					widget_type->templateData.push_back(MyGUI::PairString(key, value));
				}
				else if (field->getName() == "ParameterData")
				{
					if (!field->findAttribute("key", key))
						continue;
					if (!field->findAttribute("value", value))
						continue;
					widget_type->parameterData.push_back(MyGUI::PairString(key, value));
				}
			}

			if (widget_type->base.empty() && widget_type->name != "Widget")
				widget_type->base = "Widget";
		}

		updateDepth();
	}

	PossibleValue* WidgetTypes::getPossibleValue(const std::string& _name)
	{
		PossibleValue* possible_value = nullptr;
		for (VectorPossibleValue::iterator iter = mPossibleValues.begin(); iter != mPossibleValues.end(); ++iter)
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
			mPossibleValues.push_back(possible_value);
		}

		return possible_value;
	}

	void WidgetTypes::loadValues(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version)
	{
		MyGUI::xml::ElementEnumerator widgets = _node->getElementEnumerator();
		while (widgets.next("Value"))
		{
			std::string name = widgets->findAttribute("name");
			PossibleValue* possible_value = getPossibleValue(name);

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
				std::string key;

				if (field->getName() == "Property")
				{
					if (!field->findAttribute("key", key))
						continue;
					possible_value->values.push_back(MyGUI::LanguageManager::getInstance().replaceTags(key));
				}
			}
		}
	}

	void WidgetTypes::clearAllSkins()
	{
		for (VectorWidgetType::iterator iter = mWidgetTypes.begin(); iter != mWidgetTypes.end(); ++iter)
			(*iter)->skin.clear();

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

	WidgetStyle* WidgetTypes::findWidgetStyleBySkin(const std::string& _skinName)
	{
		for (VectorWidgetType::iterator item = mWidgetTypes.begin(); item != mWidgetTypes.end(); ++item)
		{
			for (WidgetStyle::VectorString::const_iterator skin = (*item)->skin.begin(); skin != (*item)->skin.end(); ++skin)
			{
				if ((*skin) == _skinName)
					return (*item);
			}
		}

		return nullptr;
	}

	void WidgetTypes::updateDepth()
	{
		for (VectorWidgetType::iterator item = mWidgetTypes.begin(); item != mWidgetTypes.end(); ++ item)
			(*item)->depth = updateDepth(*item);
	}

	size_t WidgetTypes::updateDepth(WidgetStyle* _style)
	{
		size_t result = 0;

		while (_style != nullptr && !_style->base.empty())
		{
			_style = findWidgetStyle(_style->base);
			++ result;
		}

		return result;
	}

}

/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#include "Precompiled.h"
#include "PropertiesPanelView.h"
#include "Common.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"
#include "UndoManager.h"
#include "Parse.h"
#include "GroupMessage.h"
#include "SettingsManager.h"
#include "CommandManager.h"
#include "WidgetSelectorManager.h"

//FIXME
template <> tools::PropertiesPanelView* MyGUI::Singleton<tools::PropertiesPanelView>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::PropertiesPanelView>::mClassTypeName("PropertiesPanelView");

namespace tools
{
	#define ON_EXIT( CODE ) class _OnExit { public: void dummy() const { }; ~_OnExit() { CODE; } } _onExit; _onExit.dummy()

	const std::string DEFAULT_STRING = "[DEFAULT]";
	std::string DEFAULT_VALUE;
	std::string ERROR_VALUE;
	const int BAR_HEIGHT = 30;

	PropertiesPanelView::PropertiesPanelView(MyGUI::Widget* _parent) :
		BaseLayout("PropertiesPanelView.layout", _parent),
		mPanelView(nullptr),
		mPairsCounter(0),
		mPanelMainProperties(nullptr),
		mPanelItems(nullptr),
		mPanelUserData(nullptr),
		mPanelControllers(nullptr),
		mCurrentWidget(nullptr),
		mPropertyItemHeight(0),
		mToolTip(nullptr)
	{
		DEFAULT_VALUE = replaceTags("ColourDefault") + DEFAULT_STRING;
		ERROR_VALUE = replaceTags("ColourError");

		assignBase(mPanelView, "scroll_View");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
		{
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &PropertiesPanelView::notifyWindowChangeCoord);
			mOldSize = window->getSize();
		}

		mToolTip = new EditorToolTip();

		mPanelMainProperties = new PanelMainProperties();
		mPanelView->addItem(mPanelMainProperties);
		mPanelMainProperties->eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
		mPanelMainProperties->eventSetPositionText = MyGUI::newDelegate(this, &PropertiesPanelView::setPositionText);

		for (int i = 0; i < MAX_BASE_TYPES_COUNT; ++i)
		{
			mPanelsTypeProperties[i] = new PanelProperties();
			mPanelView->addItem(mPanelsTypeProperties[i]);
			mPanelsTypeProperties[i]->eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
		}

		mPanelItems = new PanelItems();
		mPanelView->addItem(mPanelItems);

		mPanelUserData = new PanelUserData();
		mPanelView->addItem(mPanelUserData);

		mPanelControllers = new PanelControllers();
		mPanelView->addItem(mPanelControllers);
		mPanelControllers->eventCreatePair = MyGUI::newDelegate(this, &PropertiesPanelView::createPropertiesWidgetsPair);
		mPanelControllers->eventHidePairs = MyGUI::newDelegate(this, &PropertiesPanelView::hideWidgetsPairs);

		mPanels.push_back(mPanelMainProperties);
		for (int i = 0; i < MAX_BASE_TYPES_COUNT; ++i)
		{
			mPanels.push_back(mPanelsTypeProperties[i]);
		}
		mPanels.push_back(mPanelItems);
		mPanels.push_back(mPanelUserData);
		mPanels.push_back(mPanelControllers);

		mPropertyItemHeight = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<int>("PropertyItemHeight");

		WidgetSelectorManager::getInstance().eventChangeSelectedWidget += MyGUI::newDelegate(this, &PropertiesPanelView::notifyChangeSelectedWidget);
		CommandManager::getInstance().registerCommand("Command_ToggleRelativeMode", MyGUI::newDelegate(this, &PropertiesPanelView::commandToggleRelativeMode));

		notifyChangeSelectedWidget(nullptr);
	}

	PropertiesPanelView::~PropertiesPanelView()
	{
		WidgetSelectorManager::getInstance().eventChangeSelectedWidget -= MyGUI::newDelegate(this, &PropertiesPanelView::notifyChangeSelectedWidget);

		delete mToolTip;
		mToolTip = nullptr;

		mPanelView->removeAllItems();
		delete mPanelMainProperties;
		for (int i = 0; i < MAX_BASE_TYPES_COUNT; ++i)
		{
			delete mPanelsTypeProperties[i];
		}
		delete mPanelItems;
		delete mPanelUserData;
		delete mPanelControllers;
	}

	void PropertiesPanelView::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		const MyGUI::IntSize& size = _sender->getSize();
		if (size != mOldSize)
		{
			mOldSize = size;
			mPanelView->setNeedUpdate();
		}
	}

	void PropertiesPanelView::notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget)
	{
		mCurrentWidget = _currentWidget;

		if (nullptr != mCurrentWidget)
		{
			MyGUI::LayerManager::getInstance().upLayerItem(mCurrentWidget);
			MyGUI::IntCoord coord = mCurrentWidget->getCoord();
			MyGUI::Widget* parent = mCurrentWidget->getParent();
			if (nullptr != parent)
			{
				// если выбрали виджет на табе, то подн€ть лист таба
				if (parent->isType<MyGUI::TabItem>())
				{
					MyGUI::Tab* tab = parent->getParent()->castType<MyGUI::Tab>();
					MyGUI::TabItem* sheet = parent->castType<MyGUI::TabItem>();
					tab->setItemSelected(sheet);
				}
				// если выбрали лист таба, то подн€ть лист таба
				if (mCurrentWidget->isType<MyGUI::TabItem>())
				{
					MyGUI::Tab* tab = parent->castType<MyGUI::Tab>();
					MyGUI::TabItem* sheet = mCurrentWidget->castType<MyGUI::TabItem>();
					tab->setItemSelected(sheet);
				}
				coord = mCurrentWidget->getAbsoluteCoord();
			}

			eventChangeCoord(coord);
		}

		// delete(hide) all previous properties
		for (MapVectorStaticText::iterator iterVector = mPropertiesText.begin(); iterVector != mPropertiesText.end(); ++iterVector)
		{
			hideWidgetsPairs(iterVector->first);
		}

		if (nullptr == mCurrentWidget)
		{
			mPanelMainProperties->setVisible(false);
			mPanelItems->setVisible(false);
			mPanelUserData->setVisible(false);
			mPanelControllers->setVisible(false);
			for (int i = 0; i < MAX_BASE_TYPES_COUNT; ++i)
				mPanelsTypeProperties[i]->setVisible(false);
		}
		else
		{
			mPanelMainProperties->setVisible(true);
			mPanelItems->setVisible(true);
			mPanelUserData->setVisible(true);
			mPanelControllers->setVisible(true);

			mPairsCounter = 0;
			mPanelMainProperties->update(mCurrentWidget);

			std::string widgetTypeName = mCurrentWidget->getTypeName();

			WidgetContainer* container = EditorWidgets::getInstance().find(mCurrentWidget);
			for (MyGUI::VectorStringPairs::iterator item = container->mUserString.begin(); item != container->mUserString.end(); ++item)
			{
				if ((*item).first == "TargetWidgetType")
				{
					widgetTypeName = (*item).second;
					break;
				}
			}

			WidgetStyle* widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetTypeName);
			for (int i = 0; i < MAX_BASE_TYPES_COUNT; ++i)
			{
				mPairsCounter = 0;
				mPanelsTypeProperties[i]->setVisible(true);
				mPanelsTypeProperties[i]->update(mCurrentWidget, widgetType);
				if (widgetType && !widgetType->base.empty())
				{
					widgetType = WidgetTypes::getInstance().findWidgetStyle(widgetType->base);
				}
				else
				{
					widgetType = nullptr;
				}
			}
			mPanelItems->update(mCurrentWidget);
			mPanelUserData->update(mCurrentWidget);
			mPairsCounter = 0;
			mPanelControllers->update(mCurrentWidget);
		}
	}

	void PropertiesPanelView::hideWidgetsPairs(MyGUI::Widget* _window)
	{
		mPairsCounter = 0;
		for (VectorStaticText::iterator iter = mPropertiesText[_window].begin(); iter != mPropertiesText[_window].end(); ++iter)
		{
			(*iter)->setVisible(false);
		}

		for (MyGUI::VectorWidgetPtr::iterator iter = mPropertiesElement[_window].begin(); iter != mPropertiesElement[_window].end(); ++iter)
		{
			(*iter)->setVisible(false);
		}
	}

	void PropertiesPanelView::createPropertiesWidgetsPair(MyGUI::Widget* _window, const std::string& _property, const std::string& _value, const std::string& _type, int y)
	{
		mPairsCounter++;
		int x1 = 0;
		int x2 = 125;
		int w1 = 120;
		int w2 = _window->getWidth() - x2;
		const int h = mPropertyItemHeight;

		if (_property == "Position")
		{
			x1 = 66;
			w1 = w1 - x1;
		}

		MyGUI::StaticText* text;
		MyGUI::Widget* editOrCombo;
		//int string_int_float; // 0 - string, 1 - int, 2 - float

		enum PropertyType
		{
			PropertyType_Edit,
			PropertyType_ComboBox,
			PropertyType_EditAcceptOnly,
			PropertyType_Count
		};

		PropertyType widget_for_type;

		bool needTooltip = false;
		std::string type_names[PropertyType_Count] = { "Edit", "ComboBox", "Edit" };

		if ("Name" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("Skin" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("Position" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("Layer" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("String" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("StringAccept" == _type)
			widget_for_type = PropertyType_EditAcceptOnly;
		else if ("Align" == _type)
			widget_for_type = PropertyType_ComboBox;
		else if ("FileName" == _type)
			widget_for_type = PropertyType_Edit;
		// не совсем правильно FIXME
		else if ("1 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("2 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("4 int" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("alpha" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("1 float" == _type)
			widget_for_type = PropertyType_Edit;
		else if ("2 float" == _type)
			widget_for_type = PropertyType_Edit;
		// надо сделать проще FIXME
		else if ("Colour" == _type)
			widget_for_type = PropertyType_Edit; //"Colour" хорошо бы колорпикером
		else if ("MessageButton" == _type)
			widget_for_type = PropertyType_ComboBox;
		else
			widget_for_type = PropertyType_ComboBox;

		if (mPropertiesText[_window].size() < mPairsCounter)
		{
			text = _window->createWidget<MyGUI::StaticText>("StaticText", x1, y, w1, h, MyGUI::Align::Default);
			text->setTextAlign(MyGUI::Align::Right);

			mPropertiesText[_window].push_back(text);
		}
		else
		{
			text = mPropertiesText[_window][mPairsCounter-1];
			text->setVisible(true);
			text->setCoord(x1, y, w1, h);
		}
		std::string prop = _property;

		text->setCaption(prop);

		if ((mPropertiesElement[_window].size() < mPairsCounter) ||
			(type_names[widget_for_type] != mPropertiesElement[_window][mPairsCounter-1]->getTypeName()))
		{
			if (widget_for_type == PropertyType_Edit)
			{
				editOrCombo = _window->createWidget<MyGUI::Edit>("Edit", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
				editOrCombo->castType<MyGUI::Edit>()->eventEditTextChange += newDelegate (this, &PropertiesPanelView::notifyTryApplyProperties);
				editOrCombo->castType<MyGUI::Edit>()->eventEditSelectAccept += newDelegate (this, &PropertiesPanelView::notifyForceApplyProperties);
			}
			else if (widget_for_type == PropertyType_ComboBox)
			{
				editOrCombo = _window->createWidget<MyGUI::ComboBox>("ComboBox", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
				editOrCombo->castType<MyGUI::ComboBox>()->eventComboAccept += newDelegate (this, &PropertiesPanelView::notifyForceApplyProperties2);

				editOrCombo->castType<MyGUI::ComboBox>()->setComboModeDrop(true);

				editOrCombo->setNeedToolTip(true);
				editOrCombo->eventToolTip += newDelegate (this, &PropertiesPanelView::notifyToolTip);
			}
			else if (widget_for_type == PropertyType_EditAcceptOnly)
			{
				editOrCombo = _window->createWidget<MyGUI::Edit>("Edit", x2, y, w2, h, MyGUI::Align::Top | MyGUI::Align::HStretch);
				editOrCombo->castType<MyGUI::Edit>()->eventEditSelectAccept += newDelegate (this, &PropertiesPanelView::notifyForceApplyProperties);
			}

			if (mPropertiesElement[_window].size() < mPairsCounter)
			{
				mPropertiesElement[_window].push_back(editOrCombo);
			}
			else
			{
				MyGUI::Gui::getInstance().destroyWidget(mPropertiesElement[_window][mPairsCounter-1]);
				mPropertiesElement[_window][mPairsCounter-1] = editOrCombo;
			}
		}
		else
		{
			editOrCombo = mPropertiesElement[_window][mPairsCounter-1];
			if (widget_for_type == 1) editOrCombo->castType<MyGUI::ComboBox>()->removeAllItems();
			editOrCombo->setVisible(true);
			editOrCombo->setCoord(x2, y, w2, h);
		}

		// fill possible values
		if (widget_for_type == PropertyType_ComboBox)
		{
			std::vector<std::string> values;
			if (_type == "Skin")
			{
				values = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName())->skin;

				// добавл€ем скины и шаблоны
				MyGUI::ResourceManager::EnumeratorPtr resource = MyGUI::ResourceManager::getInstance().getEnumerator();
				while (resource.next())
				{
					MyGUI::ResourceSkin* resourceSkin = resource.current().second->castType<MyGUI::ResourceSkin>(false);
					if (resourceSkin != nullptr)
						values.push_back(replaceTags("ColourDefault") + resourceSkin->getResourceName());

					MyGUI::ResourceLayout* resourceLayout = resource.current().second->castType<MyGUI::ResourceLayout>(false);
					if (resourceLayout != nullptr)
					{
						if (resourceLayout->getResourceName().find(".layout") == MyGUI::UString::npos)
							values.push_back(replaceTags("ColourDefault") + resourceLayout->getResourceName());
					}
				}
			}
			else
			{
				values = WidgetTypes::getInstance().findPossibleValues(_type);
			}

			for (std::vector<std::string>::iterator iter = values.begin(); iter != values.end(); ++iter)
				editOrCombo->castType<MyGUI::ComboBox>()->addItem(*iter);
			editOrCombo->castType<MyGUI::ComboBox>()->beginToItemFirst();
		}

		editOrCombo->setUserString("action", _property);
		editOrCombo->setUserString("type", _type);

		if (_value.empty())
		{
			editOrCombo->castType<MyGUI::Edit>()->setCaption(DEFAULT_VALUE);
		}
		else
		{
			editOrCombo->castType<MyGUI::Edit>()->setOnlyText(_value);
			checkType(editOrCombo->castType<MyGUI::Edit>(), _type);
		}
	}

	void PropertiesPanelView::setPositionText(const std::string& _caption)
	{
		MyGUI::Widget* window = mPanelMainProperties->getMainWidget();
		if (window)
		{
			MyGUI::Widget* widget = mPropertiesElement[window][1];
			if (widget->isType<MyGUI::StaticText>())
				widget->castType<MyGUI::StaticText>()->setCaption(_caption);
		}
	}

	bool PropertiesPanelView::checkType(MyGUI::Edit* _edit, const std::string& _type)
	{
		bool success = true;
		if ("Name" == _type)
		{
			const MyGUI::UString& text = _edit->getOnlyText();
			size_t index = _edit->getTextCursor();
			WidgetContainer* textWC = EditorWidgets::getInstance().find(text);
			if ((!text.empty()) && (nullptr != textWC) &&
				(EditorWidgets::getInstance().find(mCurrentWidget) != textWC))
			{
				static const MyGUI::UString colour = ERROR_VALUE;
				_edit->setCaption(colour + text);
				success = false;
			}
			else
			{
				_edit->setCaption(text);
				success = true;
			}
			_edit->setTextCursor(index);
		}
		//else if ("Skin" == _type) widget_for_type = PropertyType_ComboBox;
		//else
		if ("Position" == _type)
		{
			if (EditorWidgets::getInstance().find(mCurrentWidget)->relative_mode)
				success = utility::checkParse<float>(_edit, 4);
			else
				success = utility::checkParse<int>(_edit, 4);
		}
		//else if ("Layer" == _type) // выбранное из комбы всегда корректно, а если в лейауте криво было - сами виноваты
		//else if ("String" == _type) // неправильна€ строка? O_o
		//else if ("Align" == _type) // выбранное из комбы всегда корректно, а если в лейауте криво было - сами виноваты
		else if ("1 int" == _type) success = utility::checkParse<int>(_edit, 1);
		else if ("2 int" == _type) success = utility::checkParse<int>(_edit, 2);
		else if ("4 int" == _type) success = utility::checkParse<int>(_edit, 4);
		else if ("alpha" == _type) success = utility::checkParseInterval<float>(_edit, 1, 0., 1.);
		else if ("1 float" == _type) success = utility::checkParse<float>(_edit, 1);
		else if ("2 float" == _type) success = utility::checkParse<float>(_edit, 2);
		// надо сделать колорпикером и без проверки FIXME
		//else if ("Colour" == _type) success = Parse::checkParse<float>(_edit, 4);
		else if ("FileName" == _type) success = utility::checkParseFileName(_edit);
		return success;
	}

	void PropertiesPanelView::notifyApplyProperties(MyGUI::Widget* _sender, bool _force)
	{
		EditorWidgets* ew = &EditorWidgets::getInstance();
		WidgetContainer* widgetContainer = ew->find(mCurrentWidget);
		MyGUI::Edit* senderEdit = _sender->castType<MyGUI::Edit>();
		std::string action = senderEdit->getUserString("action");
		std::string value = senderEdit->getOnlyText();
		std::string type = senderEdit->getUserString("type");

		ON_EXIT(UndoManager::getInstance().addValue(PR_PROPERTIES););

		bool goodData = checkType(senderEdit, type);

		if (value == DEFAULT_STRING && senderEdit->getCaption() == DEFAULT_VALUE) value = "";

		if (action == "Name")
		{
			if (goodData)
			{
				widgetContainer->name = value;
				ew->invalidateWidgets();
			}
			return;
		}
		else if (action == "Skin")
		{
			widgetContainer->skin = value;
			if (isSkinExist(widgetContainer->skin) || widgetContainer->skin.empty())
			{
				MyGUI::xml::Document* savedDoc = ew->savexmlDocument();
				ew->clear();
				ew->loadxmlDocument(savedDoc);
				delete savedDoc;
				WidgetSelectorManager::getInstance().setSelectedWidget(nullptr);
			}
			else
			{
				std::string mess = MyGUI::utility::toString("Skin '", widgetContainer->skin, "' not found. This value will be saved.");
				GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
			}
			return;
		}
		else if (action == "Position")
		{
			if (!goodData) return;
			if (widgetContainer->relative_mode)
			{
				std::istringstream str(value);
				MyGUI::FloatCoord float_coord;
				str >> float_coord;
				float_coord.left = float_coord.left / 100;
				float_coord.top = float_coord.top / 100;
				float_coord.width = float_coord.width / 100;
				float_coord.height = float_coord.height / 100;
				MyGUI::IntCoord coord = MyGUI::CoordConverter::convertFromRelative(float_coord, mCurrentWidget->getParentSize());
				mCurrentWidget->setCoord(coord);

				eventChangeCoord(mCurrentWidget->getAbsoluteCoord());
				return;
			}
			widgetContainer->widget->setProperty("Widget_Coord", value);

			eventChangeCoord(mCurrentWidget->getAbsoluteCoord());
			return;
		}
		else if (action == "Align")
		{
			widgetContainer->align = value;
			widgetContainer->widget->setAlign(MyGUI::Align::parse(value));
			return;
		}
		else if (action == "Layer")
		{
			widgetContainer->setLayerName(value);
			return;
		}
		else
		{
			std::string tmp = action;
			if (splitString(tmp, ' ') == "Controller")
			{
				int n = MyGUI::utility::parseValue<int>(splitString(tmp, ' '));
				std::string key = splitString(tmp, ' ');
				widgetContainer->mController[n]->mProperty[key] = value;
				return;
			}
		}

		bool success = false;
		if (goodData || _force)
			success = ew->tryToApplyProperty(widgetContainer->widget, action, value);
		else
			return;

		if (success)
		{
			eventChangeCoord(mCurrentWidget->getAbsoluteCoord());
		}
		else
		{
			senderEdit->setCaption(DEFAULT_VALUE);
			return;
		}

		// если такое св-во было, то заменим (или удалим если стерли) значение
		for (MyGUI::VectorStringPairs::iterator iterProperty = widgetContainer->mProperty.begin(); iterProperty != widgetContainer->mProperty.end(); ++iterProperty)
		{
			if (iterProperty->first == action)
			{
				if (value.empty()) widgetContainer->mProperty.erase(iterProperty);
				else iterProperty->second = value;
				return;
			}
		}

		// если такого свойства не было раньше, то сохран€ем
		if (!value.empty()) widgetContainer->mProperty.push_back(MyGUI::PairString(action, value));
	}

	std::string PropertiesPanelView::splitString(std::string& str, char separator)
	{
		size_t spaceIdx = str.find(separator);
		if (spaceIdx == std::string::npos)
		{
			std::string tmp = str;
			str.clear();
			return tmp;
		}
		else
		{
			std::string tmp = str.substr(0, spaceIdx);
			str.erase(0, spaceIdx + 1);
			return tmp;
		}
	}

	void PropertiesPanelView::notifyTryApplyProperties(MyGUI::Edit* _sender)
	{
		notifyApplyProperties(_sender, false);
	}

	void PropertiesPanelView::notifyForceApplyProperties(MyGUI::Edit* _sender)
	{
		notifyApplyProperties(_sender, true);
	}

	void PropertiesPanelView::notifyForceApplyProperties2(MyGUI::ComboBox* _sender, size_t _index)
	{
		notifyApplyProperties(_sender, true);
	}

	void PropertiesPanelView::commandToggleRelativeMode(const MyGUI::UString& _commandName, bool& _result)
	{
		toggleRelativeMode();

		_result = true;
	}

	void PropertiesPanelView::toggleRelativeMode()
	{
		mPanelMainProperties->notifyToggleRelativeMode();
	}

	void PropertiesPanelView::setCoord(const MyGUI::IntCoord& _coord)
	{
		setPositionText(_coord.print());
	}

	bool PropertiesPanelView::isSkinExist(const std::string& _skinName)
	{
		return _skinName == "Default" || MyGUI::SkinManager::getInstance().isExist(_skinName) || MyGUI::LayoutManager::getInstance().isExist(_skinName);
	}

	void PropertiesPanelView::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
	{
		if (_sender->getUserString("type") == "Skin")
		{
			if (_info.type == MyGUI::ToolTipInfo::Show)
			{
				if (_info.index != MyGUI::ITEM_NONE)
				{
					SkinInfo data = getCellData(_sender, _info.index);
					mToolTip->show(data);
					mToolTip->move(_info.point);
				}
			}
			else if (_info.type == MyGUI::ToolTipInfo::Hide)
			{
				mToolTip->hide();
			}
			else if (_info.type == MyGUI::ToolTipInfo::Move)
			{
				mToolTip->move(_info.point);
			}
		}
	}

	SkinInfo PropertiesPanelView::getCellData(MyGUI::Widget* _sender, size_t _index)
	{
		MyGUI::ComboBox* box = _sender->castType<MyGUI::ComboBox>();
		MyGUI::UString name = box->getItemNameAt(_index);
		return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "Widget", "");
	}

} // namespace tools

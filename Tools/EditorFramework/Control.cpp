/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "Control.h"
#include "FactoryManager.h"
#include "CommandManager.h"

namespace tools
{

	Control::~Control()
	{
		DeactivateControllers();

		for (auto& controller : mControllers)
			delete controller;
		mControllers.clear();

		for (auto& child : mChilds)
			delete child;
		mChilds.clear();
	}

	MyGUI::Widget* Control::getRoot()
	{
		return mMainWidget;
	}

	void Control::Initialise(std::string_view _layoutName)
	{
		OnInitialise(nullptr, nullptr, _layoutName);
		ActivateControllers();
	}

	void Control::Initialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		OnInitialise(_parent, _place, _layoutName);
		ActivateControllers();
	}

	void Control::OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName)
	{
		mParent = _parent;
		if (_parent != nullptr)
		{
			initialise(_layoutName, _place);
			_parent->mChilds.push_back(this);
		}
		else
		{
			initialise(_layoutName, nullptr);
		}

		AdviceWidget(mMainWidget);

		for (size_t index = 0; index < getRoot()->getChildCount(); index++)
			CreateChilds(this, getRoot()->getChildAt(index));
	}

	void Control::CreateChilds(Control* _parent, MyGUI::Widget* _widget)
	{
		AdviceWidget(_widget);

		std::string_view controlType = _widget->getUserString("ControlType");
		if (!controlType.empty())
		{
			std::string_view controlLayout = _widget->getUserString("ControlLayout");

			Control* control = components::FactoryManager::GetInstance().CreateItem<Control>(controlType);
			if (control != nullptr)
			{
				control->OnInitialise(_parent, _widget, controlLayout);
				return;
			}
		}

		for (size_t index = 0; index < _widget->getChildCount(); index++)
			CreateChilds(_parent, _widget->getChildAt(index));
	}

	void Control::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		CommandManager::getInstance().executeCommand(MyGUI::UString(_sender->getUserString("CommandClick")));
	}

	const Control::VectorControl& Control::getChilds() const
	{
		return mChilds;
	}

	void Control::notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
			CommandManager::getInstance().executeCommand(
				MyGUI::UString(_sender->getItemAt(_index)->getUserString("CommandActivate")));
	}

	void Control::notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name)
	{
		if (_name == "close")
			CommandManager::getInstance().executeCommand(MyGUI::UString(_sender->getUserString("CommandClose")));
	}

	void Control::AdviceWidget(MyGUI::Widget* _widget)
	{
		std::string_view command = _widget->getUserString("CommandClick");
		if (!command.empty())
			_widget->eventMouseButtonClick += MyGUI::newDelegate(this, &Control::notifyMouseButtonClick);

		MyGUI::TabControl* tab = _widget->castType<MyGUI::TabControl>(false);
		if (tab != nullptr)
		{
			if (tab->getItemCount() != 0 && !tab->getItemAt(0)->getUserString("CommandActivate").empty())
				tab->eventTabChangeSelect += MyGUI::newDelegate(this, &Control::notifyTabChangeSelect);
		}

		MyGUI::Window* window = _widget->castType<MyGUI::Window>(false);
		if (window != nullptr && !window->getUserString("CommandClose").empty())
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &Control::notifyWindowButtonPressed);

		command = _widget->getUserString("CommandAccept");
		if (!command.empty())
		{
			MyGUI::EditBox* edit = _widget->castType<MyGUI::EditBox>(false);
			if (edit != nullptr)
				edit->eventEditSelectAccept += MyGUI::newDelegate(this, &Control::notifyEditSelectAccept);
		}
	}

	void Control::SendCommand(std::string_view _command)
	{
		OnCommand(_command);

		for (auto& child : mChilds)
			child->SendCommand(_command);
	}

	void Control::OnCommand(std::string_view _command)
	{
	}

	void Control::notifyEditSelectAccept(MyGUI::EditBox* _sender)
	{
		CommandManager::getInstance().executeCommand(MyGUI::UString(_sender->getUserString("CommandAccept")));
	}

	MyGUI::Widget* Control::CreateFakeWidgetT(std::string_view _typeName, MyGUI::Widget* _parent)
	{
		if (_parent)
			return _parent->createWidgetT(
				_typeName,
				MyGUI::SkinManager::getInstance().getDefaultSkin(),
				MyGUI::IntCoord(),
				MyGUI::Align::Default);

		return MyGUI::Gui::getInstance().createWidgetT(
			_typeName,
			MyGUI::SkinManager::getInstance().getDefaultSkin(),
			MyGUI::IntCoord(),
			MyGUI::Align::Default,
			std::string_view{});
	}

	void Control::CreateControllers()
	{
		std::string_view controllers = mMainWidget->getUserString("ControlControllers");
		if (!controllers.empty())
		{
			std::vector<std::string> values = MyGUI::utility::split(controllers, "\t\n ,");
			for (const auto& value : values)
			{
				IControlController* controller =
					components::FactoryManager::GetInstance().CreateItem<IControlController>(value);
				if (controller != nullptr)
				{
					controller->setTarget(this);
					mControllers.push_back(controller);
				}
			}
		}
	}

	void Control::ActivateControllers()
	{
		CreateControllers();

		for (auto& controller : mControllers)
			controller->activate();

		for (auto& child : mChilds)
			child->ActivateControllers();
	}

	void Control::DeactivateControllers()
	{
		for (auto& controller : mControllers)
			controller->deactivate();
	}

	void Control::Shutdown()
	{
		mParent->mChilds.erase(
			std::remove(mParent->mChilds.begin(), mParent->mChilds.end(), this),
			mParent->mChilds.end());
		mParent = nullptr;

		shutdown();
	}

}

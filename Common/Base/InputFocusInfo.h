/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#ifndef INPUT_FOCUS_INFO_H_
#define INPUT_FOCUS_INFO_H_

#include "FocusWidgetObserver.h"

namespace diagnostic
{

	class InputFocusInfo
	{
	public:
		InputFocusInfo()
		{
			MyGUI::InputManager::getInstance().eventChangeMouseFocus +=
				MyGUI::newDelegate(this, &InputFocusInfo::notifyChangeMouseFocus);
			MyGUI::InputManager::getInstance().eventChangeKeyFocus +=
				MyGUI::newDelegate(this, &InputFocusInfo::notifyChangeKeyFocus);
			mMouseFocus.eventChange += MyGUI::newDelegate(this, &InputFocusInfo::updateFocusWidgetHelpers);
			mKeyFocus.eventChange += MyGUI::newDelegate(this, &InputFocusInfo::updateFocusWidgetHelpers);
		}

		~InputFocusInfo()
		{
			MyGUI::InputManager::getInstance().eventChangeMouseFocus -=
				MyGUI::newDelegate(this, &InputFocusInfo::notifyChangeMouseFocus);
			MyGUI::InputManager::getInstance().eventChangeKeyFocus -=
				MyGUI::newDelegate(this, &InputFocusInfo::notifyChangeKeyFocus);
			mMouseFocus.eventChange.clear();
			mKeyFocus.eventChange.clear();
			mMouseFocus.setWidget(nullptr);
			mKeyFocus.setWidget(nullptr);

			if (mKeyHelper != nullptr)
				MyGUI::Gui::getInstance().destroyWidget(mKeyHelper);
			if (mMouseHelper != nullptr)
				MyGUI::Gui::getInstance().destroyWidget(mMouseHelper);
		}

		bool getFocusVisible() const
		{
			return mFocusVisible;
		}

		void setFocusVisible(bool _value)
		{
			mFocusVisible = _value;
			auto& input = MyGUI::InputManager::getInstance();
			notifyChangeMouseFocus(input.getMouseFocusWidget());
			notifyChangeKeyFocus(input.getKeyFocusWidget());
			updateFocusWidgetHelpers();
		}

	private:
		void notifyChangeMouseFocus(MyGUI::Widget* _widget)
		{
			mMouseFocus.setWidget(mFocusVisible ? _widget : nullptr);
		}

		void notifyChangeKeyFocus(MyGUI::Widget* _widget)
		{
			mKeyFocus.setWidget(mFocusVisible ? _widget : nullptr);
		}

		void updateHelper(MyGUI::Widget*& _helper, MyGUI::Widget* _focus, std::string_view _skin)
		{
			if (!mFocusVisible || _focus == nullptr)
			{
				if (_helper != nullptr)
					_helper->setVisible(false);
				return;
			}

			if (_helper == nullptr)
			{
				if (!MyGUI::LayerManager::getInstance().isExist("Statistic") ||
					!MyGUI::SkinManager::getInstance().isExist(_skin))
					return;
				_helper = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(
					_skin,
					MyGUI::IntCoord(),
					MyGUI::Align::Default,
					"Statistic");
				_helper->setNeedMouseFocus(false);
			}

			if (_helper->getCoord() != _focus->getAbsoluteCoord())
				_helper->setCoord(_focus->getAbsoluteCoord());
			_helper->setVisible(true);
		}

		void updateFocusWidgetHelpers()
		{
			updateHelper(mMouseHelper, mMouseFocus.getWidget(), "RectGreen");
			updateHelper(mKeyHelper, mKeyFocus.getWidget(), "RectBlue");
		}

	private:
		bool mFocusVisible{false};
		MyGUI::Widget* mMouseHelper{nullptr};
		MyGUI::Widget* mKeyHelper{nullptr};
		FocusWidgetObserver mMouseFocus;
		FocusWidgetObserver mKeyFocus;
	};

} // namespace diagnostic

#endif // INPUT_FOCUS_INFO_H_

/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/

#ifndef INPUT_FOCUS_INFO_H_
#define INPUT_FOCUS_INFO_H_

#include <MyGUI.h>

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
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &InputFocusInfo::notifyFrameStart);
		}

		~InputFocusInfo()
		{
			MyGUI::InputManager::getInstance().eventChangeMouseFocus -=
				MyGUI::newDelegate(this, &InputFocusInfo::notifyChangeMouseFocus);
			MyGUI::InputManager::getInstance().eventChangeKeyFocus -=
				MyGUI::newDelegate(this, &InputFocusInfo::notifyChangeKeyFocus);
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &InputFocusInfo::notifyFrameStart);

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
			if (!mFocusVisible)
			{
				if (mMouseHelper)
					mMouseHelper->setVisible(false);
				if (mKeyHelper)
					mKeyHelper->setVisible(false);
			}
		}

	private:
		void notifyChangeMouseFocus(MyGUI::Widget* _widget)
		{
			if (mFocusVisible)
			{
				mWidgetMouseFocus = _widget;
				updateFocusWidgetHelpers();
			}
		}

		void notifyChangeKeyFocus(MyGUI::Widget* _widget)
		{
			if (mFocusVisible)
			{
				mWidgetKeyFocus = _widget;
				updateFocusWidgetHelpers();
			}
		}

		void notifyFrameStart(float _time)
		{
			if (mFocusVisible)
				updateFocusWidgetHelpers(false);
		}

		void updateFocusWidgetHelpers(bool _updateinfo = true)
		{
			const std::string_view layer = "Statistic";
			const std::string_view skin_mouse = "RectGreen";
			const std::string_view skin_key = "RectBlue";

			if ((mWidgetMouseFocus != mOldMouseFocus) ||
				((mWidgetMouseFocus != nullptr) && (mMouseHelper != nullptr) &&
				 mWidgetMouseFocus->getAbsoluteCoord() != mMouseHelper->getAbsoluteCoord()))
			{
				mOldMouseFocus = mWidgetMouseFocus;

				if (mMouseHelper == nullptr)
				{
					if (!MyGUI::LayerManager::getInstance().isExist(layer))
						return;
					if (!MyGUI::SkinManager::getInstance().isExist(skin_mouse))
						return;
					mMouseHelper = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(
						skin_mouse,
						MyGUI::IntCoord(),
						MyGUI::Align::Default,
						layer);
					mMouseHelper->setNeedMouseFocus(false);
				}

				if (mWidgetMouseFocus)
				{
					//if (_updateinfo)
					//	MyGUI::MYGUI_OUT("mouse focus : ", mWidgetMouseFocus->getName());
					mMouseHelper->setCoord(mWidgetMouseFocus->getAbsoluteCoord());
					mMouseHelper->setVisible(true);
				}
				else
				{
					//if (_updateinfo)
					//	MyGUI::MYGUI_OUT("mouse focus : nullptr");
					mMouseHelper->setVisible(false);
				}
			}

			if ((mWidgetKeyFocus != mOldKeyFocus) ||
				((mWidgetKeyFocus != nullptr) && (mKeyHelper != nullptr) &&
				 mWidgetKeyFocus->getAbsoluteCoord() != mKeyHelper->getAbsoluteCoord()))
			{
				mOldKeyFocus = mWidgetKeyFocus;

				if (mKeyHelper == nullptr)
				{
					if (!MyGUI::LayerManager::getInstance().isExist(layer))
						return;
					if (!MyGUI::SkinManager::getInstance().isExist(skin_key))
						return;
					mKeyHelper = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(
						skin_key,
						MyGUI::IntCoord(),
						MyGUI::Align::Default,
						layer);
					mKeyHelper->setNeedMouseFocus(false);
				}
				if (mWidgetKeyFocus)
				{
					//if (_updateinfo)
					//	MyGUI::MYGUI_OUT("key focus : ", mWidgetKeyFocus->getName());
					mKeyHelper->setCoord(mWidgetKeyFocus->getAbsoluteCoord());
					mKeyHelper->setVisible(true);
				}
				else
				{
					//if (_updateinfo)
					//	MyGUI::MYGUI_OUT("key focus : nullptr");
					mKeyHelper->setVisible(false);
				}
			}
		}

	private:
		bool mFocusVisible{false};
		MyGUI::Widget* mMouseHelper{nullptr};
		MyGUI::Widget* mKeyHelper{nullptr};

		MyGUI::Widget* mWidgetMouseFocus{nullptr};
		MyGUI::Widget* mWidgetKeyFocus{nullptr};
		MyGUI::Widget* mOldMouseFocus{nullptr};
		MyGUI::Widget* mOldKeyFocus{nullptr};
	};

} // namespace diagnostic

#endif // INPUT_FOCUS_INFO_H_

/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef CONTEXT_MENU_H_
#define CONTEXT_MENU_H_

#include <MyGUI.h>
#include <BaseLayout/BaseLayout.h>

namespace wraps
{

	class ContextMenu : public BaseLayout
	{
	public:
		ContextMenu(std::string_view _layout) :
			BaseLayout(_layout, nullptr)
		{
			assignWidget(mMenu, "_Main");
			mMenu->setVisible(false);
			mMenu->eventMenuCtrlAccept += MyGUI::newDelegate(this, &ContextMenu::notifyMenuCtrlAccept);
		}

		void setVisible(bool _value)
		{
			mMenu->setPosition(MyGUI::InputManager::getInstance().getMousePosition());
			mMenu->setVisibleSmooth(_value);
		}

		bool isVisible()
		{
			return mMenu->getVisible();
		}

		/** Event : Menu accept.\n
			signature : void method(wraps::ContextMenu* _sender, std::string_view _id)
			@param _sender
			@param _id
		*/
		MyGUI::delegates::Delegate<ContextMenu*, std::string_view> eventMenuAccept;

	private:
		void notifyMenuCtrlAccept(MyGUI::MenuControl* _sender, MyGUI::MenuItem* _item)
		{
			eventMenuAccept(this, _item->getItemId());
		}

	private:
		MyGUI::PopupMenu* mMenu;
	};

} // namespace wraps

#endif // CONTEXT_MENU_H_

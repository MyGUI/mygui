/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __CONTEXT_MENU_H__
#define __CONTEXT_MENU_H__

#include <MyGUI.h>
#include <BaseLayout/BaseLayout.h>

namespace wraps
{

	class ContextMenu : public BaseLayout
	{
	public:
		ContextMenu(const std::string& _layout) :
	  		BaseLayout(_layout, nullptr)
		{
			assignWidget(mMenu, "_Main");
			mMenu->setVisible(false);
			mMenu->eventMenuCtrlAccept = MyGUI::newDelegate(this, &ContextMenu::notifyMenuCtrlAccept);
		}

		void setVisible(bool _value)
		{
			mMenu->setPosition(MyGUI::InputManager::getInstance().getMousePosition());
			mMenu->setVisibleSmooth(_value);
		}

		bool isVisible()
		{
			return mMenu->isVisible();
		}

		/** Event : Menu accept.\n
			signature : void method(wraps::ContextMenu* _sender, const std::string& _id)
			@param _sender
			@param _id
		*/
		MyGUI::delegates::CDelegate2<ContextMenu*, const std::string&> eventMenuAccept;

	private:
		void notifyMenuCtrlAccept(MyGUI::MenuCtrlPtr _sender, MyGUI::MenuItemPtr _item)
		{
			eventMenuAccept(this, _item->getItemId());
		}

	private:
		MyGUI::PopupMenuPtr mMenu;

	};

} // namespace wraps

#endif // __CONTEXT_MENU_H__

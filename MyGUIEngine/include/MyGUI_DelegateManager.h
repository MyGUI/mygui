/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
	@module
*/
#ifndef __MYGUI_DELEGATE_MANAGER_H__
#define __MYGUI_DELEGATE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Common.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
   /**
      Manager for handling delegates by names through layout file.
   */

	typedef delegates::IDelegate1<WidgetPtr> IEventInfo_WidgetVoid;

	class _MyGUIExport DelegateManager
	{
		INSTANCE_HEADER(DelegateManager);
   public:
		typedef std::map<std::string, EventInfo_WidgetVoid> MapDelegate;
	public:
		void initialise();
		void shutdown();

      /** Add new delegate
			example :
				registerConsoleDelegate("delegate_name_1", newDelegate(your_func));
				registerConsoleDelegate("delegate_name_2", newDelegate(your_static_method));
				registerConsoleDelegate("delegate_name_3", newDelegate(your_class_ptr, &your_class_name::your_method_name));

			signature : void method(MyGUI::WidgetPtr _sender)
		*/
      void addDelegate(const std::string & _key, IEventInfo_WidgetVoid * _delegate);

      /** Remove delegate */
      void removeDelegate(const std::string & _key);
   private:
      void callDelegate(const std::string & _key, WidgetPtr _sender);
   public:
      void eventMouseMove(MyGUI::WidgetPtr _sender, int _left, int _top)
      {
         callDelegate(_sender->getUserString("eventMouseMove"), _sender);
      }

      MapDelegate mDelegates;
	};

} // namespace MyGUI

#endif // __MYGUI_DELEGATE_MANAGER_H__

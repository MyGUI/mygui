/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __WIDGET_CREATOR_MANAGER_H__
#define __WIDGET_CREATOR_MANAGER_H__

#include <MyGUI.h>

namespace tools
{
	class WidgetCreatorManager :
		public MyGUI::Singleton<WidgetCreatorManager>
	{
	public:
		WidgetCreatorManager();
		virtual ~WidgetCreatorManager();

		void initialise();
		void shutdown();
	};

} // namespace tools

#endif // __WIDGET_CREATOR_MANAGER_H__

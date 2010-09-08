/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __WIDGET_CREATOR_MANAGER_H__
#define __WIDGET_CREATOR_MANAGER_H__

#include <MyGUI.h>
#include "WidgetContainer.h"

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

		void notifyMouseMouseMove(const MyGUI::IntPoint& _point);
		void notifyMouseButtonPressed(const MyGUI::IntPoint& _point);
		void notifyMouseButtonReleased(const MyGUI::IntPoint& _point);

	private:
		void notifyChangeSelectedWidget(MyGUI::Widget* _currentWidget);

		MyGUI::Widget* getTopWidget(const MyGUI::IntPoint& _point);
		bool checkContainer(WidgetContainer* _container, MyGUI::Widget*& _result, const MyGUI::IntPoint& _point);

	private:
		size_t mSelectDepth;
		bool mMouseButtonPressed;
		int mLastClickX;
		int mLastClickY;
	};

} // namespace tools

#endif // __WIDGET_CREATOR_MANAGER_H__

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

	typedef MyGUI::delegates::CMultiDelegate1<bool> Event_ChangeCreatorMode;
	typedef MyGUI::delegates::CMultiDelegate2<bool, const MyGUI::IntCoord&> Event_ChangeSelector;

	class WidgetCreatorManager :
		public MyGUI::Singleton<WidgetCreatorManager>
	{
	public:
		WidgetCreatorManager();
		virtual ~WidgetCreatorManager();

		void initialise();
		void shutdown();

		void createNewWidget(const MyGUI::IntPoint& _point);
		void moveNewWidget(const MyGUI::IntPoint& _point);
		void finishNewWidget(const MyGUI::IntPoint& _point);

		void setCreatorInfo(const std::string& _widgetType, const std::string& _widgetSkin);
		void resetCreatorInfo();

		void setPopupMode(bool _value);
		bool getPopupMode() const;

		bool getCreateMode();
		const std::string& getWidgetType();
		const std::string& getWidgetSkin();

		Event_ChangeCreatorMode eventChangeCreatorMode;
		Event_ChangeSelector eventChangeSelector;

	private:
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		void resetWidget();

		int toGrid(int _value);
		MyGUI::IntCoord getCoordNewWidget(const MyGUI::IntPoint& _point);
		void resetAllCreatorInfo();

	private:
		bool mCreateMode;
		std::string mWidgetType;
		std::string mWidgetSkin;
		bool mStartNewWidget;
		MyGUI::IntPoint mStartPoint;
		MyGUI::Widget* mNewWidget;
		int mGridStep;
		bool mPopupMode;
	};

} // namespace tools

#endif // __WIDGET_CREATOR_MANAGER_H__

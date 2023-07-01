/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _47bb2ab9_21d5_4357_bbf9_061bf6ae41f1_
#define _47bb2ab9_21d5_4357_bbf9_061bf6ae41f1_

#include <MyGUI.h>
#include "WidgetContainer.h"

namespace tools
{

	using Event_ChangeCreatorMode = MyGUI::delegates::MultiDelegate<bool>;
	using Event_ChangeSelector = MyGUI::delegates::MultiDelegate<bool, const MyGUI::IntCoord&>;

	class WidgetCreatorManager
	{
		MYGUI_SINGLETON_DECLARATION(WidgetCreatorManager);

	public:
		WidgetCreatorManager();

		void initialise();
		void shutdown();

		void createNewWidget(const MyGUI::IntPoint& _point);
		void moveNewWidget(const MyGUI::IntPoint& _point);
		void finishNewWidget(const MyGUI::IntPoint& _point);

		void setCreatorInfo(std::string_view _widgetType, std::string_view _widgetSkin);
		void resetCreatorInfo();

		void setPopupMode(bool _value);
		bool getPopupMode() const;

		bool getCreateMode() const;
		const std::string& getWidgetType() const;
		const std::string& getWidgetSkin() const;

		Event_ChangeCreatorMode eventChangeCreatorMode;
		Event_ChangeSelector eventChangeSelector;

	private:
		void resetWidget();

		MyGUI::IntCoord getCoordNewWidget(const MyGUI::IntPoint& _point);
		void resetAllCreatorInfo();

	private:
		bool mCreateMode{false};
		std::string mWidgetType;
		std::string mWidgetSkin;
		bool mStartNewWidget{false};
		MyGUI::IntPoint mStartPoint;
		MyGUI::Widget* mNewWidget{nullptr};
		bool mPopupMode{false};
	};

}

#endif

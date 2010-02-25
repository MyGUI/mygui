/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef __MONITOR_PANEL_H__
#define __MONITOR_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(MonitorPanel, "Monitor.layout");
	class MonitorPanel :
		public wraps::BaseLayout
	{
	public:
		MonitorPanel();
		virtual ~MonitorPanel();

	private:
		void notifyCommand(const std::string& _name, MyGUI::Any _data);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mShipImage, "Ship");
		MyGUI::StaticImage* mShipImage;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mDescription, "Description");
		MyGUI::StaticText* mDescription;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mEnergy, "Energy");
		MyGUI::Progress* mEnergy;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mExplosion, "Explosion");
		MyGUI::Progress* mExplosion;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mTarget, "Target");
		MyGUI::Progress* mTarget;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mHP, "HP");
		MyGUI::Progress* mHP;
	};

} // namespace demo

#endif // __MONITOR_PANEL_H__

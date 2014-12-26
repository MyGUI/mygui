/*!
	@file
	@author		Albert Semenov
	@date		11/2009
*/
#ifndef MONITOR_PANEL_H_
#define MONITOR_PANEL_H_

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
		MyGUI::ImageBox* mShipImage;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mDescription, "Description");
		MyGUI::TextBox* mDescription;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mEnergy, "Energy");
		MyGUI::ProgressBar* mEnergy;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mExplosion, "Explosion");
		MyGUI::ProgressBar* mExplosion;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mTarget, "Target");
		MyGUI::ProgressBar* mTarget;
		ATTRIBUTE_FIELD_WIDGET_NAME(MonitorPanel, mHP, "HP");
		MyGUI::ProgressBar* mHP;
	};

} // namespace demo

#endif // MONITOR_PANEL_H_

/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_SELECTOR_CONTROL_H__
#define __REGION_SELECTOR_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	typedef MyGUI::delegates::CMultiDelegate0 EventHandle_ChangePosition;

	class RegionSelectorControl :
		public wraps::BaseLayout
	{
	public:
		RegionSelectorControl(MyGUI::Widget* _parent);
		virtual ~RegionSelectorControl();

		void setVisible(bool _value);
		void setScale(double _value);

		void setCoord(const MyGUI::IntCoord& _value);
		const MyGUI::IntCoord& getCoord();

		EventHandle_ChangePosition eventChangePosition;

	private:
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

		void updateCoord();

	private:
		MyGUI::IntCoord mCoordValue;
		double mScaleValue;
	};

} // namespace tools

#endif // __REGION_SELECTOR_CONTROL_H__

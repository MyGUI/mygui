/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_SELECTOR_CONTROL_2_H__
#define __REGION_SELECTOR_CONTROL_2_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	typedef MyGUI::delegates::CMultiDelegate0 EventHandle_ChangePosition;

	class RegionSelectorControl2 :
		public wraps::BaseLayout
	{
	public:
		RegionSelectorControl2(MyGUI::Widget* _parent);
		virtual ~RegionSelectorControl2();

		void setVisible(bool _value);
		void setScale(double _value);

		void setPosition(const MyGUI::IntPoint& _value);
		void setSize(const MyGUI::IntSize& _value);
		MyGUI::IntPoint getPosition();

		EventHandle_ChangePosition eventChangePosition;

	private:
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

		void updateCoord();

	private:
		MyGUI::IntCoord mCoordValue;
		MyGUI::IntCoord mCoordReal;
		double mScaleValue;
	};

} // namespace tools

#endif // __REGION_SELECTOR_CONTROL_H__

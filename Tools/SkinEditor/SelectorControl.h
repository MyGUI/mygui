/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SELECTOR_CONTROL_H__
#define __SELECTOR_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	typedef MyGUI::delegates::CMultiDelegate0 EventHandle_ChangePosition;

	class SelectorControl :
		public wraps::BaseLayout
	{
	public:
		SelectorControl(const std::string& _layout, MyGUI::Widget* _parent);
		virtual ~SelectorControl();

		void setVisible(bool _value);
		void setScale(double _value);
		void setEnabled(bool _value);

		void setPosition(const MyGUI::IntPoint& _value);
		void setSize(const MyGUI::IntSize& _value);
		void setCoord(const MyGUI::IntCoord& _value);

		MyGUI::IntPoint getPosition();
		MyGUI::IntSize getSize();
		const MyGUI::IntCoord& getCoord();

		bool getCapture();

		EventHandle_ChangePosition eventChangePosition;

	private:
		void notifyWindowChangeCoord(MyGUI::Window* _sender);
		void notifyFrameStart(float _time);

		void updateCoord();

	private:
		MyGUI::IntCoord mCoordValue;
		MyGUI::IntCoord mCoordReal;
		double mScaleValue;
		bool mPositionChanged;
		MyGUI::Widget* mProjection;
		MyGUI::IntCoord mProjectionDiff;
		bool mCapture;
	};

} // namespace tools

#endif // __SELECTOR_CONTROL_H__

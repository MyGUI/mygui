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

	class SelectorControl;

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
		const MyGUI::IntCoord& getCoord() const;

		bool getCapture();
		MyGUI::IntCoord getActionScale();

		EventHandle_ChangePosition eventChangePosition;

		MyGUI::Widget* getMainWidget();

	protected:
		void setPropertyColour(const std::string& _propertyName);

	private:
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

		void setColour(MyGUI::Colour _value);
		void updateCoord();

		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

	private:
		MyGUI::IntCoord mCoordValue;
		MyGUI::IntCoord mCoordReal;
		double mScaleValue;
		MyGUI::Widget* mProjection;
		MyGUI::IntCoord mProjectionDiff;
		std::string mPropertyColour;
	};

} // namespace tools

#endif // __SELECTOR_CONTROL_H__

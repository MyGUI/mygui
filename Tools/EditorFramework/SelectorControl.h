/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _5e4ba082_bfdd_4106_b851_0f79eb318b0e_
#define _5e4ba082_bfdd_4106_b851_0f79eb318b0e_

#include "Control.h"
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL SelectorControl : public Control, public sigslot::has_slots<>
	{
	public:
		~SelectorControl() override;

		void setVisible(bool _value);
		bool getVisible() const;

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

		sigslot::signal1<SelectorControl*> eventChangePosition;

		MyGUI::Widget* getMainWidget();

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;
		void setPropertyColour(std::string_view _propertyName);

	private:
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

		void setColour(MyGUI::Colour _value);
		void updateCoord();

		void notifySettingsChanged(std::string_view _path);

	private:
		MyGUI::IntCoord mCoordValue;
		MyGUI::IntCoord mCoordReal;
		double mScaleValue{1.0};
		MyGUI::Widget* mProjection{nullptr};
		MyGUI::IntCoord mProjectionDiff;
		std::string mPropertyColour;
	};

}

#endif

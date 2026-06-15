/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _618193ae_b8d7_415e_af0e_2c39becbae9c_
#define _618193ae_b8d7_415e_af0e_2c39becbae9c_

#include "BaseLayout/BaseLayout.h"
#include "sigslot.h"

namespace tools
{

	class SelectorControlLE;

	using EventHandle_ChangePosition = MyGUI::delegates::MultiDelegate<>;


	class SelectorControlLE : public wraps::BaseLayout, public sigslot::has_slots<>
	{
	public:
		SelectorControlLE(std::string_view _layout, MyGUI::Widget* _parent);
		~SelectorControlLE() override;

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
		MyGUI::IntCoord getActionScale() const;

		EventHandle_ChangePosition eventChangePosition;

		MyGUI::Widget* getMainWidget();

	protected:
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
		MyGUI::Widget* mProjection;
		MyGUI::IntCoord mProjectionDiff;
		std::string mPropertyColour;
	};

}

#endif

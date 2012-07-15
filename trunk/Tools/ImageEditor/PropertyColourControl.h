/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _322f22c1_b944_4d2b_b2a6_bcf620978547_
#define _322f22c1_b944_4d2b_b2a6_bcf620978547_

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"
#include "ColourPanel.h"

/*namespace tools
{

	class PropertyColourControl :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyColourControl(MyGUI::Widget* _parent);
		virtual ~PropertyColourControl();

	protected:
		virtual void updateProperty();

	private:
		void notifyEditTextChange(MyGUI::EditBox* _sender);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyPreviewColour(const MyGUI::Colour& _value);

		bool parseColour1(const std::string& _value, MyGUI::Colour& _resultValue);
		bool parseColour2(const std::string& _value, MyGUI::Colour& _resultValue);
		bool parseColour3(const std::string& _value, MyGUI::Colour& _resultValue);

		bool isValidate(MyGUI::Colour& _resultValue);
		MyGUI::UString getClearValue();
		void setColour(bool _validate);

		void updateSetProperty();

	private:
		MyGUI::EditBox* mEdit;
		MyGUI::Widget* mColour;

		ColourPanel* mColourPanel;

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mPreviewColour;
	};

}*/

#endif

/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _f8756cca_926c_499e_af27_04797b062e2a_
#define _f8756cca_926c_499e_af27_04797b062e2a_

#include "PropertyControl.h"
#include "TextureBrowseControl.h"
#include "Dialog.h"

namespace tools
{

	class PropertyFontSourceControl :
		public PropertyControl
	{
	public:
		PropertyFontSourceControl();
		virtual ~PropertyFontSourceControl();

	protected:
		virtual void updateCaption();
		virtual void updateProperty();
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

		void fillResources();

	private:
		MyGUI::TextBox* mName;
		MyGUI::ComboBox* mComboBox;

		MyGUI::VectorString mResources;
	};

}

#endif

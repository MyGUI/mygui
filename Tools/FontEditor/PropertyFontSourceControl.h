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

	class PropertyFontSourceControl : public PropertyControl
	{
	public:
		~PropertyFontSourceControl() override;

	protected:
		void updateCaption() override;
		void updateProperty() override;
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		size_t getComboIndex(const MyGUI::UString& _name);

		void fillResources();

	private:
		MyGUI::TextBox* mName{nullptr};
		MyGUI::ComboBox* mComboBox{nullptr};

		MyGUI::VectorString mResources;
	};

}

#endif

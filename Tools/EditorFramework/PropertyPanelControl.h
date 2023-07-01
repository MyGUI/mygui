/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _a90345b7_27c4_4613_9148_281c8f996ded_
#define _a90345b7_27c4_4613_9148_281c8f996ded_

#include "Control.h"
#include "sigslot.h"
#include "Data.h"
#include "PropertyControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL PropertyPanelControl : public Control, public sigslot::has_slots<>
	{
	public:
		~PropertyPanelControl() override;

		void setCurrentData(DataPtr _data);

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void notifyChangeCoord(MyGUI::Widget* _sender);

		void HideControls();
		void InitialiseProperty(PropertyPtr _property, int& _height);

		void updateView();

	private:
		DataPtr mCurrentData{nullptr};
		using VectorPairControl = std::vector<std::pair<std::string, PropertyControl*>>;
		VectorPairControl mPropertyControls;
		int mDistance{0};
		MyGUI::ScrollView* mScrollView{nullptr};
		int mContentHeight{0};
	};

}

#endif

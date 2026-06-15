/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _075ca0ae_fb89_4108_881e_3eca688913d4_
#define _075ca0ae_fb89_4108_881e_3eca688913d4_

#include "Control.h"
#include "Data.h"
#include "ListBoxDataControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL DataListBaseControl : public Control, public sigslot::has_slots<>
	{
	public:
		void commandCreateData(const MyGUI::UString& _commandName, bool& _result);
		void commandCloneData(const MyGUI::UString& _commandName, bool& _result);
		void commandDestroyData(const MyGUI::UString& _commandName, bool& _result);
		void commandRenameData(const MyGUI::UString& _commandName, bool& _result);

		void setDataInfo(
			std::string_view _parentType,
			std::string_view _currentType,
			std::string_view _propertyName,
			std::string_view _propertyUnique);

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

		bool checkCommand(bool _result);

		const std::string& getParentType() const;
		const std::string& getCurrentType() const;
		const std::string& getPropertyForUnique() const;

	private:
		void notifyChangePosition(DataPtr _data1, DataPtr _data2);
		void notifyChangeName(DataPtr _data, std::string_view _name);

	private:
		ListBoxDataControl* mListBoxControl{nullptr};
		std::string mParentType;
		std::string mCurrentType;
		std::string mPropertyForName;
		std::string mPropertyForUnique;
	};

}

#endif

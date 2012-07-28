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

	class MYGUI_EXPORT_DLL DataListBaseControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		DataListBaseControl();
		virtual ~DataListBaseControl();

		void commandCreateData(const MyGUI::UString& _commandName, bool& _result);
		void commandCloneData(const MyGUI::UString& _commandName, bool& _result);
		void commandDestroyData(const MyGUI::UString& _commandName, bool& _result);
		void commandRenameData(const MyGUI::UString& _commandName, bool& _result);

		void setDataInfo(const std::string& _parentType, const std::string& _currentType, const std::string& _propertyName, const std::string& _propertyUnique);

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

		bool checkCommand(bool _result);

		const std::string& getParentType() const;
		const std::string& getCurrentType() const;
		const std::string& getPropertyForUnique() const;

	private:
		void notifyChangePosition(DataPtr _data1, DataPtr _data2);
		void notifyChangeName(DataPtr _data, const std::string& _name);

	private:
		ListBoxDataControl* mListBoxControl;
		std::string mParentType;
		std::string mCurrentType;
		std::string mPropertyForName;
		std::string mPropertyForUnique;
	};

}

#endif

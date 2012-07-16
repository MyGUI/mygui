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

	class DataListBaseControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		DataListBaseControl();
		virtual ~DataListBaseControl();

		void commandCreateImageData(const MyGUI::UString& _commandName, bool& _result);
		void commandDestroyImageData(const MyGUI::UString& _commandName, bool& _result);
		void commandRenameImageData(const MyGUI::UString& _commandName, bool& _result);

		void setDataInfo(const std::string& _parentType, const std::string& _currentType, const std::string& _property);

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyChangePosition(Data* _data1, Data* _data2);
		void notifyChangeName(Data* _data, const std::string& _name);

		bool checkCommand(bool _result);

	private:
		size_t mNameIndex;
		ListBoxDataControl* mListBoxControl;
		std::string mParentType;
		std::string mCurrentType;
		std::string mPropertyForName;
	};

}

#endif

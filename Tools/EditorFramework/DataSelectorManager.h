/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _c34497f7_46b0_401d_aad3_78a03a236b98_
#define _c34497f7_46b0_401d_aad3_78a03a236b98_

#include <string>
#include "sigslot.h"
#include "Data.h"
#include <map>

namespace tools
{

	class MYGUI_EXPORT_DLL DataSelectorManager
	{
	public:
		DataSelectorManager();
		~DataSelectorManager();

		static DataSelectorManager& getInstance();
		static DataSelectorManager* getInstancePtr();

		void initialise();
		void shutdown();

		typedef sigslot::signal2<DataPtr, bool> EventType;
		EventType* getEvent(const std::string& _dataType);

		// у родителя полностью изменились дети
		void changeParent(DataPtr _parent);

		// у родителя изменился только активный ребенок
		void changeParentSelection(DataPtr _parent, DataPtr _selectedChild);

	private:
		void clear();
		void onChangeData(DataPtr _parent, DataTypePtr _type, bool _changeOnlySelection);

	private:
		static DataSelectorManager* mInstance;
		typedef std::map<std::string, EventType*> MapEvent;
		MapEvent mEvents;
	};

}

#endif

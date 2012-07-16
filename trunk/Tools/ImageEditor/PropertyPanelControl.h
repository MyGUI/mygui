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
#include "DataType.h"
#include "PropertyControl.h"

namespace tools
{

	class PropertyPanelControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		PropertyPanelControl();
		virtual ~PropertyPanelControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyChangeScope(const std::string& _scope);
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);

		void HideControls();
		void InitialiseProperty(Property* _property);

	private:
		DataType* mParentType;
		Data* mCurrentData;
		typedef std::vector<std::pair<std::string, PropertyControl*>> VectorPairControl;
		VectorPairControl mPropertyControls;
	};

}

#endif

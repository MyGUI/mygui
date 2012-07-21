/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _a99c0dbc_4565_4eec_82c2_fc8ed1aeb746_
#define _a99c0dbc_4565_4eec_82c2_fc8ed1aeb746_

#include "IControlController.h"
#include "PropertyPanelControl.h"
#include "DataType.h"
#include "Data.h"

namespace tools
{
	class PropertyPanelController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		PropertyPanelController();
		virtual ~PropertyPanelController();

		virtual void setTarget(Control* _control);

		virtual void activate();
		virtual void deactivate();

	private:
		void notifyChangeScope(const std::string& _scope);
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);

	private:
		PropertyPanelControl* mControl;
		DataType* mParentType;
	};
}

#endif

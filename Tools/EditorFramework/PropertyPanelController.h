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

	class MYGUI_EXPORT_DLL PropertyPanelController : public IControlController, public sigslot::has_slots<>
	{
	public:
		void setTarget(Control* _control) override;

		void activate() override;
		void deactivate() override;

	private:
		void notifyChangeScope(std::string_view _scope);
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);

	private:
		PropertyPanelControl* mControl{nullptr};
		DataTypePtr mParentType{nullptr};
	};

}

#endif

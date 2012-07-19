/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _53f75d16_750d_44cf_aad5_1b71baecd3fb_
#define _53f75d16_750d_44cf_aad5_1b71baecd3fb_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{
	class GroupTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		GroupTextureController();
		virtual ~GroupTextureController();

		virtual void setTarget(Control* _control);

		virtual void activate();
		virtual void diactivate();

	private:
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);
		void notifyChangeProperty(Property* _sender);
		void notifyChangeValue(const std::string& _value);

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		Data* mParentData;
	};
}

#endif

/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _80163c7b_2ec7_42d3_b7e7_cd1a29d88865_
#define _80163c7b_2ec7_42d3_b7e7_cd1a29d88865_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{

	class SkinTextureController : public IControlController, public sigslot::has_slots<>
	{
	public:
		void setTarget(Control* _control) override;

		void activate() override;
		void deactivate() override;

	private:
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeValue(std::string_view _value);
		void notifyChangeScope(std::string_view _scope);

		void updateCoords(std::string_view _value);
		void updateTexture(std::string_view _value);

	private:
		ScopeTextureControl* mControl{nullptr};
		std::string mParentTypeName;
		std::string mScopeName;
		DataPtr mParentData{nullptr};
		bool mActivated{false};
	};

}

#endif

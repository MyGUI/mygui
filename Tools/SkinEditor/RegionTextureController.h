/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _b0759515_f3d3_45c1_9d96_a248d1b005c7_
#define _b0759515_f3d3_45c1_9d96_a248d1b005c7_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{

	class RegionTextureController : public IControlController, public sigslot::has_slots<>
	{
	public:
		void setTarget(Control* _control) override;

		void activate() override;
		void deactivate() override;

	private:
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeScope(std::string_view _scope);
		void notifyChangeValue(std::string_view _value);

		void updateCoords(std::string_view _value);
		void updateFrames();

	private:
		ScopeTextureControl* mControl{nullptr};
		std::string mParentTypeName;
		std::string mScopeName;
		std::string mThisType;
		DataPtr mParentData{nullptr};
		bool mActivated{false};
		MyGUI::IntCoord mTextureCoord;
		ScopeTextureControl::VectorCoord mFrames;
	};

}

#endif

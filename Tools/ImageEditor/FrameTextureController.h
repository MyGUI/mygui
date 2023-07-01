/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _b80becd5_97bb_4126_acb5_348f9b926682_
#define _b80becd5_97bb_4126_acb5_348f9b926682_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{

	class FrameTextureController : public IControlController, public sigslot::has_slots<>
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
		DataPtr mParentData{nullptr};
		bool mActivated{false};
		MyGUI::IntSize mSize;
		ScopeTextureControl::VectorCoord mFrames;
	};

}

#endif

/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _bad79f35_bcf4_4607_96e7_eea5305adef3_
#define _bad79f35_bcf4_4607_96e7_eea5305adef3_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{

	class StateTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		StateTextureController();

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
		void updateTexture(std::string_view _value);

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		std::string mScopeName;
		std::string mThisType;
		DataPtr mParentData;
		bool mActivated;
		MyGUI::IntSize mSize;
		ScopeTextureControl::VectorCoord mFrames;
	};

}

#endif

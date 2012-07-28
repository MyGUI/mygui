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
		virtual ~StateTextureController();

		virtual void setTarget(Control* _control);

		virtual void activate();
		virtual void deactivate();

	private:
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeScope(const std::string& _scope);
		void notifyChangeValue(const std::string& _value);

		void updateCoords(const std::string& _value);
		void updateFrames();
		void updateTexture(const std::string& _value);

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

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

	class IndexTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		IndexTextureController();
		virtual ~IndexTextureController();

		virtual void setTarget(Control* _control);

		virtual void activate();
		virtual void deactivate();

	private:
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeScope(const std::string& _scope);

		void updateCoords(const std::string& _value);
		void updateFrames();

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		std::string mScopeName;
		DataPtr mParentData;
		bool mActivated;
		MyGUI::IntSize mSize;
		ScopeTextureControl::VectorCoord mFrames;
	};

}

#endif

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

	class RegionTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		RegionTextureController();
		virtual ~RegionTextureController();

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

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		std::string mScopeName;
		std::string mThisType;
		DataPtr mParentData;
		bool mActivated;
		MyGUI::IntCoord mTextureCoord;
		ScopeTextureControl::VectorCoord mFrames;
	};

}

#endif

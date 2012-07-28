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

	class SkinTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		SkinTextureController();
		virtual ~SkinTextureController();

		virtual void setTarget(Control* _control);

		virtual void activate();
		virtual void deactivate();

	private:
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeValue(const std::string& _value);
		void notifyChangeScope(const std::string& _scope);

		void updateCoords(const std::string& _value);
		void updateTexture(const std::string& _value);

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		std::string mScopeName;
		DataPtr mParentData;
		bool mActivated;
	};

}

#endif

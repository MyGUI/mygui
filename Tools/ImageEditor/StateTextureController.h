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
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);
		void notifyChangeProperty(Property* _sender);
		void notifyChangeScope(const std::string& _scope);
		void notifyChangeValue(const std::string& _value);

		void updateCoords(const std::string& _value);
		void updateFrames();

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		std::string mScopeName;
		Data* mParentData;
		bool mActivated;
		MyGUI::IntSize mSize;
		typedef std::vector<MyGUI::IntCoord> VectorCoord;
		VectorCoord mFrames;
	};

}

#endif

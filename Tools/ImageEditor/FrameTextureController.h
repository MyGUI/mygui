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

	class FrameTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		FrameTextureController();
		virtual ~FrameTextureController();

		virtual void setTarget(Control* _control);

		virtual void activate();
		virtual void deactivate();

	private:
		void notifyChangeDataSelector(Data* _data, bool _changeOnlySelection);
		void notifyChangeProperty(Property* _sender);
		void notifyChangeScope(const std::string& _scope);

		void updateCoords(const std::string& _value);
		void updateFrames();

	private:
		ScopeTextureControl* mControl;
		std::string mParentTypeName;
		std::string mTypeName;
		Data* mParentData;
		bool mActivated;
		MyGUI::IntSize mSize;
		typedef std::vector<MyGUI::IntCoord> VectorCoord;
		VectorCoord mFrames;
	};

}

#endif

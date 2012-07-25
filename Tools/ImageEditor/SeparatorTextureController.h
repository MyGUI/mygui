/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _581b514e_0bf3_4414_af1a_9c91ab524119_
#define _581b514e_0bf3_4414_af1a_9c91ab524119_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{

	class SeparatorTextureController :
		public IControlController,
		public sigslot::has_slots<>
	{
	public:
		SeparatorTextureController();
		virtual ~SeparatorTextureController();

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

		ScopeTextureControl::SelectorType getTypeByName(const std::string& _name);
		MyGUI::IntCoord getCoordByName(const std::string& _name, int _offset);
		int getOffsetByName(const MyGUI::IntCoord& _coord, const std::string& _name);

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

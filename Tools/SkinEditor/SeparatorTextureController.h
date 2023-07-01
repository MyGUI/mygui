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

	class SeparatorTextureController : public IControlController, public sigslot::has_slots<>
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

		ScopeTextureControl::SelectorType getTypeByName(std::string_view _name);
		MyGUI::IntCoord getCoordByName(std::string_view _name, int _offset) const;
		int getOffsetByName(const MyGUI::IntCoord& _coord, std::string_view _name) const;

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

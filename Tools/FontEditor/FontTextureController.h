/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _13d1012b_5951_4566_a159_d955696bffc7_
#define _13d1012b_5951_4566_a159_d955696bffc7_

#include "IControlController.h"
#include "ScopeTextureControl.h"

namespace tools
{

	class FontTextureController : public IControlController, public sigslot::has_slots<>
	{
	public:
		void setTarget(Control* _control) override;

		void activate() override;
		void deactivate() override;

	private:
		void notifyChangeDataSelector(DataPtr _data, bool _changeOnlySelection);
		//		void notifyChangeProperty(PropertyPtr _sender);
		void notifyChangeScope(std::string_view _scope);

		void updateTexture(std::string_view _value);

		void commandGenerateFont(const MyGUI::UString& _commandName, bool& _result);

		void updateResultPropery(DataPtr _data);

	private:
		ScopeTextureControl* mControl{nullptr};
		std::string mParentTypeName;
		std::string mScopeName;
		DataPtr mParentData{nullptr};
		bool mActivated{false};
	};

}

#endif

/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _be3dabca_96f8_4535_95eb_34f2abc3015b_
#define _be3dabca_96f8_4535_95eb_34f2abc3015b_

#include "Control.h"
#include "ScopeTextureControl.h"
#include "IControlController.h"
#include "sigslot.h"

namespace tools
{

	class TextureViewToolControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		TextureViewToolControl();
		virtual ~TextureViewToolControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void notifyChangeScope(const std::string& _scope);

	private:
		ScopeTextureControl* mTextureTool;
		MyGUI::Widget* mPlace;
		IControlController* mGroupController;
		bool mActivated;
	};

}

#endif

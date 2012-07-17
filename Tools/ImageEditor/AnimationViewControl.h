/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _75ac36ab_9bfe_4e08_bcc0_8f0ad272ac6c_
#define _75ac36ab_9bfe_4e08_bcc0_8f0ad272ac6c_

#include "Control.h"

namespace tools
{

	class AnimationViewControl :
		public Control
	{
	public:
		AnimationViewControl();
		virtual ~AnimationViewControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
	};

}

#endif

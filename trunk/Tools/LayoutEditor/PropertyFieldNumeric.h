/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _3b7fb435_8d6d_40e0_b203_9002be7048e3_
#define _3b7fb435_8d6d_40e0_b203_9002be7048e3_

#include "EditorToolTip.h"
#include "PropertyFieldEditBox.h"

namespace tools
{

	class PropertyFieldNumeric :
		public PropertyFieldEditBox
	{
	public:
		PropertyFieldNumeric(MyGUI::Widget* _parent);
		virtual ~PropertyFieldNumeric();

	protected:
		virtual bool onCheckValue();
	};

}

#endif

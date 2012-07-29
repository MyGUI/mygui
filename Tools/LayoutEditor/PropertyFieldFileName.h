/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _6a9dce6a_9108_40b2_9836_f45702d6e835_
#define _6a9dce6a_9108_40b2_9836_f45702d6e835_

#include "EditorToolTip.h"
#include "PropertyFieldEditBox.h"

namespace tools
{

	class PropertyFieldFileName :
		public PropertyFieldEditBox
	{
	public:
		PropertyFieldFileName(MyGUI::Widget* _parent);
		virtual ~PropertyFieldFileName();

	protected:
		virtual bool onCheckValue();
	};

}

#endif

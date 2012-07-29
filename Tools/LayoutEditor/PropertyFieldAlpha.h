/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _2cac608a_8e33_486a_b195_4f8735950207_
#define _2cac608a_8e33_486a_b195_4f8735950207_

#include "EditorToolTip.h"
#include "PropertyFieldEditBox.h"

namespace tools
{

	class PropertyFieldAlpha :
		public PropertyFieldEditBox
	{
	public:
		PropertyFieldAlpha(MyGUI::Widget* _parent);
		virtual ~PropertyFieldAlpha();

	protected:
		virtual bool onCheckValue();
	};

}

#endif

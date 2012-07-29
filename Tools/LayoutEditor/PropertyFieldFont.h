/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _64078f7a_8033_4147_ac77_9f1c61bc9087_
#define _64078f7a_8033_4147_ac77_9f1c61bc9087_

#include "EditorToolTip.h"
#include "PropertyFieldComboBox.h"

namespace tools
{

	class PropertyFieldFont :
		public PropertyFieldComboBox
	{
	public:
		PropertyFieldFont(MyGUI::Widget* _parent);
		virtual ~PropertyFieldFont();

	protected:
		virtual void onFillValues();
	};

}

#endif

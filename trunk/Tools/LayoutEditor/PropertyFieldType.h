/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _b79d1db2_c5f8_4f31_a1d9_346610d012f5_
#define _b79d1db2_c5f8_4f31_a1d9_346610d012f5_

#include "EditorToolTip.h"
#include "PropertyFieldComboBox.h"

namespace tools
{

	class PropertyFieldType :
		public PropertyFieldComboBox
	{
	public:
		PropertyFieldType(MyGUI::Widget* _parent);
		virtual ~PropertyFieldType();

	protected:
		virtual void onFillValues();
	};

}

#endif

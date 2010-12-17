/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_FONT_H__
#define __PROPERTY_FIELD_FONT_H__

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

} // namespace tools

#endif // __PROPERTY_FIELD_FONT_H__

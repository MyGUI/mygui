/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_NUMERIC_H__
#define __PROPERTY_FIELD_NUMERIC_H__

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

} // namespace tools

#endif // __PROPERTY_FIELD_NUMERIC_H__

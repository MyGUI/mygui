/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_FILE_NAME_H__
#define __PROPERTY_FIELD_FILE_NAME_H__

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

} // namespace tools

#endif // __PROPERTY_FIELD_FILE_NAME_H__

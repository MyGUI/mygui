/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_ALPHA_H__
#define __PROPERTY_FIELD_ALPHA_H__

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

} // namespace tools

#endif // __PROPERTY_FIELD_ALPHA_H__

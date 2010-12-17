/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_POSITION_H__
#define __PROPERTY_FIELD_POSITION_H__

#include "EditorToolTip.h"
#include "PropertyFieldEditBox.h"

namespace tools
{
	class PropertyFieldPosition :
		public PropertyFieldEditBox
	{
	public:
		PropertyFieldPosition(MyGUI::Widget* _parent);
		virtual ~PropertyFieldPosition();

	protected:
		virtual bool onCheckValue();
		virtual void onAction(const std::string& _value, bool _force);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_POSITION_H__

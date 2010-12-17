/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_CONTROLLER_H__
#define __PROPERTY_FIELD_CONTROLLER_H__

#include "EditorToolTip.h"
#include "PropertyFieldEditBox.h"

namespace tools
{
	class PropertyFieldController :
		public PropertyFieldEditBox
	{
	public:
		PropertyFieldController(MyGUI::Widget* _parent);
		virtual ~PropertyFieldController();

	private:
		std::string splitString(std::string& str, char separator);

	protected:
		virtual void onAction(const std::string& _value, bool _force);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_CONTROLLER_H__

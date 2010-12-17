/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_LAYER_H__
#define __PROPERTY_FIELD_LAYER_H__

#include "EditorToolTip.h"
#include "PropertyFieldComboBox.h"

namespace tools
{
	class PropertyFieldLayer :
		public PropertyFieldComboBox
	{
	public:
		PropertyFieldLayer(MyGUI::Widget* _parent);
		virtual ~PropertyFieldLayer();

	protected:
		virtual void onAction(const std::string& _value);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_LAYER_H__

/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/
#ifndef __PROPERTY_FIELD_SKIN_H__
#define __PROPERTY_FIELD_SKIN_H__

#include "EditorToolTip.h"
#include "PropertyFieldComboBox.h"

namespace tools
{
	class PropertyFieldSkin :
		public PropertyFieldComboBox
	{
	public:
		PropertyFieldSkin(MyGUI::Widget* _parent);
		virtual ~PropertyFieldSkin();

	protected:
		virtual void onFillValues();
		virtual void onToolTip(const MyGUI::ToolTipInfo& _info);

	private:
		SkinInfo getCellData(size_t _index);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_SKIN_H__

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

	private:
		void notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info);

		SkinInfo getCellData(size_t _index);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_SKIN_H__

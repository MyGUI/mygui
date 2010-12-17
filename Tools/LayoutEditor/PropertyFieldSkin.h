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
		PropertyFieldSkin();
		virtual ~PropertyFieldSkin();

	protected:
		virtual void onFillValues();
		virtual void onAction(const std::string& _value);
		virtual void onToolTip(const MyGUI::ToolTipInfo& _info);

	private:
		SkinInfo getCellData(size_t _index);
		bool isSkinExist(const std::string& _skinName);
		bool checkTemplate(const std::string& _skinName);
	};

} // namespace tools

#endif // __PROPERTY_FIELD_SKIN_H__

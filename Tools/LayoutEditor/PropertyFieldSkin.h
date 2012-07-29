/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _c55048a7_d765_48b3_832e_0966d2307a68_
#define _c55048a7_d765_48b3_832e_0966d2307a68_

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

}

#endif

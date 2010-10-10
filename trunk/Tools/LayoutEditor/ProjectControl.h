/*!
	@file
	@author		Albert Semenov
	@date		10/2010
*/
#ifndef __PEOJECT_CONTROL_H__
#define __PEOJECT_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "EditorToolTip.h"

namespace tools
{
	class ProjectControl :
		public wraps::BaseLayout
	{
	public:
		ProjectControl(MyGUI::Widget* _parent = nullptr);
		virtual ~ProjectControl();
	};

} // namespace tools

#endif // __PEOJECT_CONTROL_H__

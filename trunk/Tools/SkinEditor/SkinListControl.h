/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_LIST_CONTROL_H__
#define __SKIN_LIST_CONTROL_H__

#include "BaseManager.h"
#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class SkinListControl :
		public wraps::BaseLayout
	{
	public:
		SkinListControl(MyGUI::Widget* _parent);
		virtual ~SkinListControl();
	};

} // namespace tools

#endif // __SKIN_LIST_CONTROL_H__

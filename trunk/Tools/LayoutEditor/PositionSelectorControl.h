/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __POSITION_SELECTOR_CONTROL_H__
#define __POSITION_SELECTOR_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class PositionSelectorControl :
		public wraps::BaseLayout
	{
	public:
		PositionSelectorControl(MyGUI::Widget* _parent = nullptr);
		virtual ~PositionSelectorControl();

		void setVisible(bool _value);
		void setCoord(const MyGUI::IntCoord& _value);
	};

} // namespace tools

#endif // __POSITION_SELECTOR_CONTROL_H__

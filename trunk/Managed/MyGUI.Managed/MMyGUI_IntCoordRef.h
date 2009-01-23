/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"
#include "MMyGUI_IntCoord.h"

namespace MMyGUI
{

	public value struct IntCoordRef
	{
		IntCoordRef(MyGUI::IntCoord& _value)
		{
			m_value = ((MyGUI::IntCoord*)(&(_value)));
		}

		void setValue(IntCoord _value) { *m_value = _value; }

	private:
		MyGUI::IntCoord * m_value;
	};

} // namespace MMyGUI

/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>
#include "MMyGUI_Macros.h"

namespace MMyGUI
{

	public value struct BoolRef
	{
		BoolRef(bool& _value) : m_value((bool*)_value) { }

		void setValue(bool _value) { *m_value = _value; }

	private:
		bool * m_value;
	};

} // namespace MMyGUI

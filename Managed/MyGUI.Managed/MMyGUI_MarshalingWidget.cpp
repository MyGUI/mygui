/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "MMyGUI_MarshalingWidget.h"
#include "MMyGUI_WidgetHolder.h"

#include "Generate/MMyGUI_MarshalingIncludeWidget.h"

namespace MyGUI
{
	namespace Managed
	{

		#include "Generate/MMyGUI_MarshalingWidget.cpp"

	} // namespace Managed
} // namespace MyGUI

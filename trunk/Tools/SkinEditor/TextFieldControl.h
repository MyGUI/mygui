/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXT_FIELD_CONTROL_H__
#define __TEXT_FIELD_CONTROL_H__

#include "BaseManager.h"
#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class TextFieldControl :
		public wraps::BaseLayout
	{
	public:
		TextFieldControl();
		virtual ~TextFieldControl();
	};

} // namespace tools

#endif // __TEXT_FIELD_CONTROL_H__

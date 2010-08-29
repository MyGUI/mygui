/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __BACKGROUND_CONTROL_H__
#define __BACKGROUND_CONTROL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class BackgroundControl :
		public wraps::BaseLayout
	{
	public:
		BackgroundControl();
		virtual ~BackgroundControl();
	};

} // namespace tools

#endif // __BACKGROUND_CONTROL_H__

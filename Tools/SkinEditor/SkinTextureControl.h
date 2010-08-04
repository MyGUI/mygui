/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_TEXTURE_CONTROL_H__
#define __SKIN_TEXTURE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class SkinTextureControl :
		public wraps::BaseLayout
	{
	public:
		SkinTextureControl(MyGUI::Widget* _parent);
		virtual ~SkinTextureControl();

	};

} // namespace tools

#endif // __SKIN_TEXTURE_CONTROL_H__

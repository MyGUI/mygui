/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_CONTROL_H__
#define __SKIN_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "SkinListControl.h"
#include "SkinPropertyControl.h"
#include "SkinTextureControl.h"

namespace tools
{

	class SkinControl :
		public wraps::BaseLayout
	{
	public:
		SkinControl(MyGUI::Widget* _parent);
		virtual ~SkinControl();

	private:
		SkinListControl* mSkinListControl;
		SkinPropertyControl* mSkinPropertyControl;
		SkinTextureControl* mSkinTextureControl;
	};

} // namespace tools

#endif // __SKIN_CONTROL_H__

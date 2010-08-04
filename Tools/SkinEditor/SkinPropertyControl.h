/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_PROPERTY_CONTROL_H__
#define __SKIN_PROPERTY_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{

	class SkinPropertyControl :
		public wraps::BaseLayout
	{
	public:
		SkinPropertyControl(MyGUI::Widget* _parent);
		virtual ~SkinPropertyControl();

	private:
		void fillTextures();

	private:
		MyGUI::ComboBox* mTextures;
	};

} // namespace tools

#endif // __SKIN_PROPERTY_CONTROL_H__

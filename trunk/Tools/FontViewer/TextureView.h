/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __TEXTURE_VIEW_H__
#define __TEXTURE_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class TextureView :
		public wraps::BaseLayout
	{
	public:
		TextureView();

		void setFontName(const std::string& _value);

	private:
		MyGUI::Window* mWindow;
		MyGUI::ScrollView* mScrollView;
		MyGUI::Widget* mImageBack;
		MyGUI::ImageBox* mImageTexture;
	};

} // namespace demo

#endif // __TEXTURE_VIEW_H__

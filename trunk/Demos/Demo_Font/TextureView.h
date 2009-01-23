/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __TEXTURE_VIEW_H__
#define __TEXTURE_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class TextureView : public wraps::BaseLayout
	{
	public:
		TextureView();

		void setFontName(const std::string & _font);

	private:
		MyGUI::ScrollViewPtr mScrollView;
		MyGUI::StaticImagePtr mImageBack;
		MyGUI::StaticImagePtr mImageTexture;
	};

} // namespace demo

#endif // __TEXTURE_VIEW_H__

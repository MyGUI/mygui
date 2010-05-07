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

		void setFontName(const std::string& _value);

	private:
		MyGUI::ScrollView* mScrollView;
		MyGUI::StaticImage* mImageBack;
		MyGUI::StaticImage* mImageTexture;
	};

} // namespace demo

#endif // __TEXTURE_VIEW_H__

/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __FONT_VIEW_H__
#define __FONT_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class FontView :
		public wraps::BaseLayout
	{
	public:
		FontView();

		void setFontName(const std::string& _value);

	private:
		MyGUI::Window* mWindow;
		MyGUI::EditBox* mEditView;
	};

} // namespace demo

#endif // __FONT_VIEW_H__

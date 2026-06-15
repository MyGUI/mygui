/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
*/

#ifndef _5482c1d8_6662_4c64_b13b_1781d34ea0b5_
#define _5482c1d8_6662_4c64_b13b_1781d34ea0b5_

#include "BaseLayout/BaseLayout.h"
#include "SkinInfo.h"

namespace tools
{

	class EditorToolTip : public wraps::BaseLayout
	{
		MYGUI_SINGLETON_DECLARATION(EditorToolTip);

	public:
		EditorToolTip();

		void initialise();
		void shutdown();

		void show(const SkinInfo& _data);
		void hide();
		void move(const MyGUI::IntPoint& _point);

	private:
		void setPosition(const MyGUI::IntPoint& _point);

	private:
		MyGUI::EditBox* mText{nullptr};
		MyGUI::Widget* mLastWidget{nullptr};

		int mMinWidth{0};
		int mMinHeight{0};
	};

}

#endif

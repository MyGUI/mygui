/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _da9a7d14_7a9f_4612_a33d_a5d0fb74c488_
#define _da9a7d14_7a9f_4612_a33d_a5d0fb74c488_

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"

namespace tools
{

	class MYGUI_EXPORT_DLL TextureBrowseCell :
		public wraps::BaseCellView<std::string>
	{
	public:
		TextureBrowseCell(MyGUI::Widget* _parent);

		void update(const MyGUI::IBDrawItemInfo& _info, std::string _data);
		static void getCellDimension(MyGUI::Widget* _sender, MyGUI::IntCoord& _coord, bool _drop);

	private:
		MyGUI::TextBox* mTextureName;
		MyGUI::Widget* mSelector;
		MyGUI::Widget* mBack;
		MyGUI::Widget* mParentBack;
		MyGUI::ImageBox* mImage;
	};

}

#endif

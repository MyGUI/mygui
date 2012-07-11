/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __TEXTURE_BROWSE_CELL_H__
#define __TEXTURE_BROWSE_CELL_H__

#include <MyGUI.h>
#include "ItemBox/BaseCellView.h"

namespace tools
{
	class TextureBrowseCell :
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

} // namespace tools

#endif // __TEXTURE_BROWSE_CELL_H__

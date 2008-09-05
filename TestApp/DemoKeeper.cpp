/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{

		//using namespace MyGUI;
		const MyGUI::IntSize & view = MyGUI::Gui::getInstance().getViewSize();
		const MyGUI::IntSize size(800, 800);

		base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
		base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

		//MyGUI::Gui::getInstance().load("test.font");

		createFont(80, 8);
		MyGUI::FontManager::getInstance().saveFontTexture("font_tmp", "font.png");

		MyGUI::EditPtr edit = MyGUI::Gui::getInstance().createWidget<MyGUI::Edit>("Memo", MyGUI::IntCoord(100, 100, size.width, size.height), MyGUI::Align::Default, "Main");
		edit->setTextAlign(MyGUI::Align::Default);

		edit->setFontName("font_tmp");
		int height = static_cast<MyGUI::FontPtr>(MyGUI::FontManager::getInstance().getByName("font_tmp"))->getHeightPix();
		edit->setFontHeight(height);
		mInfo->change("font pix", MyGUI::utility::toString(height));

		edit->setCaption(L"01234567890 ABCDEFGHIJKLMNOPQRSTUVWXYZ \
			abcdefghijklmnopqrstuvwxyz \
			0 1 2 3 4 5 6 7 8 9 0 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z \
			b c d e f g h i j k l m n o p q r s t u v w x y z \
			ÀÁÂÃÄÅ¨ÆÇÈÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß \
			àáâãäå¸æçèêëìíîïðñòóôõö÷øùúûüýþÿ\
			À Á Â Ã Ä Å ¨ Æ Ç È Ê Ë Ì Í Î Ï Ð Ñ Ò Ó Ô Õ Ö × Ø Ù Ú Û Ü Ý Þ ß \
			à á â ã ä å ¸ æ ç è ê ë ì í î ï ð ñ ò ó ô õ ö ÷ ø ù ú û ü ý þ ÿ");
		edit->setTextCursor(0);

		edit->setFontName("Comic.18");

		MyGUI::FontManager::getInstance().remove("font_tmp");

		createFont(80, 80);

		edit->setFontName("font_tmp");
		height = static_cast<MyGUI::FontPtr>(MyGUI::FontManager::getInstance().getByName("font_tmp"))->getHeightPix();
		edit->setFontHeight(height);
		mInfo->change("font pix", MyGUI::utility::toString(height));

	}

	void DemoKeeper::destroyScene()
	{
	}

	void DemoKeeper::createFont(int _res, int _height)
	{
		MyGUI::xml::xmlDocument document;
		document.createInfo();
		MyGUI::xml::xmlNodePtr root = document.createRoot("MyGUI");
		root->addAttributes("type", "Font");

		MyGUI::xml::xmlNodePtr node = root->createChild("Font");
		node->addAttributes("name", "font_tmp");
		node->addAttributes("default_height", "11");
		node->addAttributes("source", "MICRADI_.TTF");
		node->addAttributes("size", MyGUI::utility::toString(_height));
		node->addAttributes("resolution", MyGUI::utility::toString(_res));
		node->addAttributes("antialias_colour", "false");
		node->addAttributes("space_width", "4");
		node->addAttributes("tab_width", "8");
		node->addAttributes("cursor_width", "2");
		node->addAttributes("distance", "5");
		node->addAttributes("offset_height", "0");

		node->createChild("Code")->addAttributes("range", "33 126");
		node->createChild("Code")->addAttributes("range", "1025 1105");

		MyGUI::FontManager::getInstance()._load(root, "");

	}

} // namespace demo

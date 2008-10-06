/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
 
namespace demo
{
 
    void DemoKeeper::createScene()
    {

 
        using namespace MyGUI;
        const IntSize & view = Gui::getInstance().getViewSize();
        const IntSize size(100, 100);
 
        base::BaseManager::getInstance().addResourceLocation("../../Media/Demos/Demo_StaticImage");
        //base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

		//MyGUI::xml::xmlDocument document;
		//document.open("gavno.xml");

		//Test2 * test = new Test2();

		/*StaticImagePtr image = Gui::getInstance().createWidget<StaticImage>("StaticImage", IntCoord(50, 50, 300, 300), Align::Default, "Main");
		image->setImageTexture("core.png");
		image->setImageCoord(IntCoord(0, 0, 100, 100));*/

		/*ButtonPtr button = Gui::getInstance().createWidget<Button>("Button", IntCoord(50, 50, 300, 300), Align::Default, "Main");

		button->createWidget<Button>("Button", IntCoord(50, 50, 50, 50), Align::Default);
		button->createWidget<Button>("Button", IntCoord(70, 70, 50, 50), Align::Default);*/
 
        /*widget = Gui::getInstance().createWidget<Widget>("RawRect", IntCoord(50, 50, 300, 300), Align::Default, "Main");
        ISubWidget * main = widget->_getSubWidgetMain();
        RawRect * row = static_cast<RawRect*>(main);
        row->setRectColour(Ogre::ColourValue::White, Ogre::ColourValue(1, 0, 1), Ogre::ColourValue::Black, Ogre::ColourValue::Black);
 
        rect = Gui::getInstance().createWidget<Widget>("RawRect", IntCoord(350, 50, 300, 300), Align::Default, "Main");

		ProgressPtr progress = Gui::getInstance().createWidget<Progress>("Progress", IntCoord(600, 600, 50, 15), Align::Default, "Main");
		progress->setProgressAutoTrack(true);*/

		/*TabPtr tab = Gui::getInstance().createWidget<Tab>("Tab", IntCoord(200, 200, 500, 500), Align::Default, "Main");
		tab->insertSheet(-1, "1");
		tab->insertSheet(-1, "2");
		tab->insertSheet(-1, "3");*/
		//progress->setProgressRange(100);
		//progress->setProgressPosition(1);

		/*MyGUI::Message::createMessage("IconInfo", "", false, MyGUI::Message::Ok | MyGUI::Message::IconInfo);
		MyGUI::Message::createMessage("IconError", "", false, MyGUI::Message::Ok | MyGUI::Message::IconError);
		MyGUI::Message::createMessage("IconWarning", "", false, MyGUI::Message::Ok | MyGUI::Message::IconWarning);
		MyGUI::Message::createMessage("IconQuest", "", false, MyGUI::Message::Ok | MyGUI::Message::IconQuest);*/
 
        //widget = win->createWidget<Widget>("TileClient", IntCoord(10, 10, 100, 100), Align::Default);
 
        /*RenderBoxPtr window = Gui::getInstance().createWidget<RenderBox>("RenderBox", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), Align::Default, "Main");
		window->injectObject("axes.mesh");
		window->setMouseRotation(true);*/
        //edit->setCaption("seig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshs seig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshs");
 
        /*EditPtr button = Gui::getInstance().createWidget<Edit>("Edit", IntCoord(100, 200, 59, 26), Align::Default, "Main");
        button->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyMouseButtonClick);
        button->setCaption("press me =)");
        ButtonPtr button2 = Gui::getInstance().createWidget<Button>("ButtonMinusPlus", IntCoord(100, 300, 18, 18), Align::Default, "Main");
        button2->eventMouseButtonClick = newDelegate(this, &DemoKeeper::notifyMouseButtonClick);
        button2->setCaption("press me =)");*/
        //button->setStateCheck(true);
        //button->setEnabled(false);
 
        //Gui::getInstance().createWidget<ComboBox>("ComboBox", IntCoord(100, 100, 200, 26), Align::Default, "Main");
 
        /*ResourceImageSet::registryType();
        Gui::getInstance().load("resource.xml");
        StaticImagePtr image = Gui::getInstance().createWidget<StaticImage>("StaticImage", IntCoord(100, 100, 100, 100), Align::Default, "Main");
        ResourceImageSet * info = static_cast<ResourceImageSet*>(ResourceManager::getInstance().getResource(Guid("{0FFECB2A-0BC4-4799-83BD-F8F98F8D2541}")));
        image->setImageInfo(info->getIndexInfo("ItemBox", "normal"));
        //MyGUI::Gui::getInstance().load("test.font");
        createFont(80, 8);
        //MyGUI::FontManager::getInstance().saveFontTexture("font_tmp", "font.png");
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
        mInfo->change("font pix", MyGUI::utility::toString(height));*/
 
    }
 
    void DemoKeeper::destroyScene()
    {
    }
 
} // namespace demo 
 


  
/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_ResourceManager.h"
 
namespace demo
{
 
    /*class Guid
    {
    public:
        Guid() { fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0; }
        Guid( Guid const & _value ) { *this = _value; }
        explicit Guid(const std::string& _value) { *this = parse(_value); }
        
        inline bool operator == (Guid const & _comp) const
        {
            return _comp.fast._data1 == fast._data1
                && _comp.fast._data2 == fast._data2
                && _comp.fast._data3 == fast._data3
                && _comp.fast._data4 == fast._data4;
        }
        inline bool operator != ( Guid const & _comp ) const
        {
            return ! (*this == _comp);
        }
        inline Guid & operator = (Guid const & _rvalue)
        {
            fast._data1 = _rvalue.fast._data1;
            fast._data2 = _rvalue.fast._data2;
            fast._data3 = _rvalue.fast._data3;
            fast._data4 = _rvalue.fast._data4;
            return *this;
        }
        inline bool empty() const
        {
            return fast._data1 == 0
                && fast._data2 == 0
                && fast._data3 == 0
                && fast._data4 == 0;
        }
        inline void clear()
        {
            fast._data1 = fast._data2 = fast._data3 = fast._data4 = 0; 
        }
        std::string print() const;
        static Guid parse(const std::string& _value);
        static Guid generate();
        inline friend std::ostream& operator << ( std::ostream& _stream, const Guid &  _value )
        {
            _stream << _value.print();
            return _stream;
        }
        inline friend std::istream& operator >> ( std::istream& _stream, Guid &  _value )
        {
            std::string value;
            _stream >> value;
            if (_stream.fail()) _value.clear();
            else _value = Guid::parse(value);
            return _stream;
        }
    private:
        // массив для быстрой конвертации
        static const char convert_hex[64];
        union {
            struct _original { 
                MyGUI::uint32 data1;
                MyGUI::uint16 data2, data3;
                MyGUI::uint8 data4[8];
            } original;
            struct _fast { 
                MyGUI::uint32 _data1, _data2, _data3, _data4;
            } fast;
        };
    };
    const char Guid::convert_hex[64] = {0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,11,12,13,14,15,0,0,0,0,0,0,0,0,0};
    Guid Guid::parse(const std::string& _value)
    {
        Guid ret;
        size_t pos=0;
        // { ... }
        if (_value.size() == 38) pos ++;
        // ...
        else if (_value.size() != 36) {
            // error
            return ret;
        }
        for (; pos<9; ++pos) {
            ret.original.data1 <<= 4;
            ret.original.data1 += convert_hex[ ((_value[pos]) - 48) & 0x3F];
        }
        for (size_t pos=10; pos<14; ++pos) {
            ret.original.data2 <<= 4;
            ret.original.data2 += convert_hex[ ((_value[pos]) - 48) & 0x3F];
        }
        for (size_t pos=15; pos<19; ++pos) {
            ret.original.data3 <<= 4;
            ret.original.data3 += convert_hex[ ((_value[pos]) - 48) & 0x3F];
        }
        size_t num = 0;
        for (size_t pos=20; pos<24;) {
            ret.original.data4[num] = convert_hex[ ((_value[pos++]) - 48) & 0x3F] << 4;
            ret.original.data4[num++] += convert_hex[ ((_value[pos++]) - 48) & 0x3F];
        }
        for (size_t pos=25; pos<37;) {
            ret.original.data4[num] = convert_hex[ ((_value[pos++]) - 48) & 0x3F] << 4;
            ret.original.data4[num++] += convert_hex[ ((_value[pos++]) - 48) & 0x3F];
        }
        return ret;
    }
    std::string Guid::print() const
    {
        const size_t SIZE = 39;
        char buff[SIZE];
        sprintf(buff, "{%.8X-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}\0", (int)(original.data1), (int)(original.data2), (int)(original.data3),
            (int)(original.data4[0]), (int)(original.data4[1]),
            (int)(original.data4[2]), (int)(original.data4[3]), (int)(original.data4[4]), (int)(original.data4[5]), (int)(original.data4[6]), (int)(original.data4[7])
            );
        return buff;
    }
    Guid Guid::generate()
    {
        Guid ret;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        GUID guid;
        HRESULT result = CoCreateGuid(&guid);
        MYGUI_ASSERT(S_OK == result, "Error generate GUID");
        ret.original.data1 = guid.Data1;
        ret.original.data2 = guid.Data2;
        ret.original.data3 = guid.Data3;
        memcpy(ret.original.data4, guid.Data4, 8);
#else
#endif
        return ret;
    }*/
 
MyGUI::WidgetPtr widget = 0;
MyGUI::WidgetPtr rect = 0;

float GetAngle(Ogre::Vector3 _vec0, Ogre::Vector3 _vec1)
{
	return Ogre::Math::ACos(_vec0.dotProduct(_vec1) / (_vec0.length() * _vec1.length())).valueRadians();
}

Ogre::ColourValue getColour(const MyGUI::IntPoint & _point, const MyGUI::IntSize & _size, const Ogre::ColourValue & _colour)
{
	if (_point.left < 0 || _point.top < 0 || _point.left > _size.width || _point.top > _size.height) return Ogre::ColourValue::Black;

	float x = 1. * (_point.left) / _size.width;
	float y = 1. * (_point.top) / _size.height;

	if (x>1 || y>1 || x<0 || y<0) {
		return Ogre::ColourValue::Black;
	}
	else {
		if (x < (1 - y)) {

			Ogre::Vector3 vec1 = Ogre::Vector3(_point.left + _size.width, 0, 0);
			Ogre::Vector3 vec2 = Ogre::Vector3(_point.left, 0, _point.top);

			// угол между гранями верха, точкой проходящей через мышку и левый верхний угол
			float angle = GetAngle(vec1, vec2);
			// правый верхний угол
			float angle2 = Ogre::Degree(45).valueRadians();

			// большая грань треугольника (гипотенуза)
			float gipatenuza = Ogre::Math::Sqrt( _size.width * _size.width + _size.width * _size.width );
			// длинна грани получившегося треугольника
			float b = ((float)widget->getWidth() * Ogre::Math::Sin(angle)) / Ogre::Math::Sin((angle + angle2));
			// длинна куска на гипотенузе
			float len1 = b / gipatenuza;

			// длинна грани получившегося треугольника
			float b2 = ((float)widget->getWidth() * Ogre::Math::Sin(angle2)) / Ogre::Math::Sin((angle + angle2));
			// длинна от верхнего до мыши
			float len2 = vec2.length() / b2;

			// цвет по гипотенузе
			Ogre::ColourValue colour1 = _colour + len1 * (Ogre::ColourValue::Black - _colour);

			// общий цвет
			Ogre::ColourValue colour = colour1 + (1 - len2) * 0.8 * (Ogre::ColourValue::White - colour1);

			return colour;
		}
		else {
			// линейный сверху вниз
			Ogre::ColourValue colour = (1 - y) * _colour;
			return colour;
		}
	}
}
 
    bool DemoKeeper::mouseMoved( const OIS::MouseEvent &arg )
    {
        return base::BaseManager::mouseMoved(arg);

		Ogre::ColourValue pickedColour = Ogre::ColourValue(1, 0, 1); // тут цвет из столбика справа
        MyGUI::ISubWidget * main = rect->_getSubWidgetMain();
        MyGUI::RawRect * row = static_cast<MyGUI::RawRect*>(main);

		if (!rect) return true;
        
        if (!widget) return true;

			const Ogre::ColourValue & value = getColour(MyGUI::IntPoint(arg.state.X.abs - widget->getLeft(), arg.state.Y.abs - widget->getTop()), widget->getSize(), Ogre::ColourValue(1, 0, 1));
			row->setRectColour(value, value, value, value);

			/*float x, y;
	        x = y = 0;
            x = 1. * (arg.state.X.abs - widget->getLeft()) / widget->getWidth();
            y = 1. * (arg.state.Y.abs - widget->getTop()) / widget->getHeight();
			//mInfo->change("x", x);
			//mInfo->change("y", y);
			if (x>1 || y>1 || x<0 || y<0) {
				row->setRectColour(Ogre::ColourValue::Black, Ogre::ColourValue::Black, Ogre::ColourValue::Black, Ogre::ColourValue::Black);
			}
			else {
				if (x < (1 - y)) {
					x = y = 0;

					Ogre::Vector3 vec1 = Ogre::Vector3(widget->getLeft() + widget->getWidth(), 0, 0);
					Ogre::Vector3 vec2 = Ogre::Vector3(arg.state.X.abs - widget->getLeft(), 0, arg.state.Y.abs - widget->getTop());

					// угол между гранями верха, точкой проходящей через мышку и левый верхний угол
					float angle = GetAngle(vec1, vec2);
					// правый верхний угол
					float angle2 = Ogre::Degree(45).valueRadians();

					// большая грань треугольника (гипотенуза)
					float gipatenuza = Ogre::Math::Sqrt( widget->getWidth() * widget->getWidth() + widget->getWidth() * widget->getWidth() );//(float)widget->getWidth();
					// длинна грани получившегося треугольника
					float b = ((float)widget->getWidth() * Ogre::Math::Sin(angle)) / Ogre::Math::Sin((angle + angle2));
					// длинна куска на гипотенузе
					float len1 = b / gipatenuza;

					// длинна грани получившегося треугольника
					float b2 = ((float)widget->getWidth() * Ogre::Math::Sin(angle2)) / Ogre::Math::Sin((angle + angle2));
					// длинна от верхнего до мыши
					float len2 = vec2.length() / b2;

					// цвет по гипотенузе
					Ogre::ColourValue colour1 = pickedColour + len1 * (Ogre::ColourValue::Black - pickedColour);

					// общий цвет
					Ogre::ColourValue colour = colour1 + (1 - len2) * 0.8 * (Ogre::ColourValue::White - colour1);

					row->setRectColour(colour, colour, colour, colour);
					return true;
				}
				else {
					// линейный сверху вниз
					Ogre::ColourValue colour = (1 - y) * (pickedColour * x + pickedColour * (1 - x));
					row->setRectColour(colour, colour, colour, colour);
				}
			}
        }*/
 
        return true;
    }
 
    void DemoKeeper::createScene()
    {

 
        using namespace MyGUI;
        const IntSize & view = Gui::getInstance().getViewSize();
        const IntSize size(100, 100);
 
        base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
        base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

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
            АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ \
            абвгдеёжзиклмнопрстуфхцчшщъыьэюя\
            А Б В Г Д Е Ё Ж З И К Л М Н О П Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я \
            а б в г д е ё ж з и к л м н о п р с т у ф х ц ч ш щ ъ ы ь э ю я");
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
 
    void DemoKeeper::createFont(int _res, int _height)
    {
		//std::auto_ptr
        /*MyGUI::xml::xmlDocument document;
        document.createInfo();
        MyGUI::xml::xmlNodePtr root = document.createRoot("MyGUI");
        root->addAttributes("type", "Font");
        MyGUI::xml::xmlNodePtr node = root->createChild("Font");
        node->addAttributes("name", "font_tmp");
        node->addAttributes("default_height", "11");
        node->addAttributes("source", "Comic.ttf");
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
        MyGUI::FontManager::getInstance()._load(root, "");*/
 
    }
 
} // namespace demo 
 


  
/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{
	PaintInfo createPaintPrimitive(const MyGUI::IntSize& _size)
	{
		PaintInfo info;
		info.vert = rand() % 2 != 0;
		info.x = rand() % _size.width;
		info.y = rand() % _size.height;
		info.len = rand() % 32;
		info.colour = Ogre::ColourValue(
			rand()%256 / float(255),
			rand()%256 / float(255),
			rand()%256 / float(255),
			rand()%256 / float(255));
		return info;
	}

	void drawPaintPrimitive(const PaintInfo& _data, MyGUI::CanvasPtr _canvas)
	{
		const MyGUI::IntSize& size = _canvas->getTextureRealSize();
		int xdelta = _data.vert ? 0 : 1;
		int ydelta = _data.vert ? 1 : 0;
		int x = _data.x;
		int y = _data.y;
		int count = _data.len;
		while (count != 0) {
			if (x < size.width && y < size.height) {
				_canvas->setPixel(x, y, _data.colour);
			}
			x += xdelta;
			y += ydelta;
			count--;
		}
	}

	void DemoKeeper::createScene()
    {
		// потемнее скин
		mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

		mCanvas1Size = 255;
		mCanvas2Size = 300;
		mCanvas3Size = 300;


		/*
		немного поправил канвас и демки, работает все както не так =) пишу как должно

		1. режим без кеширования, с растягиванием - текстура создается один раз заданного пользователем размером (с поправкой на степень двойки)
			при изменении размеров ничего не гарантируется, каждая новыя партия примитивов рисуется правильно
			т.е. во весь виджет, если рисуется не во весь, значит не корректируеются текстурные координаты текстуры
			ширина линий всегда одинаковая, при увеличении размеров, ширина всех новых и старых линий одинакова

		2. режим с кешированием, с растягиванием - текстура создается один раз заданного пользователем размером (с поправкой на степень двойки)
			при изменении размеров старое сохраняется, новое рисуется во весь виджет, размеры примитивав всегда одинаковые по толщтне
			если они меняются, значит хз че, точнее разные форматы или размеры у кеша и текстуры
			новые примитивы рисуются во весь виджет

		3. режим без кеширования, попиксельный - пиксель в пиксель при растягивании, информация может терятся, каждые новые
			примитивы рисуются во весь виджет. толщина линий всегда строго пиксель. текстура пересоздаетмя на границах перехода степень двойки
			до этого момента, правятся ткстурные координаты только.

		4. (стремный и невнятный режим) режим c кешированием, попиксельный - пиксель в пиксель при растягивании, информация не теряется, каждые новые
			примитивы рисуются во весь виджет. толщина линий всегда строго пиксель. текстура пересоздаетмя на границах перехода степень двойки
			до этого момента, правятся ткстурные координаты только.




		*/

		mCanvasFactory = new MyGUI::factory::CanvasFactory();

		// первая мета текстура
		// мы по евенту лочим и добавляем в текстуру данные и все
		// Re: без кеша
		mPanel1 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, mCanvas1Size, mCanvas1Size), MyGUI::Align::Default, "Overlapped");
		mPanel1->setCaption( Ogre::UTFString( "Const size - stretches" ) );
		mCanvas1 = mPanel1->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel1->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas1->createTexture( mCanvas1Size, mCanvas1Size, MyGUI::Canvas::TRM_PT_CONST_SIZE ); // создаём ровно то, что сказали
		mCanvas1->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture1 );
		//MyGUI::StaticImagePtr image1 = mPanel1->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, mCanvas1Size, mCanvas1Size), MyGUI::Align::Stretch);
		//image1->setImageTexture( mCanvas1->getName() );

		mPanel2 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(310, 10, mCanvas2Size, mCanvas2Size), MyGUI::Align::Default, "Overlapped");
		mPanel2->setCaption( Ogre::UTFString( "Pixel in pixel - recreates" ) );
		mCanvas2 = mPanel2->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel2->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas2->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED ); // текстура с размерами степень двойки - потому что не задали размеры
		mCanvas2->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture2 );
		

		// третья мета текстура
		// мы запоминаем примитив(ы) для рендера и говорим текстуре что данные обновились
		// а данные обновляем в методу апдейт
		// Re: кеша нет - примитивы
		mPanel3 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(610, 10, mCanvas3Size, mCanvas3Size), MyGUI::Align::Default, "Overlapped");
		mPanel3->setCaption( Ogre::UTFString( "Pixel in pixel(primitives) - recreates" ) );
		mCanvas3 = mPanel3->createWidget< MyGUI::Canvas >("Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel3->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas3->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED );
		mCanvas3->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture3 );
		
	}	

    void DemoKeeper::destroyScene()
    {
		delete mCanvasFactory;
    }

	void DemoKeeper::requestUpdateTexture1( MyGUI::CanvasPtr canvas )
    {
		//canvas->loadFromFile( "wallpaper0.jpg" );
	}

	// Load from cache
	void DemoKeeper::requestUpdateTexture2( MyGUI::CanvasPtr canvas )
    {
	}

	// Primitives used 
	void DemoKeeper::requestUpdateTexture3( MyGUI::CanvasPtr canvas )
    {
		canvas->lock();
		for (VectorPaintInfo::const_iterator iter = mPaintData.begin(); iter!=mPaintData.end(); ++iter) {
			drawPaintPrimitive(*iter, canvas);
		}
		canvas->unlock();
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		bool result = BaseManager::keyPressed(arg);

		const Ogre::Real plusSize = 20;

		if (arg.key == OIS::KC_F1) {

			mCanvas1->lock();

			size_t count = rand() % 64;
			while (count != 0) {
				drawPaintPrimitive(createPaintPrimitive(mCanvas1->getTextureRealSize()), mCanvas1);
				count --;
			}

			mCanvas1->unlock();

		}
		else if ( arg.key == OIS::KC_F2) {

			mCanvas2->lock();

			size_t count = rand() % 64;
			while (count != 0) {
				drawPaintPrimitive(createPaintPrimitive(mCanvas2->getTextureRealSize()), mCanvas2);
				count --;
			}

			mCanvas2->unlock();
			
		}
		else if ( arg.key == OIS::KC_F3) {

			size_t count = rand() % 64;
			while (count != 0) {
				mPaintData.push_back(createPaintPrimitive(mCanvas3->getTextureRealSize()));
				count --;
			}

			// и говорим что данные устарели,
			// по идее евент придет на границе кадра а не сразу
			// Re: во время рендеринга загружается :)
			// Re: время рендеринга надо экономить - сразу перезагружаю... придёт сразу.
			mCanvas3->updateTexture();

		} else if( arg.key == OIS::KC_1 ) { // бррр :)
			mCanvas1->createTexture( mCanvas1->getTextureRealWidth() + 1, mCanvas1->getTextureRealHeight() + 1, MyGUI::Canvas::TRM_PT_CONST_SIZE );
		}

		return result;
	}
	 
} // namespace demo

/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
/*
требование и мотивация:

виджет сий, будет называться Canvas и суть его отображенна в названии.
варианты использования, для внешнего программиста, всего два

1) я маленький даун, я хочу виджет в котором я могу попиксельно рисовать то что я хочу и когда захочу.
2) я хочу прикрутить нечто что рисует в текстуру примитивы или еще что либо.

соответсвенно реализация обновления двух видов:

1) есть некое кеширование, опциональное, пользователь лочит текстурку пишет туда попиксельно или как захочет и анлочит и все радуются. 
  но тут проблема в том что если не будет кеширования то все пропадет при девайс лост что он намалевал.
  для вариантов видео или рендер бокса не страшно, оно и так обновляется перфрейм.

2) есть некая библиотека, котоая управляет примитивами, ей нужен указатель на некую память и ее размеры, и все, она сама там что то делает
  и без кеширования ибо рулит примитивами. ей нужен метод апдейт, в который она и выводит все разом с нуля.
Re: а) ну и пусть метод хавает саму текстуру... то есть проостите Canvas, сама его лочит/анлочит и вообще пусть делает что хочет. Пусть она знает
    что и когда её нужно писать...
	б) 


отсюда выходит интерфейс следующего типа.
  создать виджет, указав ему только размеры, не надо напрягать пользователя деталями, т.е. никакх имен, зачем ему нужно имя текстуры?
  зачем группа зачем формат, он хочет тыкать пиксели. т.е. обязательно только размер и то не факт =)
  методы для указания нужно ли кеширование
  методы сет пиксель
  методы лок анлок
  событие апдейта

для наследования должны быть методы апдейта без евента, т.е. если я библиотека, я хочу создать текстуру
и в вирт методе апдейт все самому ресовать а не снаружи

виджет Canvas должен иметь флаги, такие как режим растягивания
1) созданная текстура не меняет размер, она расятгивается
2) созданая текстура есть пиксель в пиксель, соответвенно пересоздается как только виджет увеличился и его рамера не достаточно
   правда об этом мона подумать в производных классах

   ниже будет два варианта использования виджетов
	F1 управление первой текстурой
	F2 управление второй текстурой
*/

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
		const MyGUI::IntSize& size = _canvas->getSize();
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

		mCanvas1Size = 260;
		mCanvas2Size = 260;
		mCanvas3Size = 256;


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

		//base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		//base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		mCanvasFactory = new MyGUI::factory::CanvasFactory();

		// первая мета текстура
		// мы по евенту лочим и добавляем в текстуру данные и все
		// Re: без кеша
		mPanel1 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, mCanvas1Size, mCanvas1Size), MyGUI::Align::Default, "Overlapped");
		mCanvas1 = mPanel1->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel1->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas1->createTexture(/* mCanvas1Size, mCanvas1Size */); // создаём ровно то, что сказали
		mCanvas1->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture1 );

		//MyGUI::StaticImagePtr image1 = mPanel1->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, mCanvas1Size, mCanvas1Size), MyGUI::Align::Stretch);
		//image1->setImageTexture( mCanvas1->getName() );

		// Re: кеш
		mPanel2 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(310, 10, mCanvas2Size, mCanvas2Size), MyGUI::Align::Default, "Overlapped");
		mCanvas2 = mPanel2->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel1->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas2->setCacheUse( true );
		mCanvas2->createTexture(/* mCanvas2Size, mCanvas2Size */); // текстура с размерами степень двойки - потому что не задали размеры
		mCanvas2->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture2 );
		
		// третья мета текстура
		// мы запоминаем примитив(ы) для рендера и говорим текстуре что данные обновились
		// а данные обновляем в методу апдейт
		// Re: кеша нет - примитивы
		mPanel3 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(610, 10, mCanvas3Size, mCanvas3Size), MyGUI::Align::Default, "Overlapped");
		mCanvas3 = mPanel3->createWidget< MyGUI::Canvas >("Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel1->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas3->createTexture(/* mCanvas3Size, mCanvas3Size */);
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
		if( canvas->getCacheUse() && ! canvas->isCacheEmpty() )
		{
			// Re: по-умолчанию в левый верхний угол.. можно задать и другой бокс, куда копировать
			// - для RM_EXACT_REQUEST
			if( canvas->getResizeMode() == MyGUI::Canvas::RM_EXACT_REQUEST )
				canvas->restoreFromCache();
			else // но если честно... лучше без =) ... а нормально апдейтить текстуру
				canvas->restoreFromCacheResampled( Ogre::Image::Box( 0, 0, canvas->getTextureRealWidth(), canvas->getTextureRealHeight() ), Ogre::Image::FILTER_BILINEAR );
		}
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

			// почему то данные сбрасываются при каждом локе =/
			// Re: угу

			mCanvas1->lock();

			size_t count = rand() % 64;
			while (count != 0) {
				drawPaintPrimitive(createPaintPrimitive(mCanvas1->getSize()), mCanvas1);
				count --;
			}

			mCanvas1->unlock();

		}
		else if ( arg.key == OIS::KC_F2) {

			// Re: кеш работает - все счастливы :)

			mCanvas2->lock();

			size_t count = rand() % 64;
			while (count != 0) {
				drawPaintPrimitive(createPaintPrimitive(mCanvas2->getSize()), mCanvas2);
				count --;
			}

			mCanvas2->unlock();
			
		}
		else if ( arg.key == OIS::KC_F3) {

			size_t count = rand() % 64;
			while (count != 0) {
				mPaintData.push_back(createPaintPrimitive(mCanvas3->getSize()));
				count --;
			}

			// и говорим что данные устарели,
			// по идее евент придет на границе кадра а не сразу
			// Re: во время рендеринга загружается :)
			mCanvas3->updateTexture();
		}

		//}  else if(arg.key == OIS::KC_Q) {
		//	mCanvas1Size += plusSize;
		//	mCanvas1->setSize( mCanvas1Size, mCanvas1Size );
		//	/*mCanvas2->setPosition( mCanvas2->getPosition().left + plusSize,
		//						  mCanvas2->getPosition().top );

		//	mCanvas3->setPosition( mCanvas3->getPosition().left + plusSize,
		//						  mCanvas3->getPosition().top );*/
		//	mPanel1->setSize( mCanvas1Size, mCanvas1Size );

		//	mPanel2->setPosition( mPanel2->getPosition().left + plusSize,
		//						  mPanel2->getPosition().top );

		//	mPanel3->setPosition( mPanel3->getPosition().left + plusSize,
		//						  mPanel3->getPosition().top );
		//} else if(arg.key == OIS::KC_W) {
		//	mCanvas2Size += plusSize;
		//	mCanvas2->setSize( mCanvas2Size, mCanvas2Size );
		//	/*mCanvas3->setPosition( mCanvas3->getPosition().left + plusSize,
		//						  mCanvas3->getPosition().top );*/
		//	mPanel2->setSize( mCanvas2Size, mCanvas2Size );

		//	mPanel3->setPosition( mPanel3->getPosition().left + plusSize,
		//						  mPanel3->getPosition().top );
		//} else if(arg.key == OIS::KC_E) {
		//	mCanvas3Size += plusSize;
		//	mCanvas3->setSize( mCanvas3Size, mCanvas3Size );
		//	mPanel3->setSize( mCanvas3Size, mCanvas3Size );
		//// smaller
		//} else if(arg.key == OIS::KC_A) {
		//	mCanvas1Size -= plusSize;
		//	mCanvas1->setSize( mCanvas1Size, mCanvas1Size );

		//	/*mCanvas2->setPosition( mCanvas2->getPosition().left - plusSize,
		//						  mCanvas2->getPosition().top );

		//	mCanvas3->setPosition( mCanvas3->getPosition().left - plusSize,
		//						  mCanvas3->getPosition().top );*/
		//	mPanel1->setSize( mCanvas1Size, mCanvas1Size );

		//	mPanel2->setPosition( mPanel2->getPosition().left - plusSize,
		//						  mPanel2->getPosition().top );

		//	mPanel3->setPosition( mPanel3->getPosition().left - plusSize,
		//						  mPanel3->getPosition().top );
		//} else if(arg.key == OIS::KC_S) {
		//	mCanvas2Size -= plusSize;
		//	mCanvas2->setSize( mCanvas2Size, mCanvas2Size );

		//	/*mCanvas2->setPosition( mCanvas3->getPosition().left - plusSize,
		//						  mCanvas3->getPosition().top );*/
		//	mPanel2->setSize( mCanvas2Size, mCanvas2Size );

		//	mPanel3->setPosition( mPanel3->getPosition().left - plusSize,
		//						  mPanel3->getPosition().top );
		//} else if(arg.key == OIS::KC_D) {
		//	mCanvas3Size -= plusSize;
		//	mCanvas3->setSize( mCanvas3Size, mCanvas3Size );
		//	mPanel3->setSize( mCanvas3Size, mCanvas3Size );
		//}


		return result;
	}
	 
} // namespace demo

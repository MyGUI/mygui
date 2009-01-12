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

		mCanvasFactory = new MyGUI::factory::CanvasFactory();
		mTestRenderBoxFactory = new MyGUI::factory::TestRenderBoxFactory();

		/*MyGUI::WindowPtr wnd = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(400, 400, 400, 400), MyGUI::Align::Default, "Overlapped");
		mTestRenderBox1 = wnd->createWidget<MyGUI::TestRenderBox>( "TestRenderBox", MyGUI::IntCoord( MyGUI::IntPoint(), wnd->getClientCoord().size() ), MyGUI::Align::Stretch );
		mTestRenderBox1->setRenderTarget( mCamera );
		mTestRenderBox1->setViewScale( true );
		mSceneMgr->getRootSceneNode()->attachObject( mSceneMgr->createEntity( "axes", "axes.mesh" ) );*/

		// первая мета текстура
		// мы по евенту лочим и добавляем в текстуру данные и все
		// Re: без кеша
		mPanel1 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, mCanvas1Size, mCanvas1Size), MyGUI::Align::Default, "Overlapped");
		mPanel1->setCaption( Ogre::UTFString( "Const size - stretches" ) );
		mCanvas1 = mPanel1->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel1->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas1->createTexture( mCanvas1Size, mCanvas1Size, MyGUI::Canvas::TRM_PT_CONST_SIZE ); // создаём ровно то, что сказали
		mCanvas1->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas1 );
		//MyGUI::StaticImagePtr image1 = mPanel1->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, mCanvas1Size, mCanvas1Size), MyGUI::Align::Stretch);
		//image1->setImageTexture( mCanvas1->getName() );

		mPanel2 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(310, 10, mCanvas2Size, mCanvas2Size), MyGUI::Align::Default, "Overlapped");
		mPanel2->setCaption( Ogre::UTFString( "Pixel in pixel - recreates" ) );
		mCanvas2 = mPanel2->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel2->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas2->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED ); // текстура с размерами степень двойки - потому что не задали размеры
		//mCanvas2->loadTexture( "wallpaper0.jpg" );
		mCanvas2->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas2 );
		

		// третья мета текстура
		// мы запоминаем примитив(ы) для рендера и говорим текстуре что данные обновились
		// а данные обновляем в методу апдейт
		// Re: кеша нет - примитивы
		mPanel3 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(610, 10, mCanvas3Size, mCanvas3Size), MyGUI::Align::Default, "Overlapped");
		mPanel3->setCaption( Ogre::UTFString( "Pixel in pixel(primitives) - recreates" ) );
		mCanvas3 = mPanel3->createWidget< MyGUI::Canvas >("Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel3->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas3->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED );
		mCanvas3->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas3 );
	}	

    void DemoKeeper::destroyScene()
    {
		delete mCanvasFactory;
		delete mTestRenderBoxFactory;
    }

	void DemoKeeper::requestUpdateCanvas1( MyGUI::CanvasPtr canvas )
    {
	}

	// Load from cache
	void DemoKeeper::requestUpdateCanvas2( MyGUI::CanvasPtr canvas )
    {
	}

	// Primitives used 
	void DemoKeeper::requestUpdateCanvas3( MyGUI::CanvasPtr canvas )
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

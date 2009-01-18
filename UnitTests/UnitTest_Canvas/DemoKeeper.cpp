/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

#include "precompiled.h"
#include "DemoKeeper.h"

#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"

#include "agg_scanline_u.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_pixfmt_rgb.h"
#include "agg_path_storage.h"
#include "agg_curves.h"
#include "agg_conv_stroke.h"

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

	bool DemoKeeper::frameStarted( const Ogre::FrameEvent & evt )
	{
		bool res = BaseManager::frameStarted( evt );

		mSceneMgr->getRootSceneNode()->getChildIterator().peekNextValue()->yaw( Ogre::Radian( 10 )* evt.timeSinceLastFrame );

		return res;
	}


	void DemoKeeper::createScene()
    {
		// потемнее скин
		//mGUI->load("core_theme_black_orange.xml");
		//mGUI->load("core_skin.xml");
		mNodeWindow1 = mNodeWindow2 = nullptr;

		mCanvas1Size = 350;
		mCanvas2Size = 300;
		mCanvas3Size = 300;

		mCanvasFactory = new MyGUI::factory::CanvasFactory();
		mTestRenderBoxFactory = new MyGUI::factory::TestRenderBoxFactory();

		MyGUI::WindowPtr wnd = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(400, 400, 400, 400), MyGUI::Align::Default, "Overlapped");
		mTestRenderBox1 = wnd->createWidget<MyGUI::TestRenderBox>( "TestRenderBox", MyGUI::IntCoord( MyGUI::IntPoint(), wnd->getClientCoord().size() ), MyGUI::Align::Stretch );
		mTestRenderBox1->setRenderTarget( mCamera );
		mTestRenderBox1->setViewScale( true );
		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject( mSceneMgr->createEntity( "axes", "axes.mesh" ) );

		mSceneMgr->getRootSceneNode()->getChildIterator().peekNextValue()->scale( Ogre::Vector3( 4 ) );


		// первая мета текстура
		// мы по евенту лочим и добавляем в текстуру данные и все
		// Re: без кеша
		mPanel1 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, mCanvas1Size, mCanvas1Size), MyGUI::Align::Default, "Overlapped");
		mPanel1->setCaption( Ogre::UTFString( "Const size - stretches" ) );
		mCanvas1 = mPanel1->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(0, 0, 256, 256), MyGUI::Align::Stretch);
		mCanvas1->createTexture( 256, 256, MyGUI::Canvas::TRM_PT_CONST_SIZE ); // создаём ровно то, что сказали
		mCanvas1->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas1 );
		//MyGUI::StaticImagePtr image1 = mPanel1->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, mCanvas1Size, mCanvas1Size), MyGUI::Align::Stretch);
		//image1->setImageTexture( mCanvas1->getName() );

		//mPanel1 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, mCanvas1Size, mCanvas1Size), MyGUI::Align::Default, "Overlapped");
		//mPanel1->setCaption( Ogre::UTFString( "Const size - stretches" ) );
		//mCanvas1 = mPanel1->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel1->getClientCoord().size()), MyGUI::Align::Stretch);
		//mCanvas1->createTexture( mCanvas1Size, mCanvas1Size, MyGUI::Canvas::TRM_PT_CONST_SIZE ); // создаём ровно то, что сказали
		//mCanvas1->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas1 );
		////MyGUI::StaticImagePtr image1 = mPanel1->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, mCanvas1Size, mCanvas1Size), MyGUI::Align::Stretch);
		////image1->setImageTexture( mCanvas1->getName() );

		//mPanel2 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(310, 10, mCanvas2Size, mCanvas2Size), MyGUI::Align::Default, "Overlapped");
		//mPanel2->setCaption( Ogre::UTFString( "Pixel in pixel - recreates" ) );
		//mCanvas2 = mPanel2->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel2->getClientCoord().size()), MyGUI::Align::Stretch);
		//mCanvas2->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED ); // текстура с размерами степень двойки - потому что не задали размеры
		////mCanvas2->loadTexture( "wallpaper0.jpg" );
		//mCanvas2->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas2 );
		

		// третья мета текстура
		// мы запоминаем примитив(ы) для рендера и говорим текстуре что данные обновились
		// а данные обновляем в методу апдейт
		// Re: кеша нет - примитивы
		mPanel3 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(410, 10, 600, mCanvas3Size), MyGUI::Align::Default, "Overlapped");
		mPanel3->setCaption( Ogre::UTFString( "Pixel in pixel(primitives) - recreates" ) );
		mCanvas3 = mPanel3->createWidget< MyGUI::Canvas >("Canvas", MyGUI::IntCoord(MyGUI::IntPoint(), mPanel3->getClientCoord().size()), MyGUI::Align::Stretch);
		mCanvas3->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED );
		mCanvas3->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas3 );
		mCanvas3->updateTexture();

		mNodeWindow1 = mPanel3->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowSmallC", MyGUI::IntCoord(100, 50, 100, 80), MyGUI::Align::Default);
		mNodeWindow1->setCaption( "Node1" );
		mNodeWindow1->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(1, 40, 9, 9), MyGUI::Align::Default);
		mNodeWindow1->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(mNodeWindow1->getWidth() - 12, 40, 9, 9), MyGUI::Align::Right | MyGUI::Align::Top);
		mNodeWindow1->eventWindowChangeCoord = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowChangeCoord);

		mNodeWindow2 = mPanel3->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowSmallC", MyGUI::IntCoord(300, 150, 100, 80), MyGUI::Align::Default);
		mNodeWindow2->setCaption( "Node2" );
		mNodeWindow2->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(1, 40, 9, 9), MyGUI::Align::Default);
		mNodeWindow2->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(mNodeWindow2->getWidth() - 12, 40, 9, 9), MyGUI::Align::Right | MyGUI::Align::Top);
		mNodeWindow2->eventWindowChangeCoord = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowChangeCoord);
		mCanvas3->updateTexture();
	}	

    void DemoKeeper::destroyScene()
    {
		delete mCanvasFactory;
		delete mTestRenderBoxFactory;
    }

	void DemoKeeper::notifyWindowChangeCoord(MyGUI::WidgetPtr _sender)
	{
		mCanvas3->updateTexture();
	}

	void DemoKeeper::requestUpdateCanvas1( MyGUI::CanvasPtr canvas, MyGUI::CanvasEvent _canvasEvent )
    {
	}

	// Load from cache
	void DemoKeeper::requestUpdateCanvas2( MyGUI::CanvasPtr canvas, MyGUI::CanvasEvent _canvasEvent )
    {
	}

	// Primitives used 
	void DemoKeeper::requestUpdateCanvas3( MyGUI::CanvasPtr canvas, MyGUI::CanvasEvent _canvasEvent )
    {
		if (_canvasEvent == MyGUI::CanvasEvent::CE_WIDGET_RESIZED) return;
		if (mNodeWindow1 == nullptr || mNodeWindow2 == nullptr) return;

		unsigned char * data = (unsigned char*)canvas->lock();

		int width = canvas->getTextureRealWidth();
		int height = canvas->getTextureRealHeight();

		const MyGUI::IntPoint& node1 = mNodeWindow1->getPosition();
		const MyGUI::IntPoint& node2 = mNodeWindow2->getPosition();
		
		struct SplineInfo
		{
			SplineInfo(int _x1, int _y1, int _x2, int _y2, float _r, float _g, float _b, bool _is_left1, bool _is_left2) :
				x1(_x1), y1(_y1), x2(_x2), y2(_y2), r(_r), g(_g), b(_b), is_left1(_is_left1), is_left2(_is_left2)
			{ }
			int x1, y1, x2, y2;
			float r, g, b;
			bool is_left1, is_left2;
		};

		SplineInfo info(node1.left + 96, node1.top + 45, node2.left + 2, node2.top + 45, 0.7, 0, 0, false, true);


        //============================================================ 
        // AGG
        agg::rendering_buffer rbuf;
        rbuf.attach((unsigned char*)data, width, height, width*4);

        // Pixel format and basic primitives renderer
        agg::pixfmt_bgra32 pixf(rbuf);
        agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

        renb.clear(agg::rgba8(1, 1, 1, 0));

        // Scanline renderer for solid filling.
        agg::renderer_scanline_aa_solid<agg::renderer_base<agg::pixfmt_bgra32> > ren(renb);

        // Rasterizer & scanline
        agg::rasterizer_scanline_aa<> ras;
        agg::scanline_p8 sl;

		// хранилище всех путей
		agg::path_storage path;

		// кривая безье которая строится по 4 точкам
		agg::curve4 curve;
		curve.approximation_method(agg::curve_approximation_method_e(agg::curve_inc)); // метод апроксимации, curve_inc - быстрый но много точек
		curve.approximation_scale(0.3); //масштаб апроксимации
		curve.angle_tolerance(agg::deg2rad(0));
		curve.cusp_limit(agg::deg2rad(0));
		curve.init(info.x1, info.y1, info.x1 + (200 * (info.is_left1 ? -1 : 1)) - 8, info.y1, info.x2 + (200 * (info.is_left2 ? -1 : 1)) - 8, info.y2, info.x2, info.y2);

		// добавляем путь безье
		path.concat_path(curve);

		// сам путь который рисуется, растерезатор
		agg::conv_stroke<agg::path_storage> stroke(path);
		stroke.width(2); // ширина линии
		stroke.line_join(agg::line_join_e(agg::bevel_join)); // хз че такое
		stroke.line_cap(agg::line_cap_e(agg::butt_cap)); //обрезка концов
		stroke.inner_join(agg::inner_join_e(agg::inner_miter)); // соединения внутри линии точек
		stroke.inner_miter_limit(1.01);

		ras.add_path(stroke);

        // Setting the attrribute (color) & Rendering
        ren.color(agg::rgba8(20, 100, 20));
        agg::render_scanlines(ras, sl, ren);
        //============================================================

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

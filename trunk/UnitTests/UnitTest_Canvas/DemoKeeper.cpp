/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

#include "precompiled.h"
#include "DemoKeeper.h"

/*#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"

#include "agg_scanline_u.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_pixfmt_rgb.h"
#include "agg_path_storage.h"
#include "agg_curves.h"
#include "agg_conv_stroke.h"*/

#include "RenderBox/RenderBoxWrap.h"

#include "GraphNodeSimple.h"

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

	void DemoKeeper::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		/*if (_id == MyGUI::MouseButton::Left)
		{
			if ( ! mIsDrug )
			{
				mIsDrug = true;

				mDrugLine.colour.set(1, 1, 1, 1);
				mDrugLine.end_offset = 0;
				mDrugLine.start_offset = _sender->getLeft() < 10 ? -1 : 1;
				mDrugLine.point_start.set(
					_sender->getAbsoluteLeft() - _sender->getParent()->getParent()->getAbsoluteLeft() + 4,
					_sender->getAbsoluteTop() - _sender->getParent()->getParent()->getAbsoluteTop() + 4
					);
				mDrugLine.point_end = mDrugLine.point_start;

				mCanvas3->updateTexture();
			}
		}*/
	}

	void DemoKeeper::notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		// сбрасываем при любой клавише
		/*if (mIsDrug)
		{
			mIsDrug = false;
			mCanvas3->updateTexture();
		}*/
	}

	void DemoKeeper::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		/*if (mIsDrug)
		{
			mDrugLine.point_end.set(_left - _sender->getParent()->getParent()->getAbsoluteLeft(), _top - _sender->getParent()->getParent()->getAbsoluteTop());

			// устанавлваем длинну загиба от дистанции
			double distance = ((mDrugLine.point_end.left - mDrugLine.point_start.left) * (mDrugLine.point_end.left - mDrugLine.point_start.left)) +
				((mDrugLine.point_end.top - mDrugLine.point_start.top) * (mDrugLine.point_end.top - mDrugLine.point_start.top));
			distance = std::sqrt(distance);

			const int offset = 40;
			distance *= 0.5;
			if (distance < 1) distance = 1;
			else if (distance > offset) distance = offset;
			if (mDrugLine.start_offset < 0) mDrugLine.start_offset = distance * -1;
			else  mDrugLine.start_offset = distance;

			// пикаем виджет под нами
			MyGUI::WidgetPtr widget = MyGUI::LayerManager::getInstance().getWidgetFromPoint(_left, _top);
			if (widget != nullptr && widget->isUserString("NodeLink"))
			{
				bool accept = MyGUI::utility::parseBool(widget->getUserString("NodeLink"));
				if (accept)
					mDrugLine.colour = MyGUI::Colour::Green;
				else
					mDrugLine.colour = MyGUI::Colour::Red;
			}
			else 
			{
				mDrugLine.colour = MyGUI::Colour::White;
			}


			mCanvas3->updateTexture();
		}*/
	}

	void DemoKeeper::createScene()
    {
		mGraphView = 0;
		// потемнее скин
		//mGUI->load("core_theme_black_orange.xml");
		//mGUI->load("core_skin.xml");
		//mNodeWindow1 = mNodeWindow2 = mNodeWindow3 = nullptr;
		//mIsDrug = false;

		mCanvas1Size = 350;
		mCanvas2Size = 300;
		mCanvas3Size = 300;

		//mCanvasFactory = new MyGUI::factory::CanvasFactory();
		//mTestRenderBoxFactory = new MyGUI::factory::TestRenderBoxFactory();
		MyGUI::EditPtr edit = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(40, 600, 200, 50), MyGUI::Align::Default, "Overlapped");
		edit->setEditMultiLine(true);
		edit->setOverflowToTheLeft(true);
		edit->setMaxTextLength(60);
		edit->addText("1234jkdfhgkjahgsdkjfagsdkjfgakjdhgkfajhgsdkjsdhgkajgksdjjsdghf");
		edit->setTextIntervalColour(2, 10, MyGUI::Colour::Red);

		MyGUI::WindowPtr wnd = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(400, 400, 400, 400), MyGUI::Align::Default, "Overlapped");
		mTestRenderBox1 = wnd->createWidget<MyGUI::RenderBox>( "TestRenderBox", MyGUI::IntCoord( MyGUI::IntPoint(), wnd->getClientCoord().size() ), MyGUI::Align::Stretch );
		//mTestRenderBox1->setCamera( mCamera );
		//mTestRenderBox1->setBackgroungColour(Ogre::ColourValue::ZERO);

		mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject( mSceneMgr->createEntity( "axes", "axes.mesh" ) );

		mSceneMgr->getRootSceneNode()->getChildIterator().peekNextValue()->scale( Ogre::Vector3( 4 ) );

		wraps::RenderBoxWrap * box = new wraps::RenderBoxWrap(mTestRenderBox1);
		box->injectObject("axes.mesh");
		box->setAutoRotation(true);
		box->setMouseRotation(true);
		box->setViewScale(true);

		// первая мета текстура
		// мы по евенту лочим и добавляем в текстуру данные и все
		// Re: без кеша
		mPanel1 = mGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, mCanvas1Size, mCanvas1Size), MyGUI::Align::Default, "Overlapped");
		mPanel1->setCaption( Ogre::UTFString( "Const size - stretches" ) );
		mCanvas1 = mPanel1->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(0, 0, 256, 256), MyGUI::Align::Stretch);
		mCanvas1->createTexture( 256, 256, MyGUI::Canvas::TRM_PT_CONST_SIZE ); // создаём ровно то, что сказали
		mCanvas1->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas1 );

		mPanel1->setAutoAlpha(true);
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
		mCanvas3->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED);
		mCanvas3->requestUpdateCanvas = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateCanvas3 );
		mCanvas3->updateTexture();

		/*mNodeWindow1 = mPanel3->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowSmallC", MyGUI::IntCoord(20, 50, 100, 80), MyGUI::Align::Default);
		mNodeWindow1->setCaption( "Node1" );
		mNodeWindow1->eventWindowChangeCoord = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowChangeCoord);

		MyGUI::ButtonPtr button = mNodeWindow1->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(1, 40, 9, 9), MyGUI::Align::Default);
		button->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonPressed);
		button->eventMouseButtonReleased = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonReleased);
		button->eventMouseDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseDrag);
		button->setUserString("NodeLink", "true");

		button = mNodeWindow1->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(mNodeWindow1->getWidth() - 12, 40, 9, 9), MyGUI::Align::Right | MyGUI::Align::Top);
		button->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonPressed);
		button->eventMouseButtonReleased = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonReleased);
		button->eventMouseDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseDrag);
		button->setUserString("NodeLink", "false");

		mNodeWindow2 = mPanel3->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowSmallC", MyGUI::IntCoord(220, 150, 100, 80), MyGUI::Align::Default);
		mNodeWindow2->setCaption( "Node2" );
		mNodeWindow2->eventWindowChangeCoord = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowChangeCoord);

		button = mNodeWindow2->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(1, 40, 9, 9), MyGUI::Align::Default);
		button->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonPressed);
		button->eventMouseButtonReleased = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonReleased);
		button->eventMouseDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseDrag);
		button->setUserString("NodeLink", "true");

		button = mNodeWindow2->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(mNodeWindow2->getWidth() - 12, 40, 9, 9), MyGUI::Align::Right | MyGUI::Align::Top);
		button->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonPressed);
		button->eventMouseButtonReleased = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonReleased);
		button->eventMouseDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseDrag);
		button->setUserString("NodeLink", "false");

		mNodeWindow3 = mPanel3->createWidget<MyGUI::Window>(MyGUI::WidgetStyle::Overlapped, "WindowSmallC", MyGUI::IntCoord(420, 50, 100, 80), MyGUI::Align::Default);
		mNodeWindow3->setCaption( "Node3" );
		mNodeWindow3->eventWindowChangeCoord = MyGUI::newDelegate(this, &DemoKeeper::notifyWindowChangeCoord);

		button = mNodeWindow3->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(1, 40, 9, 9), MyGUI::Align::Default);
		button->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonPressed);
		button->eventMouseButtonReleased = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonReleased);
		button->eventMouseDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseDrag);
		button->setUserString("NodeLink", "true");

		button = mNodeWindow3->createWidget<MyGUI::Button>("ButtonRound", MyGUI::IntCoord(mNodeWindow3->getWidth() - 12, 40, 9, 9), MyGUI::Align::Right | MyGUI::Align::Top);
		button->eventMouseButtonPressed = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonPressed);
		button->eventMouseButtonReleased = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseButtonReleased);
		button->eventMouseDrag = MyGUI::newDelegate(this, &DemoKeeper::notifyMouseDrag);
		button->setUserString("NodeLink", "false");

		mCanvas3->updateTexture();*/

		/*mGraphView = new wraps::BaseGraphView("", mCanvas3);

		GraphNodeSimple * node1 = new GraphNodeSimple("Node1");
		mGraphView->addItem(node1);
		//mGraphView->removeItem(node1);

		GraphNodeSimple * node2 = new GraphNodeSimple("Node2");
		mGraphView->addItem(node2);

		GraphNodeSimple * node3 = new GraphNodeSimple("Node3");
		mGraphView->addItem(node3);*/
	}	

    void DemoKeeper::destroyScene()
    {
		//delete mCanvasFactory;
		//delete mTestRenderBoxFactory;
		delete mGraphView;
    }

	void DemoKeeper::notifyWindowChangeCoord(MyGUI::WindowPtr _sender)
	{
		mCanvas3->updateTexture();
	}

	void DemoKeeper::requestUpdateCanvas1( MyGUI::CanvasPtr canvas, MyGUI::Canvas::Event _canvasEvent )
    {
	}

	// Load from cache
	void DemoKeeper::requestUpdateCanvas2( MyGUI::CanvasPtr canvas, MyGUI::Canvas::Event _canvasEvent )
    {
	}

	/*struct SplineInfo
	{
		SplineInfo(int _x1, int _y1, int _x2, int _y2, int _offset1, int _offset2, int _r, int _g, int _b, bool _is_left1, bool _is_left2, int _line_width) :
			x1(_x1), y1(_y1), x2(_x2), y2(_y2), offset1(_offset1), offset2(_offset2), r(_r), g(_g), b(_b), is_left1(_is_left1), is_left2(_is_left2), line_width(_line_width)
		{ }
		int x1, y1, x2, y2;
		int offset1, offset2;
		int r, g, b;
		bool is_left1, is_left2;
		int line_width;
	};*/

	void clearCanvas(unsigned char* _data, int _width, int _height)
	{
        agg::rendering_buffer rbuf;
        rbuf.attach(_data, _width, _height, _width*4);

        // Pixel format and basic primitives renderer
        agg::pixfmt_bgra32 pixf(rbuf);
        agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

        renb.clear(agg::rgba8(106, 147, 221, 0));
	}

	/*void drawCurve(unsigned char* _data, int _width, int _height, SplineInfo& _info)
	{
        //============================================================ 
        // AGG
        agg::rendering_buffer rbuf;
        rbuf.attach(_data, _width, _height, _width*4);

        // Pixel format and basic primitives renderer
        agg::pixfmt_bgra32 pixf(rbuf);
        agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

        //renb.clear(agg::rgba8(152, 185, 254, 0));

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
		curve.init(_info.x1, _info.y1, _info.x1 + (_info.offset1 * (_info.is_left1 ? -1 : 1)), _info.y1, _info.x2 + (_info.offset2 * (_info.is_left2 ? -1 : 1)), _info.y2, _info.x2, _info.y2);

		// добавляем путь безье
		path.concat_path(curve);

		// сам путь который рисуется, растерезатор
		agg::conv_stroke<agg::path_storage> stroke(path);
		stroke.width(_info.line_width); // ширина линии
		stroke.line_join(agg::line_join_e(agg::bevel_join)); // хз че такое
		stroke.line_cap(agg::line_cap_e(agg::butt_cap)); //обрезка концов
		stroke.inner_join(agg::inner_join_e(agg::inner_miter)); // соединения внутри линии точек
		stroke.inner_miter_limit(1.01);

		ras.add_path(stroke);

        // Setting the attrribute (color) & Rendering
		ren.color(agg::rgba8(_info.r, _info.g, _info.b, 255)); //a
        agg::render_scanlines(ras, sl, ren);

		//============================================================ 
  	}*/

	void drawCurve(unsigned char* _data, int _width, int _height, LinkInfo& _info)
	{
        //============================================================ 
        // AGG
        agg::rendering_buffer rbuf;
        rbuf.attach(_data, _width, _height, _width*4);

        // Pixel format and basic primitives renderer
        agg::pixfmt_bgra32 pixf(rbuf);
        agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

        //renb.clear(agg::rgba8(152, 185, 254, 0));

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
		curve.approximation_scale(0.7); //масштаб апроксимации
		curve.angle_tolerance(agg::deg2rad(0));
		curve.cusp_limit(agg::deg2rad(0));
		const int offset = 3;
		curve.init(
			_info.point_start.left, _info.point_start.top, _info.point_start.left, _info.point_start.top,
			_info.point_end.left, _info.point_end.top, _info.point_end.left, _info.point_end.top);

		// добавляем путь безье
		path.concat_path(curve);

		// сам путь который рисуется, растерезатор
		agg::conv_stroke<agg::path_storage> stroke(path);
		stroke.width(2); // ширина линии
		stroke.line_join(agg::line_join_e(agg::bevel_join)); // хз че такое
		stroke.line_cap(agg::line_cap_e(agg::round_cap)); //обрезка концов
		stroke.inner_join(agg::inner_join_e(agg::inner_miter)); // соединения внутри линии точек
		stroke.inner_miter_limit(0);

		ras.add_path(stroke);

        // Setting the attrribute (color) & Rendering
		ren.color(agg::rgba8(_info.colour.red * 255, _info.colour.green * 255, _info.colour.blue * 255, 255));
        agg::render_scanlines(ras, sl, ren);


		//============================================================ 
		// хранилище всех путей
		/*agg::path_storage path2;

		// кривая безье которая строится по 4 точкам
		agg::curve4 curve2;
		curve2.approximation_method(agg::curve_approximation_method_e(agg::curve_inc)); // метод апроксимации, curve_inc - быстрый но много точек
		curve2.approximation_scale(0.7); //масштаб апроксимации
		curve2.angle_tolerance(agg::deg2rad(0));
		curve2.cusp_limit(agg::deg2rad(0));
		curve2.init(
			_info.point_start.left, _info.point_start.top, _info.point_start.left + _info.start_offset, _info.point_start.top,
			_info.point_end.left + _info.end_offset, _info.point_end.top, _info.point_end.left, _info.point_end.top);

		// добавляем путь безье
		path2.concat_path(curve2);

		// сам путь который рисуется, растерезатор
		agg::conv_stroke<agg::path_storage> stroke2(path2);
		stroke2.width(2); // ширина линии
		stroke2.line_join(agg::line_join_e(agg::bevel_join)); // хз че такое
		stroke2.line_cap(agg::line_cap_e(agg::butt_cap)); //обрезка концов
		stroke2.inner_join(agg::inner_join_e(agg::inner_miter)); // соединения внутри линии точек
		stroke2.inner_miter_limit(1.01);

		ras.add_path(stroke2);

        // Setting the attrribute (color) & Rendering
		ren.color(agg::rgba8(_info.colour.red * 255, _info.colour.green * 255, _info.colour.blue * 255, 255));
        agg::render_scanlines(ras, sl, ren);*/
		//============================================================ 
	}

	void DemoKeeper::requestUpdateCanvas3( MyGUI::CanvasPtr canvas, MyGUI::Canvas::Event _canvasEvent )
    {
		if( ! _canvasEvent.textureChanged && ! _canvasEvent.requested ) return;
		//if (mNodeWindow1 == nullptr || mNodeWindow2 == nullptr) return;

		unsigned char * data = (unsigned char*)canvas->lock();

		int width = canvas->getTextureRealWidth();
		int height = canvas->getTextureRealHeight();

		//const MyGUI::IntPoint& node1 = mNodeWindow1->getPosition();
		//const MyGUI::IntPoint& node2 = mNodeWindow2->getPosition();

		clearCanvas((unsigned char*)data, width, height);

		/*SplineInfo info1(node1.left + 96, node1.top + 46, node2.left + 2, node2.top + 46, 96, 104, 88, 88, 88, false, true, 2);
		drawCurve((unsigned char*)data, width, height, info1);
		
		SplineInfo info2(node1.left + 96, node1.top + 45, node2.left + 2, node2.top + 45, 100, 100, 255, 0, 0, false, true, 2);
		drawCurve((unsigned char*)data, width, height, info2);*/

		LinkInfo link(MyGUI::IntPoint(10, 10), MyGUI::IntPoint(100, 100), MyGUI::Colour(1, 1, 1), 0, 0);
		drawCurve((unsigned char*)data, width, height, link);

		// yниточк адля драга
		//if (mIsDrug)
		//	drawCurve((unsigned char*)data, width, height, mDrugLine);
		

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

		}
		/*else if( arg.key == OIS::KC_1 )
		{ // бррр :)
			mCanvas1->createTexture( mCanvas1->getTextureRealWidth() + 1, mCanvas1->getTextureRealHeight() + 1, MyGUI::Canvas::TRM_PT_CONST_SIZE );
		}*/
		else if (arg.key == OIS::KC_Q)
		{
			mTestRenderBox1->removeCamera();
		}
		else if (arg.key == OIS::KC_W)
		{
			mTestRenderBox1->setCamera( mCamera );
		}
		else if (arg.key == OIS::KC_1)
		{
			mPanel1->setVisible(true);
		}
		else if (arg.key == OIS::KC_2)
		{
			mPanel1->setVisible(false);
		}
		else if (arg.key == OIS::KC_3)
		{
			mPanel1->setVisibleSmooth(true);
		}
		else if (arg.key == OIS::KC_4)
		{
			mPanel1->setVisibleSmooth(false);
		}

		return result;
	}
	 
} // namespace demo

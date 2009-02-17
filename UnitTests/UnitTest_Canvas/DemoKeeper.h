/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
//#include "MyGUI_Canvas.h"
//#include "MyGUI_CanvasFactory.h"

//#include "MyGUI_TestRenderBox.h"
//#include "MyGUI_TestRenderBoxFactory.h"

#include "BaseGraphView.h"

namespace demo
{

	struct LinkInfo
	{
		LinkInfo() { }
		LinkInfo(const MyGUI::IntPoint& _point_start, const MyGUI::IntPoint& _point_end, const MyGUI::Colour& _colour, int _start_offset, int _end_offset) :
			point_start(_point_start), point_end(_point_end), colour(_colour), start_offset(_start_offset), end_offset(_end_offset)
		{ }
		MyGUI::Colour colour;
		MyGUI::IntPoint point_start;
		MyGUI::IntPoint point_end;
		int start_offset;
		int end_offset;
	};

	struct PaintInfo
	{
		int x, y;
		Ogre::ColourValue colour;
		bool vert;
		int len;
	};
	typedef std::vector<PaintInfo> VectorPaintInfo;

	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper() : base::BaseManager() { mGraphView = 0; }
		virtual void createScene();
		virtual void destroyScene();
		virtual bool keyPressed( const OIS::KeyEvent &arg );

		virtual bool frameStarted( const Ogre::FrameEvent & evt );

	private:
		void requestUpdateCanvas1( MyGUI::CanvasPtr canvas, MyGUI::Canvas::Event _canvasEvent );
		void requestUpdateCanvas2( MyGUI::CanvasPtr canvas, MyGUI::Canvas::Event _canvasEvent );
		void requestUpdateCanvas3( MyGUI::CanvasPtr canvas, MyGUI::Canvas::Event _canvasEvent );

		void notifyWindowChangeCoord(MyGUI::WindowPtr _sender);
		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);

		//MyGUI::factory::CanvasFactory* mCanvasFactory;
		//MyGUI::factory::TestRenderBoxFactory* mTestRenderBoxFactory;

		MyGUI::CanvasPtr mCanvas1;
		MyGUI::CanvasPtr mCanvas2;
		MyGUI::CanvasPtr mCanvas3;

		MyGUI::RenderBoxPtr mTestRenderBox1;

		MyGUI::WindowPtr mPanel1;
		MyGUI::WindowPtr mPanel2;
		MyGUI::WindowPtr mPanel3;

		Ogre::Real mCanvas1Size;
		Ogre::Real mCanvas2Size;
		Ogre::Real mCanvas3Size;

		VectorPaintInfo mPaintData;

		//MyGUI::WindowPtr mNodeWindow1;
		//MyGUI::WindowPtr mNodeWindow2;
		//MyGUI::WindowPtr mNodeWindow3;

		//LinkInfo mDrugLine;
		//bool mIsDrug;

		wraps::BaseGraphView * mGraphView;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

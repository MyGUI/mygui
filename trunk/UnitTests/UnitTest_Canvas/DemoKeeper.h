/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "MyGUI_Canvas.h"
#include "MyGUI_CanvasFactory.h"

namespace demo
{

	struct PaintInfo
	{
		int x, y;
		Ogre::ColourValue colour;
	};
	typedef std::vector<PaintInfo> VectorPaintInfo;

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();
		virtual bool keyPressed( const OIS::KeyEvent &arg );

	private:
		void requestUpdateTexture1( MyGUI::CanvasPtr canvas );
		void requestUpdateTexture2( MyGUI::CanvasPtr canvas );
		void requestUpdateTexture3( MyGUI::CanvasPtr canvas );

		MyGUI::factory::CanvasFactory * mCanvasFactory;

		MyGUI::CanvasPtr mCanvas1;
		MyGUI::CanvasPtr mCanvas2;
		MyGUI::CanvasPtr mCanvas3;

		MyGUI::WidgetPtr mPanel1;
		MyGUI::WidgetPtr mPanel2;
		MyGUI::WidgetPtr mPanel3;

		Ogre::Real mCanvas1Size;
		Ogre::Real mCanvas2Size;
		Ogre::Real mCanvas3Size;

		VectorPaintInfo mPaintData;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

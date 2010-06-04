/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "BaseGraphView.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper() : base::BaseManager() { mGraphView = 0; }
		virtual void createScene();
		virtual void destroyScene();

	private:
		void eventChangeSize(wraps::BaseGraphView* _sender, MyGUI::IntSize _size);

	private:
		MyGUI::CanvasPtr mCanvas3;
		MyGUI::WindowPtr mPanel3;
		MyGUI::ScrollViewPtr mScrollView3;

		wraps::BaseGraphView * mGraphView;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

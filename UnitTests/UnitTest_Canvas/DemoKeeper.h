/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "BaseGraphView.h"
#include "GraphNodeSimple.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();
		virtual void createScene();
		virtual void destroyScene();
		virtual void setupResources();

	private:
		void eventChangeSize(wraps::BaseGraphView* _sender, MyGUI::IntSize _size);

	private:
		MyGUI::CanvasPtr mCanvas3;
		MyGUI::WindowPtr mPanel3;
		MyGUI::ScrollViewPtr mScrollView3;

		wraps::BaseGraphView* mGraphView;
		GraphNodeSimple* mNode1;
		GraphNodeSimple* mNode2;
		GraphNodeSimple* mNode3;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

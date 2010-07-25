/*!
	@file
	@author		George Evmenov
	@date		07/2010
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();

		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
	private:
		virtual void setupResources();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

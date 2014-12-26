/*!
	@file
	@author		George Evmenov
	@date		02/2010
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		virtual void createScene();

		void injectMousePress(int _absx, int _absy, MyGUI::MouseButton _id);
	private:
		virtual void setupResources();

		virtual void injectMouseMove(int _absx, int _absy, int _absz);
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

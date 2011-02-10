/*!
	@file
	@author		George Evmenov
	@date		02/2011
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();
		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

	private:
		MyGUI::TextBox* m_button;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>
#include "Base/BaseManager.h"
#include "Console.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

		virtual bool keyPressed( const OIS::KeyEvent &arg );

	private:
		void command(const Ogre::UTFString & _key, const Ogre::UTFString & _value);

	private:
		Console * mConsole;
		MyGUI::EditPtr mEdit;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

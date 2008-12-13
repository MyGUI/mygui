/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"

namespace demo
{
		/*struct Colour
		{
			float red, green, blue, alpha;

			Colour() : red( 1 ), green( 1 ), blue( 1 ), alpha( 1 ) { }
			Colour( float _red, float _green, float _blue, float _alpha = 1 ) : red( _red ), green( _green ), blue( _blue ), alpha( _alpha ) { }

			//static const Colour Red = { 1, 0, 0, 1 };
		};*/

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();
		virtual bool keyPressed( const OIS::KeyEvent &arg );
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

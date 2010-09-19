/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "FontPanel.h"

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
		FontPanel* mFontPanel;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

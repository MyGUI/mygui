/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "FontPanel.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		FontPanel * mFontPanel;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

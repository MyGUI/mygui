/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "ColourPanel.h"
#include "EditPanel.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		void notifyColourAccept(ColourPanel* _sender);

	private:
		ColourPanel * mColourPanel;
		EditPanel * mEditPanel;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

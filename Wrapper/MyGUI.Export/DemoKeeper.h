/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "ExportDefine.h"

namespace demo
{

	typedef void (MYGUICALLBACK *HandleFrameStart)(float _time);

	class DemoKeeper : public base::BaseManager
	{
	public:
		DemoKeeper() : mFrameStartDelegate(nullptr) { }

		virtual void createScene();
		virtual void destroyScene();

		virtual bool frameStarted(const Ogre::FrameEvent& evt);
		virtual bool frameEnded(const Ogre::FrameEvent& evt);

		void exit() { m_exit = true; }

		void setFrameEvent(HandleFrameStart _delegate) { mFrameStartDelegate = _delegate; }

	private:
		HandleFrameStart mFrameStartDelegate;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

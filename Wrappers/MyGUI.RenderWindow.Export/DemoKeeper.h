/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "ExportDefine.h"

namespace demo
{

	typedef void (MYGUICALLBACK* HandleFrameStart)(float _time);

	class DemoKeeper :
		public base::BaseManager
	{
	public:
		DemoKeeper();

		virtual void setupResources();
		virtual void createScene();
		virtual void destroyScene();

		void notifyFrameStart(float _time);
		void setFrameEvent(HandleFrameStart _delegate);

	private:
		HandleFrameStart mFrameStartDelegate;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

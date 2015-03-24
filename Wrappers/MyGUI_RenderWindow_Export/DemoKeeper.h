/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

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

#endif // DEMO_KEEPER_H_

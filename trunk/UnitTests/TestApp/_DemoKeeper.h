#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "RenderManager.h"

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
		RenderManager* mRenderManager;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

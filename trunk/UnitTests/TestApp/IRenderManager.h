#ifndef __I_RENDER_MANAGER_H__
#define __I_RENDER_MANAGER_H__

#include "BaseManager.h"

namespace demo
{

	class IRenderManager
	{
	public:
		IRenderManager() : mCurrentManual(false) { }
		virtual ~IRenderManager() { }

		bool getCurrentManual() { return mCurrentManual; }
		void setCurrentManual(bool _value) { mCurrentManual = _value; }

		virtual void initState() { }
		virtual Ogre::RenderSystem* getRenderSystem() { return nullptr; }

	private:
		bool mCurrentManual;
	};

} // namespace demo

#endif // __I_RENDER_MANAGER_H__

/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "View.h"

namespace editor
{

	class Controller : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	private:
		View * mView;

	};

} // namespace editor

#endif // __DEMO_KEEPER_H__

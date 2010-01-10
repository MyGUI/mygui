/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "BaseManager.h"
#include "View.h"

namespace editor
{

	class Controller : public base::BaseManager
	{
	public:
		Controller();

		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

	private:
		View* mView;

	};

} // namespace editor

#endif // __CONTROLLER_H__

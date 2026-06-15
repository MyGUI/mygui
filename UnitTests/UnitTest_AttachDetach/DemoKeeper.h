/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

		void notifyFrameStarted(float _time);
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

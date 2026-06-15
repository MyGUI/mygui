/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "State.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void notifyButtonPress(ControllerType _type, bool _complite);
		void setupResources() override;

	private:
		State* mMenu1{nullptr};
		State* mMenu2{nullptr};
		State* mMenu3{nullptr};
		State* mMenu4{nullptr};
		ControllerType mType;
		bool mIsAnim{false};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

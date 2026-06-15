/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "ColourPanel.h"
#include "EditPanel.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void notifyColourAccept(ColourPanel* _sender);
		void setupResources() override;

	private:
		ColourPanel* mColourPanel{nullptr};
		EditPanel* mEditPanel{nullptr};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "HyperTextBox.h"

namespace demo
{
	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

		virtual void createScene();
		virtual void destroyScene();

	private:
		virtual void setupResources();

		void OnClickUrl(MyGUI::HyperTextBox* _sender, const std::string& _url);
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "HyperTextBox.h"

namespace demo
{
	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void setupResources() override;

		void OnClickUrl(MyGUI::HyperTextBox* _sender, std::string_view _url);
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

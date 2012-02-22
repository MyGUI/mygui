#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "BaseManager.h"
#include "HyperTextBox.h"

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

		void OnClickUrl(MyGUI::HyperTextBox* _sender, const std::string& _url);

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

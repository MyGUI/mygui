#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include <MyGUI.h>
#include "Base/BaseDemoManager.h"
#include <array>

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	public:
		void createScene() override;
		void destroyScene() override;

	private:
		void setupResources() override;

		void increaseFontHeight(MyGUI::Widget* _sender);
		void decreaseFontHeight(MyGUI::Widget* _sender);

		void notifyFrameStart(float _time);

	private:
		std::vector<MyGUI::EditBox*> mEditBoxes;
		MyGUI::EditBox* mCycleEdit{nullptr};
		MyGUI::Window* mCycleWindow{nullptr};
		std::vector<std::string> mFontList;
		size_t mFontIndex{0};
		float mFrameTimer{0};
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

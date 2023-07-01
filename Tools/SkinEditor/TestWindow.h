/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/

#ifndef _b38d3a42_9f99_471a_80ed_7ebdaf3282f4_
#define _b38d3a42_9f99_471a_80ed_7ebdaf3282f4_

#include <MyGUI.h>
#include "Dialog.h"
#include "Data.h"
#include "Control.h"
#include "BackgroundControl.h"

namespace tools
{

	class TestWindow : public Dialog, public Control
	{
	public:
		TestWindow();
		~TestWindow() override;

		void setSkinItem(DataPtr _item);

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

		void onDoModal() override;
		void onEndModal() override;

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

		void createSkin();
		void deleteSkin();

		void generateSkin();

	private:
		BackgroundControl* mBackgroundControl{nullptr};
		DataPtr mSkinItem{nullptr};
		MyGUI::Button* mSkinButton{nullptr};
		std::string mSkinName;
		MyGUI::Widget* mBack{nullptr};
		std::string mTestSkinFileName;
		std::string mDefaultFontName;
	};

}

#endif

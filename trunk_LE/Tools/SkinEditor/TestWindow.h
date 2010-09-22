/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __TEST_WINDOW_H__
#define __TEST_WINDOW_H__

#include <MyGUI.h>
#include "Dialog.h"
#include "BackgroundControl.h"
#include "SkinItem.h"

namespace tools
{

	class TestWindow :
		public Dialog
	{
	public:
		TestWindow();
		virtual ~TestWindow();

		void setSkinItem(SkinItem* _item);

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);

		void createSkin();
		void deleteSkin();

		void generateSkin();

	private:
		BackgroundControl* mBackgroundControl;
		SkinItem* mSkinItem;
		MyGUI::Button* mSkinButton;
		MyGUI::UString mSkinName;
	};

} // namespace tools

#endif // __TEST_WINDOW_H__

/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/

#ifndef _2836f03e_07c4_4388_aca4_d133e07c887f_
#define _2836f03e_07c4_4388_aca4_d133e07c887f_

#include <MyGUI.h>
#include "Dialog.h"
#include "BackgroundControl.h"
#include "SkinItem.h"

/*namespace tools
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

}*/

#endif

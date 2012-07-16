/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _07f612bd_91d2_4e55_828d_a10206631a31_
#define _07f612bd_91d2_4e55_828d_a10206631a31_

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"
#include "TextureBrowseControl.h"
#include "Dialog.h"

/*namespace tools
{

	class PropertyTexturesControl :
		public wraps::BaseLayout,
		public PropertyControl
	{
	public:
		PropertyTexturesControl(MyGUI::Widget* _parent);
		virtual ~PropertyTexturesControl();

	protected:
		virtual void updateProperty();

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyEndDialog(Dialog* _sender, bool _result);

		size_t getComboIndex(const MyGUI::UString& _name);

		void fillTextures();

	private:
		MyGUI::ComboBox* mComboBox;
		MyGUI::Button* mBrowse;

		TextureBrowseControl* mTextureBrowseControl;
		MyGUI::VectorString mTextures;
	};

}*/

#endif

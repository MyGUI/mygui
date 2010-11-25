/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_TEXTURES_CONTROL_H__
#define __PROPERTY_TEXTURES_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "PropertyControl.h"
#include "TextureBrowseControl.h"
#include "Tools/Dialog.h"

namespace tools
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

} // namespace tools

#endif // __PROPERTY_TEXTURES_CONTROL_H__

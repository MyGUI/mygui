/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __STATIC_IMAGE_VIEW_H__
#define __STATIC_IMAGE_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

class StaticImageView : public BaseLayout
{
public:
	StaticImageView();

	virtual void initialise();

private:

	MyGUI::ComboBoxPtr mComboResource;
	MyGUI::ComboBoxPtr mComboGroup;
	MyGUI::ComboBoxPtr mComboName;

	MyGUI::StaticTextPtr mTextGuid;
	MyGUI::StaticTextPtr mTextTexture;
	MyGUI::StaticTextPtr mTextSize;
	MyGUI::StaticTextPtr mTextFrames;
	MyGUI::StaticTextPtr mTextRate;

	MyGUI::ScrollViewPtr mImageView;

};

#endif // __STATIC_IMAGE_VIEW_H__

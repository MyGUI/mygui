/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __STATIC_IMAGE_VIEW_H__
#define __STATIC_IMAGE_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	class StaticImageView : public wraps::BaseLayout
	{
	public:
		StaticImageView();

	private:
		void notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index);

		void initialiseImages();
		void selectResource(size_t _index);
		void selectGroup(size_t _index);
		void selectName(size_t _index);

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
		MyGUI::StaticImagePtr mImage;

		MyGUI::IntSize mImageSize;

	};

} // namespace demo

#endif // __STATIC_IMAGE_VIEW_H__

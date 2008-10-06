/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/

#include "StaticImageView.h"

StaticImageView::StaticImageView() :
	BaseLayout("StaticImageView.layout")
{
}

void StaticImageView::initialise()
{
	loadLayout();

	assignWidget(mComboResource, "combo_Resource");
	assignWidget(mComboGroup, "combo_Group");
	assignWidget(mComboName, "combo_Name");

	assignWidget(mTextGuid, "text_Guid");
	assignWidget(mTextTexture, "text_Texture");
	assignWidget(mTextSize, "text_Size");
	assignWidget(mTextFrames, "text_Frames");
	assignWidget(mTextRate, "text_Rate");

	assignWidget(mImageView, "view_Image");

}


/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "precompiled.h"
#include "StaticImageView.h"

namespace demo
{

	StaticImageView::StaticImageView() : BaseLayout("StaticImageView.layout")
	{
		assignWidget(mComboResource, "combo_Resource");
		assignWidget(mComboGroup, "combo_Group");
		assignWidget(mComboName, "combo_Name");

		assignWidget(mTextGuid, "text_Guid");
		assignWidget(mTextTexture, "text_Texture");
		assignWidget(mTextSize, "text_Size");
		assignWidget(mTextFrames, "text_Frames");
		assignWidget(mTextRate, "text_Rate");

		assignWidget(mImageView, "view_Image");
		assignWidget(mImage, "image_Image");

		mComboResource->eventComboAccept = MyGUI::newDelegate(this, &StaticImageView::notifyComboAccept);
		mComboGroup->eventComboAccept = MyGUI::newDelegate(this, &StaticImageView::notifyComboAccept);
		mComboName->eventComboAccept = MyGUI::newDelegate(this, &StaticImageView::notifyComboAccept);

		initialiseImages();
	}

	void StaticImageView::initialiseImages()
	{
		typedef std::vector<MyGUI::ResourceImageSetPtr> Resources;
		Resources resources = MyGUI::ResourceManager::getInstance().getResources<MyGUI::ResourceImageSet>();
		for (Resources::iterator iter=resources.begin(); iter!=resources.end(); ++iter) {
			mComboResource->addItem((*iter)->getResourceName(), (*iter));
		}

		if (mComboResource->getItemCount() > 0) {
			mComboResource->setIndexSelected(0);
		}
		selectResource(0);
	}

	void StaticImageView::notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index)
	{
		if (_sender->compare(mComboResource)) {
			selectResource(_index);
		}
		else if (_sender->compare(mComboGroup)) {
			selectGroup(_index);
		}
		else if (_sender->compare(mComboName)) {
			selectName(_index);
		}
	}

	void StaticImageView::selectResource(size_t _index)
	{
		if (_index < mComboResource->getItemCount()) {
			MyGUI::ResourceImageSetPtr image = *mComboResource->getItemDataAt<MyGUI::ResourceImageSetPtr>(_index);
			mTextGuid->setCaption(image->getResourceID().print());

			mComboGroup->removeAllItems();
			MyGUI::EnumeratorGroupImage groups = image->getEnumerator();
			while (groups.next()) {
				mComboGroup->addItem(groups.current().name, groups.current());
			}

			if (mComboGroup->getItemCount() > 0) {
				mComboGroup->setIndexSelected(0);
			}
			selectGroup(0);
		}
		else {
			mTextGuid->setCaption("");
			mTextTexture->setCaption("");
			mTextSize->setCaption("");
			mTextFrames->setCaption("");
			mTextRate->setCaption("");
			mComboGroup->removeAllItems();
			mComboName->removeAllItems();
			mImageView->setCanvasSize(0, 0);
			mImage->setVisible(false);
		}
	}

	void StaticImageView::selectGroup(size_t _index)
	{
		if (_index < mComboGroup->getItemCount()) {
			MyGUI::GroupImage * group = mComboGroup->getItemDataAt<MyGUI::GroupImage>(_index);

			mTextTexture->setCaption(group->texture);
			mTextSize->setCaption(MyGUI::utility::toString(group->size.width, " x ", group->size.height));
			mImageSize = group->size;

			mComboName->removeAllItems();
			for (size_t pos=0; pos<group->indexes.size(); ++pos) {
				mComboName->addItem(group->indexes[pos].name, group->indexes[pos]);
			}

			if (mComboName->getItemCount() > 0) {
				mComboName->setIndexSelected(0);
			}
			selectName(0);
		}
		else {
			mTextTexture->setCaption("");
			mTextSize->setCaption("");
			mTextFrames->setCaption("");
			mTextRate->setCaption("");
			mComboName->removeAllItems();
			mImageView->setCanvasSize(0, 0);
			mImage->setVisible(false);
		}
	}

	void StaticImageView::selectName(size_t _index)
	{
		if (_index < mComboName->getItemCount()) {
			MyGUI::IndexImage * name = mComboName->getItemDataAt<MyGUI::IndexImage>(_index);

			mTextFrames->setCaption(MyGUI::utility::toString(name->frames.size()));
			mTextRate->setCaption(MyGUI::utility::toString(name->rate));

			mImage->setItemResource(mComboResource->getCaption());
			mImage->setItemGroup(mComboGroup->getCaption());
			mImage->setItemName(mComboName->getCaption());
			mImage->setVisible(true);
			mImage->setCoord(0, 0, mImageSize.width, mImageSize.height);
			mImageView->setCanvasSize(mImageSize);
		}
		else {
			mTextFrames->setCaption("");
			mTextRate->setCaption("");
			mImageView->setCanvasSize(0, 0);
			mImage->setVisible(false);
		}
	}

} // namespace demo

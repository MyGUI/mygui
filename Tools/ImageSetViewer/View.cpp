/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "precompiled.h"
#include "View.h"

namespace editor
{

	View::View() : BaseLayout("View.layout")
	{
		assignWidget(mComboResource, "combo_Resource");
		assignWidget(mTextGuid, "text_Guid");
		assignWidget(mImageView, "view_Image");

		mComboResource->eventComboAccept = MyGUI::newDelegate(this, &View::notifyComboAccept);

		initialiseImages();
	}

	void View::initialiseImages()
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

	void View::notifyComboAccept(MyGUI::ComboBoxPtr _sender, size_t _index)
	{
		if (_sender->compare(mComboResource)) {
			selectResource(_index);
		}
	}

	void View::selectResource(size_t _index)
	{
		if (_index < mComboResource->getItemCount()) {
			MyGUI::ResourceImageSetPtr image = *mComboResource->getItemDataAt<MyGUI::ResourceImageSetPtr>(_index);
			mTextGuid->setCaption(image->getResourceID().print());

			updateView(image);
		}
		else {
			mTextGuid->setCaption("");
			mImageView->setCanvasSize(0, 0);
		}
	}

	void View::updateView(MyGUI::ResourceImageSetPtr _image)
	{
		MyGUI::EnumeratorWidgetPtr childs = mImageView->getEnumerator();
		MyGUI::WidgetManager::getInstance().destroyWidgets(childs);

		MyGUI::IntSize size(0, 10);
		MyGUI::EnumeratorGroupImage groups = _image->getEnumerator();
		while (groups.next()) {
			addGroup(_image, groups.current(), size);
		}
		mImageView->setCanvasSize(size);
	}

	void View::addGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage & _group, MyGUI::IntSize & _size)
	{
		const int left = 10;
		const int text_height = 26;

		MyGUI::StaticTextPtr text = mImageView->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(left, _size.height, 100, text_height), MyGUI::Align::Default);
		text->setCaption(MyGUI::utility::toString("group name : '", _group.name, "' ,  texture : '", _group.texture, "' ,  size : ", _group.size.print()));
		MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		text->setSize(size.width, text->getHeight());
		_size.height += text_height;

		if (_size.width < text->getRight()) _size.width = text->getRight();

		for (size_t pos=0; pos<_group.indexes.size(); ++pos) {
			addIndex(_image, _group, _group.indexes[pos], _size);
		}
	}

	void View::addIndex(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage & _group, const MyGUI::IndexImage & _index, MyGUI::IntSize & _size)
	{
		const int left = 10;
		const int offset_height = 10;
		const int text_height = 26;

		MyGUI::StaticImagePtr image = mImageView->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(left, _size.height, _group.size.width, _group.size.height), MyGUI::Align::Default);
		image->setItemResource(_image);
		image->setItemGroup(_group.name);
		image->setItemName(_index.name);

		MyGUI::StaticTextPtr text = mImageView->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(image->getRight() + left, _size.height, 100, text_height), MyGUI::Align::Default);
		text->setCaption(MyGUI::utility::toString("'", _index.name, "'"));
		MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		text->setSize(size.width, text->getHeight());

		if (_size.width < text->getRight()) _size.width = text->getRight();
		_size.height = std::max(text->getBottom(), image->getBottom()) + offset_height;
	}

} // namespace editor

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
		assignWidget(mImageView, "view_Image");
		assignWidget(mMultiList, "multi_List");
		assignWidget(mEditResourceName, "edit_ResourceName");
		assignWidget(mEditResourceID, "edit_ResourceID");
		assignWidget(mEditFileName, "edit_FileName");

		const int column_width = 300;
		mMultiList->addColumn("name", column_width);
		mMultiList->addColumn("id", mMultiList->getClientCoord().width - column_width);
		mMultiList->eventListChangePosition = MyGUI::newDelegate(this, &View::notifyListChangePosition);

		initialiseImages();

		m_CurrentTime = 0;
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &View::notifyFrameStart);

	}

	void View::notifyListChangePosition(MyGUI::MultiListPtr _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE) 
		{
			mEditResourceName->setCaption("");
			mEditResourceID->setCaption("");
			mImageView->setCanvasSize(0, 0);
		}
		else
		{
			mVectorAnimImages.clear();
			mEditResourceName->setCaption(_sender->getItemNameAt(_index));
			std::string id = _sender->getSubItemNameAt(1, _index);
			mEditResourceID->setCaption(id);
			mEditFileName->setCaption(MyGUI::helper::getResourcePath(MyGUI::ResourceManager::getInstance().getFileNameByID(MyGUI::Guid::parse(id))));

			MyGUI::ResourceImageSetPtr resource = *_sender->getItemDataAt<MyGUI::ResourceImageSetPtr>(_index);
			updateView(resource);
		}
	}

	void View::notifyFrameStart(float _time)
	{
		m_CurrentTime += _time;
		if (m_CurrentTime < 1) return;
		m_CurrentTime -= 1;

		for (VectorPairAnim::iterator iter=mVectorAnimImages.begin(); iter!=mVectorAnimImages.end(); ++iter)
		{
			PairAnim& paira = (*iter);
			if (paira.index + 1 < paira.group.indexes.size())
			{
				paira.index ++;
			}
			else
			{
				paira.index = 0;
			}
			paira.image->setItemName(paira.group.indexes[paira.index].name);
			paira.text->setCaption(paira.group.indexes[paira.index].name);
		}
	}

	void View::initialiseImages()
	{
		typedef std::vector<MyGUI::ResourceImageSetPtr> Resources;
		Resources resources = MyGUI::ResourceManager::getInstance().getResources<MyGUI::ResourceImageSet>();
		for (Resources::iterator iter=resources.begin(); iter!=resources.end(); ++iter) {

			mMultiList->addItem((*iter)->getResourceName(), (*iter));
			mMultiList->setSubItemNameAt(1, mMultiList->getItemCount()-1, (*iter)->getResourceID().print());
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

		MyGUI::EditPtr text = mImageView->createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(left, _size.height, 100, text_height), MyGUI::Align::Default);
		text->setEditReadOnly(true);
		text->setTextAlign(MyGUI::Align::Center);
		text->setCaption(MyGUI::utility::toString("group name : '", _group.name, "' ,  texture : '", _group.texture, "' ,  size : ", _group.size.print()));
		MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		text->setSize(size.width + 20, text->getHeight());
		_size.height += text_height + 20;

		if (_size.width < text->getRight()) _size.width = text->getRight();

		addAnimGroup(_image, _group, _size);

		for (size_t pos=0; pos<_group.indexes.size(); ++pos) {
			addIndex(_image, _group, _group.indexes[pos], _size);
		}

	}

	void View::addAnimGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage & _group, MyGUI::IntSize & _size)
	{
		const int left = 10;
		const int offset_height = 10;
		const int text_height = 26;

		MyGUI::StaticImagePtr image = mImageView->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(left, _size.height, _group.size.width, _group.size.height), MyGUI::Align::Default);
		image->setItemResourcePtr(_image);
		image->setItemGroup(_group.name);
		//image->setItemName(_index.name);

		MyGUI::StaticTextPtr text = mImageView->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(image->getRight() + left, _size.height, 200, text_height), MyGUI::Align::Default);
		//text->setEditReadOnly(true);
		//text->setCaption("Animatable group");
		MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		//text->setSize(size.width, text->getHeight());

		if (_size.width < text->getRight()) _size.width = text->getRight();
		_size.height = std::max(text->getBottom(), image->getBottom()) + offset_height + 20;

		mVectorAnimImages.push_back(PairAnim(text, image, _group));
	}

	void View::addIndex(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage & _group, const MyGUI::IndexImage & _index, MyGUI::IntSize & _size)
	{
		const int left = 10;
		const int offset_height = 10;
		const int text_height = 26;

		MyGUI::StaticImagePtr image = mImageView->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(left, _size.height, _group.size.width, _group.size.height), MyGUI::Align::Default);
		image->setItemResourcePtr(_image);
		image->setItemGroup(_group.name);
		image->setItemName(_index.name);

		MyGUI::EditPtr text = mImageView->createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(image->getRight() + left, _size.height, 200, text_height), MyGUI::Align::Default);
		text->setEditReadOnly(true);
		text->setCaption(_index.name);
		MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		//text->setSize(size.width, text->getHeight());

		if (_size.width < text->getRight()) _size.width = text->getRight();
		_size.height = std::max(text->getBottom(), image->getBottom()) + offset_height;
	}

} // namespace editor

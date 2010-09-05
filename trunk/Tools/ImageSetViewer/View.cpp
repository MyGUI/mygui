/*!
	@file
	@author		Albert Semenov
	@date		10/2008
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
		assignWidget(mEditFileName, "edit_FileName");

		mMultiList->addColumn("name", mMultiList->getClientCoord().width);
		mMultiList->eventListChangePosition += MyGUI::newDelegate(this, &View::notifyListChangePosition);

		initialiseImages();

		m_CurrentTime = 0;
		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &View::notifyFrameStart);

		updateColumn();

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
		{
			window->eventWindowChangeCoord += MyGUI::newDelegate(this, &View::notifyWindowChangeCoord);
		}
	}

	void View::notifyListChangePosition(MyGUI::MultiList* _sender, size_t _index)
	{
		if (_index == MyGUI::ITEM_NONE)
		{
			mEditResourceName->setCaption("");
			mImageView->setCanvasSize(0, 0);
		}
		else
		{
			mVectorAnimImages.clear();
			mEditResourceName->setCaption(_sender->getItemNameAt(_index));

			MyGUI::ResourceImageSetPtr resource = *_sender->getItemDataAt<MyGUI::ResourceImageSetPtr>(_index);
			updateView(resource);
		}
	}

	void View::notifyFrameStart(float _time)
	{
		m_CurrentTime += _time;
		if (m_CurrentTime < 1)
			return;
		m_CurrentTime -= 1;

		for (VectorPairAnim::iterator iter = mVectorAnimImages.begin(); iter != mVectorAnimImages.end(); ++iter)
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
		MyGUI::ResourceManager::EnumeratorPtr resource = MyGUI::ResourceManager::getInstance().getEnumerator();
		while (resource.next())
		{
			MyGUI::ResourceImageSetPtr image = resource.current().second->castType<MyGUI::ResourceImageSet>(false);
			if (image != nullptr)
			{
				mMultiList->addItem(image->getResourceName(), image);
			}
		}
	}

	void View::updateView(MyGUI::ResourceImageSetPtr _image)
	{
		MyGUI::EnumeratorWidgetPtr childs = mImageView->getEnumerator();
		MyGUI::WidgetManager::getInstance().destroyWidgets(childs);

		MyGUI::IntSize size(0, 10);
		MyGUI::EnumeratorGroupImage groups = _image->getEnumerator();
		while (groups.next())
		{
			addGroup(_image, groups.current(), size);
		}
		mImageView->setCanvasSize(size);
	}

	void View::addGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage& _group, MyGUI::IntSize& _size)
	{
		const int left = 10;
		const int text_height = 26;

		MyGUI::Edit* text = mImageView->createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(left, _size.height, 600, text_height), MyGUI::Align::Default);
		text->setEditReadOnly(true);
		text->setTextAlign(MyGUI::Align::Center);
		text->setCaption(MyGUI::utility::toString("group name : '", _group.name, "' ,  texture : '", _group.texture, "' ,  size : ", _group.size.print()));
		//MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		//text->setSize(size.width + 20, text->getHeight());
		_size.height += text_height + 20;

		if (_size.width < text->getRight()) _size.width = text->getRight();

		addAnimGroup(_image, _group, _size);

		for (size_t pos = 0; pos < _group.indexes.size(); ++pos)
		{
			addIndex(_image, _group, _group.indexes[pos], _size);
		}

	}

	void View::addAnimGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage& _group, MyGUI::IntSize& _size)
	{
		const int left = 10;
		const int offset_height = 10;
		const int text_height = 26;

		MyGUI::StaticImage* image = mImageView->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(left, _size.height, _group.size.width, _group.size.height), MyGUI::Align::Default);
		image->setItemResourcePtr(_image);
		image->setItemGroup(_group.name);
		//image->setItemName(_index.name);

		MyGUI::StaticText* text = mImageView->createWidget<MyGUI::StaticText>("StaticText", MyGUI::IntCoord(image->getRight() + left, _size.height, 600, text_height), MyGUI::Align::Default);
		//text->setEditReadOnly(true);
		//text->setCaption("Animatable group");
		//MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		//text->setSize(size.width, text->getHeight());

		if (_size.width < text->getRight()) _size.width = text->getRight();
		_size.height = std::max(text->getBottom(), image->getBottom()) + offset_height + 20;

		mVectorAnimImages.push_back(PairAnim(text, image, _group));
	}

	void View::addIndex(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage& _group, const MyGUI::IndexImage& _index, MyGUI::IntSize& _size)
	{
		const int left = 10;
		const int offset_height = 10;
		const int text_height = 26;

		MyGUI::StaticImage* image = mImageView->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(left, _size.height, _group.size.width, _group.size.height), MyGUI::Align::Default);
		image->setItemResourcePtr(_image);
		image->setItemGroup(_group.name);
		image->setItemName(_index.name);

		MyGUI::Edit* text = mImageView->createWidget<MyGUI::Edit>("Edit", MyGUI::IntCoord(image->getRight() + left, _size.height, 600, text_height), MyGUI::Align::Default);
		text->setEditReadOnly(true);
		text->setCaption(_index.name);
		//MyGUI::IntSize size = text->getSubWidgetText()->getTextSize();
		//text->setSize(size.width, text->getHeight());

		if (_size.width < text->getRight()) _size.width = text->getRight();
		_size.height = std::max(text->getBottom(), image->getBottom()) + offset_height;
	}

	void View::updateColumn()
	{
		mMultiList->setColumnWidthAt(0, mMultiList->getClientCoord().width);
	}

	void View::notifyWindowChangeCoord(MyGUI::Window* _sender)
	{
		updateColumn();
	}

} // namespace editor

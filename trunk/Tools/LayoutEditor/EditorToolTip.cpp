/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
*/

#include "precompiled.h"
#include "EditorToolTip.h"
#include "MyGUI_SkinManager.h"

namespace tools
{
	EditorToolTip::EditorToolTip() :
		BaseLayout("EditorToolTip.layout")
	{
		assignWidget(mText, "Text");

		mMinWidth = MyGUI::utility::parseInt(mMainWidget->getUserString("minWidth"));
		mMinHeight = MyGUI::utility::parseInt(mMainWidget->getUserString("minHeight"));
		mLastWidget = nullptr;
	}

	void EditorToolTip::show(MyGUI::Widget* _sender)
	{
		static const MyGUI::UString colour_error = MyGUI::LanguageManager::getInstance().getTag("ColourError");
		static const MyGUI::UString colour_success = MyGUI::LanguageManager::getInstance().getTag("ColourSuccess");

		std::string widget = _sender->getUserString("widget");
		bool exist = MyGUI::WidgetManager::getInstance().isFactoryExist(widget);
		std::string skin = _sender->getUserString("skin");
		std::string text = "Widget: " + (exist ? colour_success : colour_error) + widget + colour_success +
			"\nSkin: " + skin +
			"\nDefaultSize: " + _sender->getUserString("width") + " x " + _sender->getUserString("height")
			;

		mText->setCaption(text);

		int width = MyGUI::utility::parseInt(_sender->getUserString("width"));
		int height = MyGUI::utility::parseInt(_sender->getUserString("height"));

		const int MARGIN = 5;
		const int LINE_HEIGHT = 22;
		const int LINES = 3;

		MyGUI::SkinManager & manager = MyGUI::SkinManager::getInstance();
		if (manager.isExist(skin))
		{
			MyGUI::IntSize max_size;

			MyGUI::ResourceSkin* info = manager.getByName(skin);
			if (info != nullptr)
			{
				const MyGUI::VectorChildSkinInfo & child = info->getChild();
				for (size_t pos=0; pos<child.size(); ++pos)
				{
					const std::string& child_skin = child[pos].skin;
					if (!manager.isExist(child_skin)) continue;
					const MyGUI::ResourceSkin * child_info = manager.getByName(child_skin);
					const MyGUI::IntSize & child_size = child[pos].coord.size();
					MyGUI::IntSize dif_size = child_info->getSize() - child_size;

					if (max_size.width < dif_size.width) max_size.width = dif_size.width;
					if (max_size.height < dif_size.height) max_size.height = dif_size.height;
				}
			}

			width += max_size.width;
			height += max_size.height;
		}

		mMainWidget->setSize(std::max(mMinWidth, width + 2*MARGIN), std::max(mMinHeight, height + LINE_HEIGHT*LINES + 2*MARGIN));
		if (mLastWidget)
			MyGUI::Gui::getInstance().destroyWidget(mLastWidget);
		mLastWidget = mMainWidget->createWidgetT("StaticText", skin, MARGIN, MARGIN + LINE_HEIGHT*LINES, width, height, MyGUI::Align::Default);
		mLastWidget->castType<MyGUI::StaticText>()->setCaption(skin);

		mMainWidget->setVisible(true);
	}

	void EditorToolTip::hide()
	{
		mMainWidget->setVisible(false);
	}

	void EditorToolTip::move(const MyGUI::IntPoint & _point)
	{
		setPosition(_point);
	}

	void EditorToolTip::setPosition(const MyGUI::IntPoint & _point)
	{
		const MyGUI::IntPoint offset(10, 10);

		MyGUI::IntPoint point = _point + offset;

		const MyGUI::IntSize& size = mMainWidget->getSize();
		const MyGUI::IntSize& view_size = mMainWidget->getParentSize();

		if ((point.left + size.width) > view_size.width)
		{
			point.left -= offset.left + offset.left + size.width;
		}
		if ((point.top + size.height) > view_size.height)
		{
			point.top -= offset.top + offset.top + size.height;
		}

		mMainWidget->setPosition(point);
	}

} // namespace tools

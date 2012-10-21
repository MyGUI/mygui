/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
*/

#include "Precompiled.h"
#include "EditorToolTip.h"
#include "MyGUI_SkinManager.h"
#include "EditorWidgets.h"
#include "WidgetTypes.h"

template <> tools::EditorToolTip* MyGUI::Singleton<tools::EditorToolTip>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::EditorToolTip>::mClassTypeName = "EditorToolTip";

namespace tools
{

	EditorToolTip::EditorToolTip() :
		BaseLayout("EditorToolTip.layout"),
		mText(nullptr),
		mLastWidget(nullptr),
		mMinWidth(0),
		mMinHeight(0)
	{
		assignWidget(mText, "Text");

		mMinWidth = MyGUI::utility::parseInt(mMainWidget->getUserString("minWidth"));
		mMinHeight = MyGUI::utility::parseInt(mMainWidget->getUserString("minHeight"));
		mLastWidget = nullptr;
	}

	EditorToolTip::~EditorToolTip()
	{
	}

	void EditorToolTip::initialise()
	{
	}

	void EditorToolTip::shutdown()
	{
	}

	void EditorToolTip::show(const SkinInfo& _data)
	{
		const std::string& colour_error = MyGUI::LanguageManager::getInstance().getTag("ColourError");
		const std::string& colour_success = MyGUI::LanguageManager::getInstance().getTag("ColourSuccess");

		std::string skin = _data.widget_skin;
		std::string widget = _data.widget_type;

		MyGUI::ResourceSkin* skinInfo = MyGUI::SkinManager::getInstance().getByName(skin);
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(skin, false);

		bool templateRootFound = false;

		MyGUI::IntSize skinDefaultSize;
		if (skinInfo != nullptr)
		{
			skinDefaultSize = skinInfo->getSize();
			templateRootFound = true;

			if (widget.empty())
			{
				WidgetStyle* style = WidgetTypes::getInstance().findWidgetStyleBySkin(skin);
				if (style != nullptr)
					widget = style->name;
			}
		}
		else if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (MyGUI::VectorWidgetInfo::const_iterator container = data.begin(); container != data.end(); ++container)
			{
				if (container->name == "Root")
				{
					skinDefaultSize = container->intCoord.size();
					templateRootFound = true;

					MyGUI::MapString::const_iterator item = container->userStrings.find("LE_TargetWidgetType");
					if (item != container->userStrings.end())
						widget = (*item).second;

					break;
				}
			}
		}

		bool exist = MyGUI::WidgetManager::getInstance().isFactoryExist(widget);
		std::string text = "Widget: " + (exist ? colour_success : colour_error) + widget + colour_success +
			"\nSkin: " + skin +
			"\nDefaultSize: " + (templateRootFound ? MyGUI::utility::toString(skinDefaultSize.width, " x ", skinDefaultSize.height) : (colour_error + "'Root' widget not found"));

		mText->setCaption(text);

		const int MARGIN = 5;
		const int LINE_HEIGHT = 22;
		const int LINES = 3;

		if (MyGUI::SkinManager::getInstance().isExist(skin))
		{
			MyGUI::IntSize max_size;

			MyGUI::ResourceSkin* info = MyGUI::SkinManager::getInstance().getByName(skin);
			if (info != nullptr)
			{
				const MyGUI::VectorChildSkinInfo& child = info->getChild();
				for (size_t pos = 0; pos < child.size(); ++pos)
				{
					const std::string& child_skin = child[pos].skin;
					if (!MyGUI::SkinManager::getInstance().isExist(child_skin))
						continue;
					const MyGUI::ResourceSkin* child_info = MyGUI::SkinManager::getInstance().getByName(child_skin);
					const MyGUI::IntSize& child_size = child[pos].coord.size();
					MyGUI::IntSize dif_size = child_info->getSize() - child_size;

					if (max_size.width < dif_size.width)
						max_size.width = dif_size.width;
					if (max_size.height < dif_size.height)
						max_size.height = dif_size.height;
				}
			}

			skinDefaultSize.width += max_size.width;
			skinDefaultSize.height += max_size.height;
		}

		if (mLastWidget)
			MyGUI::Gui::getInstance().destroyWidget(mLastWidget);

		MyGUI::IntSize widgetSize = skinDefaultSize;

		mLastWidget = mMainWidget->createWidgetT("TextBox", skin, MARGIN, MARGIN + LINE_HEIGHT * LINES, widgetSize.width, widgetSize.height, MyGUI::Align::Default);
		MyGUI::TextBox* textBox = mLastWidget->castType<MyGUI::TextBox>();
		textBox->setCaption(skin);

		MyGUI::IntSize contentSize = textBox->getSize() - textBox->getTextRegion().size() + textBox->getTextSize();
		widgetSize.set(std::max(widgetSize.width, contentSize.width), std::max(widgetSize.height, contentSize.height));
		textBox->setSize(widgetSize);

		int width = std::max(mMinWidth, widgetSize.width + 2 * MARGIN);
		width = std::max(width, mText->getTextSize().width + 2 * MARGIN);
		mMainWidget->setSize(width, std::max(mMinHeight, widgetSize.height + LINE_HEIGHT * LINES + 2 * MARGIN));
		mMainWidget->setVisible(true);
	}

	void EditorToolTip::hide()
	{
		mMainWidget->setVisible(false);
	}

	void EditorToolTip::move(const MyGUI::IntPoint& _point)
	{
		setPosition(_point);
	}

	void EditorToolTip::setPosition(const MyGUI::IntPoint& _point)
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

}

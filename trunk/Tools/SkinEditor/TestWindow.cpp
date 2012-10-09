/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/

#include "Precompiled.h"
#include "TestWindow.h"
#include "SkinExportSerializer.h"
#include "SettingsManager.h"

namespace tools
{

	TestWindow::TestWindow() :
		mBackgroundControl(nullptr),
		mSkinItem(nullptr),
		mSkinButton(nullptr),
		mSkinName("GeneratedSkinName"),
		mBack(nullptr)
	{
	}

	TestWindow::~TestWindow()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed -= MyGUI::newDelegate(this, &TestWindow::notifyWindowButtonPressed);
	}

	void TestWindow::OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName)
	{
		Control::OnInitialise(_parent, _place, "TestWindow.layout");

		setDialogRoot(mMainWidget);

		assignWidget(mBack, "BackgroundControl");

		mBackgroundControl = findControl<BackgroundControl>();

		mTestSkinFileName = SettingsManager::getInstance().getValue("Editor/TestSkinFileName");
		mDefaultFontName = getRoot()->getUserString("DefaultFontName");
		if (mDefaultFontName.empty())
			mDefaultFontName = "Default";

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TestWindow::notifyWindowButtonPressed);
	}

	void TestWindow::onDoModal()
	{
		createSkin();
	}

	void TestWindow::onEndModal()
	{
		deleteSkin();
	}

	void TestWindow::notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
	{
		eventEndDialog(this, false);
	}

	void TestWindow::setSkinItem(DataPtr _item)
	{
		mSkinItem = _item;
	}

	void TestWindow::createSkin()
	{
		if (mSkinItem == nullptr)
			return;

		MyGUI::IntSize canvasSize = mBackgroundControl->getCanvas()->getSize();

		generateSkin();

		mSkinButton = mBackgroundControl->getCanvas()->createWidget<MyGUI::Button>(mSkinName, MyGUI::IntCoord(0, 0, canvasSize.width, canvasSize.height), MyGUI::Align::Stretch);
		mSkinButton->setFontName(mDefaultFontName);
		mSkinButton->setTextAlign(MyGUI::Align::Center);
		mSkinButton->setCaption("Caption");
		mSkinButton->eventMouseButtonPressed += MyGUI::newDelegate(this, &TestWindow::notifyMouseButtonPressed);

		MyGUI::IntCoord coord = mSkinItem->getPropertyValue<MyGUI::IntCoord>("Size");
		MyGUI::IntSize windowSize = coord.size() + mMainWidget->getSize() - canvasSize;
		MyGUI::IntSize parentSize = mMainWidget->getParentSize();

		mMainWidget->setCoord((parentSize.width - windowSize.width) / 2, (parentSize.height - windowSize.height) / 2, windowSize.width, windowSize.height);
	}

	void TestWindow::deleteSkin()
	{
		if (mSkinButton != nullptr)
		{
			mSkinButton->eventMouseButtonPressed -= MyGUI::newDelegate(this, &TestWindow::notifyMouseButtonPressed);
			MyGUI::WidgetManager::getInstance().destroyWidget(mSkinButton);
			mSkinButton = nullptr;
		}

		mSkinItem = nullptr;
	}

	void TestWindow::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Right)
			mSkinButton->setStateSelected(!mSkinButton->getStateSelected());
	}

	void TestWindow::generateSkin()
	{
		if (MyGUI::ResourceManager::getInstance().isExist(mSkinName))
			MyGUI::ResourceManager::getInstance().removeByName(mSkinName);

		pugi::xml_document doc;
		pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
		decl.append_attribute("version") = "1.0";
		decl.append_attribute("encoding") = "UTF-8";

		pugi::xml_node root = doc.append_child("MyGUI");
		root.append_attribute("type").set_value("Resource");
		root.append_attribute("version").set_value("1.1");

		SkinExportSerializer* serializer = new SkinExportSerializer();
		serializer->writeSkin(root, mSkinItem);
		delete serializer;

		root.select_single_node("Resource/@name").attribute().set_value(mSkinName.c_str());

		/*bool result = */doc.save_file(mTestSkinFileName.c_str(), "\t", (pugi::format_indent | pugi::format_write_bom | pugi::format_win_new_line) & (~pugi::format_space_before_slash));

		MyGUI::xml::Document docLoad;
		docLoad.open(mTestSkinFileName);
		MyGUI::xml::Element* resourceNode = docLoad.getRoot();

		MyGUI::ResourceManager::getInstance().loadFromXmlNode(resourceNode, "", MyGUI::Version(1, 1, 0));
	}

}

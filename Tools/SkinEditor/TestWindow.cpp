/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "TestWindow.h"
#include "ExportManager.h"

namespace tools
{

	TestWindow::TestWindow() :
		Dialog("TestWindow.layout"),
		mBackgroundControl(nullptr),
		mSkinItem(nullptr),
		mSkinButton(nullptr),
		mSkinName("GeneratedSkinName")
	{
		assignBase(mBackgroundControl, "BackgroundControl");

		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed += MyGUI::newDelegate(this, &TestWindow::notifyWindowButtonPressed);

		mMainWidget->setVisible(false);
	}

	TestWindow::~TestWindow()
	{
		MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
		if (window != nullptr)
			window->eventWindowButtonPressed -= MyGUI::newDelegate(this, &TestWindow::notifyWindowButtonPressed);
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

	void TestWindow::setSkinItem(SkinItem* _item)
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
		mSkinButton->setFontName("Default");
		mSkinButton->setTextAlign(MyGUI::Align::Center);
		mSkinButton->setCaption("Caption");
		mSkinButton->eventMouseButtonPressed += MyGUI::newDelegate(this, &TestWindow::notifyMouseButtonPressed);

		MyGUI::IntCoord coord = MyGUI::IntCoord ::parse(mSkinItem->getPropertySet()->getPropertyValue("Coord"));
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

		MyGUI::xml::Document doc;
		doc.createDeclaration();
		MyGUI::xml::Element* root = doc.createRoot("Resource");
		mSkinItem->serialization(root, MyGUI::Version());

		MyGUI::xml::Document docOut;
		docOut.createDeclaration();
		MyGUI::xml::Element* rootOut = docOut.createRoot("MyGUI");
		rootOut->addAttribute("type", "Resource");
		rootOut->addAttribute("version", "1.1");
		MyGUI::xml::Element* resourceNode = rootOut->createChild("Resource");

		ExportManager::getInstance().convertSkin(root, resourceNode);

		resourceNode->setAttribute("name", mSkinName);

		MyGUI::ResourceManager::getInstance().loadFromXmlNode(rootOut, "", MyGUI::Version(1, 1, 0));
	}

 } // namespace tools

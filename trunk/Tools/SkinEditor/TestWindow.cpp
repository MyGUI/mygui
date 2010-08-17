/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "precompiled.h"
#include "TestWindow.h"
#include "DemoKeeper.h"

namespace tools
{

	TestWindow::TestWindow() :
		wraps::BaseLayout("TestWindow.layout"),
		mBackgroundControl(nullptr),
		mSkinItem(nullptr),
		mSkinButton(nullptr),
		mSkinName("SE_GeneratedSkinName")
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

	void TestWindow::setVisible(bool _value)
	{
		if (mMainWidget->getVisible() != _value)
		{
			mMainWidget->setVisible(_value);

			if (_value)
			{
				MyGUI::InputManager::getInstance().addWidgetModal(mMainWidget);
				addDialog(this);

				createSkin();
			}
			else
			{
				deleteSkin();

				MyGUI::InputManager::getInstance().removeWidgetModal(mMainWidget);
				removeDialog(this);
			}
		}
	}

	bool TestWindow::getVisible()
	{
		return mMainWidget->getVisible();
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
		rootOut->addAttribute("version", "1.2");
		MyGUI::xml::Element* resourceNode = rootOut->createChild("Resource");

		DemoKeeper::convertSkin(root, resourceNode);

		doc.save(MyGUI::UString("test1.xml"));
		docOut.save(MyGUI::UString("test2.xml"));
	}

 } // namespace tools

/*!
	@file
	@author     Albert Semenov
	@date       08/2008
	@module
*/
#include "DemoKeeper.h"
#include "Base/Main.h"
#include "MyGUI_RTTLayer.h"
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_LayerNodeAnimation.h"
#include "WobbleNodeAnimator.h"
#include "FadeNodeAnimator.h"
#include "BaseLayout/BaseLayout.h"
#include "TreeControl.h"
#include "TreeControlItem.h"

	class SampleLayout : public wraps::BaseLayout
	{
	public:
		SampleLayout();

		void notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);
		MyGUI::UString getPath(MyGUI::TreeControl::Node* pNode) const;

	private:
		MyGUI::TreeControl * mpResourcesTree;
	};

SampleLayout * mSampleLayout;

    SampleLayout::SampleLayout() : BaseLayout("SampleLayout.layout")
    {
        assignWidget(mpResourcesTree, "ResourcesTree");
        mpResourcesTree->eventTreeNodePrepare = newDelegate(this, &SampleLayout::notifyTreeNodePrepare);

        MyGUI::TreeControl::Node* pRoot = mpResourcesTree->getRoot();
        Ogre::ArchiveManager::ArchiveMapIterator ArchiveIterator = Ogre::ArchiveManager::getSingleton().getArchiveIterator();
        while (ArchiveIterator.hasMoreElements())
        {
            Ogre::Archive* pArchive = ArchiveIterator.getNext();

            MyGUI::TreeControl::Node* pNode = new MyGUI::TreeControl::Node(pArchive->getName(), "Data");
            pNode->setData(pArchive);
            pRoot->add(pNode);
        }
    }

    void SampleLayout::notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
    {
        if (pNode == pTreeControl->getRoot())
            return;

        pNode->removeAll();
        Ogre::Archive* pArchive = *(pNode->getData<Ogre::Archive*>());

        MyGUI::UString strPath(getPath(pNode));
        Ogre::StringVectorPtr Resources = pArchive->find(strPath + "*", false, true);
        for (Ogre::StringVector::iterator Iterator = Resources->begin(); Iterator != Resources->end(); Iterator++)
        {
            MyGUI::TreeControl::Node* pChild = new MyGUI::TreeControl::Node(*Iterator, "Folder");
            pChild->setData(pArchive);
            pNode->add(pChild);
        }

        Resources = pArchive->find(strPath + "*", false, false);
        for (Ogre::StringVector::iterator Iterator = Resources->begin(); Iterator != Resources->end(); Iterator++)
        {
            MyGUI::UString strName(*Iterator);
            MyGUI::UString strExtension;
            size_t nPosition = strName.rfind(".");
            if (nPosition != MyGUI::UString::npos)
            {
                strExtension = strName.substr(nPosition + 1);
                std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), tolower);
            }

            MyGUI::UString strImage;
            if (strExtension == "png" || strExtension == "tif" || strExtension == "tiff" || strExtension == "jpg" || strExtension == "jpeg")
                strImage = "Image";
            else
            if (strExtension == "mat" || strExtension == "material")
                strImage = "Material";
            else
            if (strExtension == "layout")
                strImage = "Layout";
            else
            if (strExtension == "ttf" || strExtension == "font" || strExtension == "fontdef")
                strImage = "Font";
            else
            if (strExtension == "txt" || strExtension == "text")
                strImage = "Text";
            else
            if (strExtension == "xml")
                strImage = "XML";
            else
            if (strExtension == "mesh")
                strImage = "Mesh";
            else
            if (strExtension == "htm" || strExtension == "html")
                strImage = "HTML";
            else
                strImage = "Unknown";

            MyGUI::TreeControl::Node* pChild = new MyGUI::TreeControl::Node(strName, strImage);
            pChild->setPrepared(true);
            pNode->add(pChild);
        }
    }

    MyGUI::UString SampleLayout::getPath(MyGUI::TreeControl::Node* pNode) const
    {
        if (!pNode || pNode == mpResourcesTree->getRoot())
            return MyGUI::UString();

        MyGUI::UString strPath;
        while (pNode->getParent() != mpResourcesTree->getRoot())
        {
            strPath = pNode->getText() + "/" + strPath;
            pNode = pNode->getParent();
        }

        return strPath;
    }

namespace demo
{

	MyGUI::WindowPtr widget = nullptr;

	void notifyWindowButtonPressed(MyGUI::WindowPtr _sender, const std::string& _name)
	{
		if (_name == "close")
		{
			MyGUI::WidgetManager::getInstance().destroyWidget(_sender);
			widget = nullptr;
		}
		else if (_name == "check")
		{
			const MyGUI::IntCoord coord(0, 0, 1024, 768);
			const MyGUI::IntSize size(300, 300);

			if (widget->getCoord().width != coord.width)
			{
				widget->setCoord(coord);
			}
			else
			{
				widget->setCoord(coord.width / 2 - size.width / 2, coord.height / 2 - size.height / 2, size.width, size.height);
			}
		}
	}

	void test(int _value)
	{
	}

    void DemoKeeper::createScene()
    {
		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		factory.registryFactory<MyGUI::TreeControl>("Widget");
		factory.registryFactory<MyGUI::TreeControlItem>("Widget");

		mSampleLayout = new SampleLayout();
		/*MyGUI::FactoryManager::getInstance().registryFactory<WobbleNodeAnimator>("NodeAnimator");
		MyGUI::FactoryManager::getInstance().registryFactory<FadeNodeAnimator>("NodeAnimator");

		MyGUI::FactoryManager::getInstance().registryFactory<MyGUI::RTTLayer>("Layer");

		this->addResourceLocation("../../Media/UnitTests/TestApp");
		mGUI->load("test_layer.xml");

		widget = mGUI->createWidget<MyGUI::Window>("WindowCSMX", MyGUI::IntCoord(56, 16, 300, 300), MyGUI::Align::Default, "RTT_Test");
		widget->setCaption("Vertext mode");

		//MyGUI::EditPtr text = mGUI->createWidget<MyGUI::Edit>("EditStretch", MyGUI::IntCoord(356, 316, 300, 300), MyGUI::Align::Default, "RTT_Test");
		//text->setCaption("0 1 2 3");
		//text->setFontName("ManualFont");
		//text->setTextColour(MyGUI::Colour::White);

		//MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(46, 46, 164, 164), MyGUI::Align::Default, "RTT_Test");

		widget->eventWindowButtonPressed = MyGUI::newDelegate(notifyWindowButtonPressed);*/

	}

    void DemoKeeper::destroyScene()
    {
    }

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		if (arg.key == OIS::KC_H)
		{
			widget = mGUI->createWidget<MyGUI::Window>("WindowCSMX", MyGUI::IntCoord(56, 16, 300, 300), MyGUI::Align::Default, "RTT_Test");
			widget->setCaption("Vertext mode");
			widget->eventWindowButtonPressed = MyGUI::newDelegate(notifyWindowButtonPressed);
		}

		if (widget == nullptr) return BaseManager::keyPressed( arg );

		if (arg.key == OIS::KC_1)
		{
			mCamera->setPolygonMode(Ogre::PM_SOLID);
		}
		else if (arg.key == OIS::KC_2)
		{
			mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else if (arg.key == OIS::KC_3)
		{
			mCamera->setPolygonMode(Ogre::PM_POINTS);
		}
		else if (arg.key == OIS::KC_4)
		{
			MyGUI::LayerManager::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
			while(layer.next())
			{
				if (layer->getName() == "RTT_Test")
				{
					//layer->castType<MyGUI::RTTLayer>()->setLayerNodeAnimation(&gCustomLayerNodeAnimation);
				}
			}
		}
		else if (arg.key == OIS::KC_SPACE)
		{
			MyGUI::LayerManager::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
			while(layer.next())
			{
				if (layer->getName() == "RTT_Test")
				{
					MyGUI::EnumeratorILayerNode node = layer->getEnumerator();
					while(node.next())
					{
						MyGUI::RTTLayerNode* rttnode = node->castType<MyGUI::RTTLayerNode>(false);
						if (rttnode != nullptr)
						{
							rttnode->setCacheUsing(!rttnode->getCacheUsing());

							if (rttnode->getCacheUsing())
							{
								/*if (rttnode->getLayerNodeAnimation() != nullptr)
								{
									//rttnode->setLayerNodeAnimation(nullptr);
									widget->setCaption("RTT mode");
								}
								else
								{
									//rttnode->setLayerNodeAnimation(&gCustomLayerNodeAnimation);
									widget->setCaption("Abstract mode");
								}*/
							}
							else
							{
								widget->setCaption("Vertext mode");
							}

						}
					}
				}
			}
		}

		return BaseManager::keyPressed( arg );
	}

} // namespace demo

MYGUI_APP(demo::DemoKeeper)

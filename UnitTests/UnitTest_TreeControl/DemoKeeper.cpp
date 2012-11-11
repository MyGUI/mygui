/*!
	@file
	@author     George Evmenov
	@date       08/2009
*/
#include "DemoKeeper.h"
#include "Base/Main.h"

#include "BaseLayout/BaseLayout.h"
#include "TreeControl.h"
#include "TreeControlItem.h"

#include "FileSystemInfo/FileSystemInfo.h"

// рутовая папка всей медиа
MyGUI::UString gMediaBase;
typedef std::pair<std::wstring, common::FileInfo> PairFileInfo;

class SampleLayout :
	public wraps::BaseLayout
{
public:
	SampleLayout();

	void notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode);
	MyGUI::UString getPath(MyGUI::TreeControl::Node* pNode) const;

private:
	MyGUI::TreeControl* mpResourcesTree;
};

SampleLayout* mSampleLayout;

SampleLayout::SampleLayout() : BaseLayout("SampleLayout.layout")
{
	assignWidget(mpResourcesTree, "ResourcesTree");
	mpResourcesTree->eventTreeNodePrepare += newDelegate(this, &SampleLayout::notifyTreeNodePrepare);


	MyGUI::TreeControl::Node* pRoot = mpResourcesTree->getRoot();

/*#ifdef MYGUI_OGRE_PLATFORM
	Ogre::ArchiveManager::ArchiveMapIterator ArchiveIterator = Ogre::ArchiveManager::getSingleton().getArchiveIterator();
	while (ArchiveIterator.hasMoreElements())
	{
		Ogre::Archive* pArchive = ArchiveIterator.getNext();

		MyGUI::TreeControl::Node* pNode = new MyGUI::TreeControl::Node(pArchive->getName(), "Data");
		pNode->setData(pArchive);
		pRoot->add(pNode);
	}
#else*/
	common::VectorFileInfo result;
	common::getSystemFileList(result, gMediaBase, L"*.*");

	for (common::VectorFileInfo::iterator item = result.begin(); item != result.end(); ++item)
	{
		if ((*item).name == L".." || (*item).name == L".")
			continue;
		MyGUI::TreeControl::Node* pNode = new MyGUI::TreeControl::Node((*item).name, "Data");

		pNode->setData(PairFileInfo(gMediaBase, *item));
		pRoot->add(pNode);
	}

//#endif
}

void SampleLayout::notifyTreeNodePrepare(MyGUI::TreeControl* pTreeControl, MyGUI::TreeControl::Node* pNode)
{
	if (pNode == pTreeControl->getRoot())
		return;

	pNode->removeAll();

/*#ifdef MYGUI_OGRE_PLATFORM
	Ogre::Archive* pArchive = *(pNode->getData<Ogre::Archive*>());

	MyGUI::UString strPath(getPath(pNode));
	Ogre::StringVectorPtr Resources = pArchive->find(strPath + "*", false, true);
	for (Ogre::StringVector::iterator Iterator = Resources->begin(); Iterator != Resources->end(); ++Iterator)
	{
		MyGUI::TreeControl::Node* pChild = new MyGUI::TreeControl::Node(*Iterator, "Folder");
		pChild->setData(pArchive);
		pNode->add(pChild);
	}

	Resources = pArchive->find(strPath + "*", false, false);
	for (Ogre::StringVector::iterator Iterator = Resources->begin(); Iterator != Resources->end(); ++Iterator)
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
#else*/
	PairFileInfo info = *(pNode->getData<PairFileInfo>());
	// если папка, то добавляем детей
	if (info.second.folder)
	{
		std::wstring path = info.first + L"/" + info.second.name;
		common::VectorFileInfo result;
		common::getSystemFileList(result, path, L"*.*");

		for (common::VectorFileInfo::iterator item = result.begin(); item != result.end(); ++item)
		{
			if ((*item).name == L".." || (*item).name == L".")
				continue;
			if ((*item).folder)
			{
				MyGUI::TreeControl::Node* pChild = new MyGUI::TreeControl::Node((*item).name, "Folder");
				pChild->setData(PairFileInfo(path, *item));
				pNode->add(pChild);
			}
			else
			{
				MyGUI::UString strName((*item).name);
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
				else if (strExtension == "mat" || strExtension == "material")
					strImage = "Material";
				else if (strExtension == "layout")
					strImage = "Layout";
				else if (strExtension == "ttf" || strExtension == "font" || strExtension == "fontdef")
					strImage = "Font";
				else if (strExtension == "txt" || strExtension == "text")
					strImage = "Text";
				else if (strExtension == "xml")
					strImage = "XML";
				else if (strExtension == "mesh")
					strImage = "Mesh";
				else if (strExtension == "htm" || strExtension == "html")
					strImage = "HTML";
				else
					strImage = "Unknown";

				MyGUI::TreeControl::Node* pChild = new MyGUI::TreeControl::Node((*item).name, strImage);
				pChild->setPrepared(true);
				pNode->add(pChild);
			}
		}
	}

//#endif
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

	void DemoKeeper::setupResources()
	{
		base::BaseManager::setupResources();
		addResourceLocation(getRootMedia() + "/UnitTests/UnitTest_TreeControl");
		addResourceLocation(getRootMedia() + "/Common/Tools");
		gMediaBase = getRootMedia();
	}

	void DemoKeeper::createScene()
	{
		base::BaseDemoManager::createScene();
		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		std::string widgetCategory = MyGUI::WidgetManager::getInstance().getCategoryName();
		factory.registerFactory<MyGUI::TreeControl>(widgetCategory);
		factory.registerFactory<MyGUI::TreeControlItem>(widgetCategory);

		MyGUI::ResourceManager::getInstance().load("FrameworkFonts.xml");
		MyGUI::ResourceManager::getInstance().load("TreeControlSkin.xml");
		MyGUI::ResourceManager::getInstance().load("TreeControlTemplate.xml");

		mSampleLayout = new SampleLayout();
	}

	void DemoKeeper::destroyScene()
	{
		delete mSampleLayout;
		mSampleLayout = nullptr;

		MyGUI::FactoryManager& factory = MyGUI::FactoryManager::getInstance();
		std::string widgetCategory = MyGUI::WidgetManager::getInstance().getCategoryName();
		factory.unregisterFactory<MyGUI::TreeControl>(widgetCategory);
		factory.unregisterFactory<MyGUI::TreeControlItem>(widgetCategory);
	}
} // namespace demo

MYGUI_APP(demo::DemoKeeper)

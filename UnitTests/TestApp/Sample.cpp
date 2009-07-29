
    SampleLayout::SampleLayout() : Layout("SampleLayout.layout")
    {
        assignWidget(mpResourcesTree, "ResourcesTree");
        mpResourcesTree->eventTreeNodePrepare = newDelegate(this, &SampleLayout::notifyTreeNodePrepare);

        TreeControl::Node* pRoot = mpResourcesTree->getRoot();
        Ogre::ArchiveManager::ArchiveMapIterator ArchiveIterator = Ogre::ArchiveManager::getSingleton().getArchiveIterator();
        while (ArchiveIterator.hasMoreElements())
        {
            Ogre::Archive* pArchive = ArchiveIterator.getNext();

            TreeControl::Node* pNode = new TreeControl::Node(pArchive->getName(), "Data");
            pNode->setData(pArchive);
            pRoot->add(pNode);
        }
    }

    void SampleLayout::notifyTreeNodePrepare(TreeControl* pTreeControl, TreeControl::Node* pNode)
    {
        if (pNode == pTreeControl->getRoot())
            return;

        pNode->removeAll();
        Ogre::Archive* pArchive = *(pNode->getData<Ogre::Archive*>());

        String strPath(getPath(pNode));
        Ogre::StringVectorPtr Resources = pArchive->find(strPath + "*", false, true);
        for (Ogre::StringVector::iterator Iterator = Resources->begin(); Iterator != Resources->end(); Iterator++)
        {
            TreeControl::Node* pChild = new TreeControl::Node(*Iterator, "Folder");
            pChild->setData(pArchive);
            pNode->add(pChild);
        }

        Resources = pArchive->find(strPath + "*", false, false);
        for (Ogre::StringVector::iterator Iterator = Resources->begin(); Iterator != Resources->end(); Iterator++)
        {
            String strName(*Iterator);
            String strExtension;
            size_t nPosition = strName.rfind(".");
            if (nPosition != String::npos)
            {
                strExtension = strName.substr(nPosition + 1);
                std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), tolower);
            }

            String strImage;
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

            TreeControl::Node* pChild = new TreeControl::Node(strName, strImage);
            pChild->setPrepared(true);
            pNode->add(pChild);
        }
    }

    String SampleLayout::getPath(TreeControl::Node* pNode) const
    {
        if (!pNode || pNode == mpResourcesTree->getRoot())
            return String();

        String strPath;
        while (pNode->getParent() != mpResourcesTree->getRoot())
        {
            strPath = pNode->getText() + "/" + strPath;
            pNode = pNode->getParent();
        }

        return strPath;
    }


#pragma once

namespace MyGUI
{
    template<class NODE, class OWNER>
    class GenericNode
    {
    public:
        typedef std::vector<NODE*> VectorGenericNodePtr;

        GenericNode();
        GenericNode(OWNER* pOwner);
        GenericNode(const MyGUI::UString& strText, NODE* pParent = NULL);
        virtual ~GenericNode();

        void add(NODE* pNode);
        void remove(NODE* pNode, bool bDelete = true);
        void removeAll(bool bDelete = true);

        bool hasAncestor(const NODE* pNode) const;
        bool hasDescendant(const NODE* pNode) const;
        bool hasChildren() const;
        const VectorGenericNodePtr& getChildren() const;
        VectorGenericNodePtr& getChildren();
        
        NODE* getParent() const;

        const MyGUI::UString& getText() const;
        void setText(const MyGUI::UString& strText);

        OWNER* getOwner() const;
        void setOwner(OWNER* pOwner);

        void invalidate();

    protected:
        NODE* mpParent;
        VectorGenericNodePtr mChildren;
        MyGUI::UString mstrText;
        OWNER* mpOwner;
    };

    template<class NODE, class OWNER>
    GenericNode<NODE, OWNER>::GenericNode(OWNER* pOwner) :
        mpOwner(pOwner),
        mpParent(NULL),
        mstrText("[ROOT]")
    {
        MYGUI_DEBUG_ASSERT(mpOwner, "GenericNode<NODE, OWNER>::GenericNode pOwner is NULL");
    }

    template<class NODE, class OWNER>
    GenericNode<NODE, OWNER>::GenericNode(const MyGUI::UString& strText, NODE* pParent) :
        mpParent(pParent),
        mstrText(strText)
    {
        if (pParent)
        {
            mpOwner = pParent->mpOwner;
            pParent->mChildren.push_back((NODE*)this);

            invalidate();
        }
        else
            mpOwner = NULL;
    }

    template<class NODE, class OWNER>
    GenericNode<NODE, OWNER>::~GenericNode()
    {
        if (mpParent)
            mpParent->remove((NODE*)this, false);

        while (!mChildren.empty())
        {
            NODE* pChild = mChildren.back();
            mChildren.pop_back();

            pChild->mpParent = NULL;
            delete pChild;
        }

        invalidate();
    }

    template<class NODE, class OWNER>
    void GenericNode<NODE, OWNER>::setText(const MyGUI::UString& strText)
    {
        mstrText = strText;

        invalidate();
    }

    template<class NODE, class OWNER>
    void GenericNode<NODE, OWNER>::setOwner(OWNER* pOwner)
    {
        mpOwner = pOwner;

        for (VectorGenericNodePtr::iterator Iterator = mChildren.begin(); Iterator != mChildren.end(); Iterator++)
            (*Iterator)->setOwner(pOwner);
    }

    template<class NODE, class OWNER>
    bool GenericNode<NODE, OWNER>::hasAncestor(const NODE* pNode) const
    {
        return (mpParent == pNode || (mpParent && mpParent->hasAncestor(pNode)));
    }

    template<class NODE, class OWNER>
    bool GenericNode<NODE, OWNER>::hasDescendant(const NODE* pNode) const
    {
        for (VectorGenericNodePtr::const_iterator Iterator = mChildren.begin(); Iterator != mChildren.end(); Iterator++)
        {
            if (*Iterator == pNode || (*Iterator)->hasDescendant(pNode))
                return true;
        }

        return false;
    }

    template<class NODE, class OWNER>
    void GenericNode<NODE, OWNER>::add(NODE* pNode)
    {
        MYGUI_DEBUG_ASSERT(pNode, "GenericNode<NODE, OWNER>::add pNode is NULL");

        if (pNode->mpParent)
            pNode->mpParent->remove(pNode, false);

        pNode->mpParent = (NODE*)this;
        pNode->setOwner(mpOwner);
        mChildren.push_back(pNode);

        invalidate();
    }

    template<class NODE, class OWNER>
    void GenericNode<NODE, OWNER>::remove(NODE* pNode, bool bDelete = true)
    {
        MYGUI_DEBUG_ASSERT(pNode, "GenericNode<NODE, OWNER>::remove pNode is NULL");

        for (VectorGenericNodePtr::iterator Iterator = mChildren.begin(); Iterator != mChildren.end(); Iterator++)
        {
            if (*Iterator != pNode)
                continue;

            mChildren.erase(Iterator);
            pNode->mpParent = NULL;

            if (bDelete)
                delete pNode;

            invalidate();

            return;
        }

        MYGUI_DEBUG_ASSERT(pNode, "GenericNode<NODE, OWNER>::remove pNode not child");
    }

    template<class NODE, class OWNER>
    void GenericNode<NODE, OWNER>::removeAll(bool bDelete = true)
    {
        while (!mChildren.empty())
        {
            NODE* pChild = mChildren.back();
            mChildren.pop_back();

            pChild->mpParent = NULL;
            if (bDelete)
                delete pChild;
        }

        invalidate();
    }

    template<class NODE, class OWNER>
    void GenericNode<NODE, OWNER>::invalidate()
    {
        if (mpOwner)
            mpOwner->invalidate();
    }

template<class NODE, class OWNER>
inline GenericNode<NODE, OWNER>::GenericNode()
    { mpParent = NULL; mpOwner = NULL; }
template<class NODE, class OWNER>
inline const std::vector<NODE*>& GenericNode<NODE, OWNER>::getChildren() const
    { return mChildren; }
template<class NODE, class OWNER>
inline std::vector<NODE*>& GenericNode<NODE, OWNER>::getChildren()
    { return mChildren; }
template<class NODE, class OWNER>
inline bool GenericNode<NODE, OWNER>::hasChildren() const
    { return !mChildren.empty(); }
template<class NODE, class OWNER>
inline const MyGUI::UString& GenericNode<NODE, OWNER>::getText() const
    { return mstrText; }
template<class NODE, class OWNER>
inline OWNER* GenericNode<NODE, OWNER>::getOwner() const
    { return mpOwner; }
template<class NODE, class OWNER>
NODE* GenericNode<NODE, OWNER>::getParent() const
    { return mpParent; }
}

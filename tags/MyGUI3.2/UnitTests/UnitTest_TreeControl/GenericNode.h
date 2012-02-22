/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/
#ifndef __GENERIC_NODE_H__
#define __GENERIC_NODE_H__

namespace MyGUI
{
	template<class NODE, class OWNER>
	class GenericNode
	{
	public:
		typedef std::vector<NODE*> VectorGenericNodePtr;

		GenericNode();
		GenericNode(OWNER* pOwner);
		GenericNode(const UString& strText, NODE* pParent = nullptr);
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

		const UString& getText() const;
		void setText(const UString& strText);

		OWNER* getOwner() const;
		void setOwner(OWNER* pOwner);

		void invalidate();

	protected:
		NODE* mpParent;
		VectorGenericNodePtr mChildren;
		UString mstrText;
		OWNER* mpOwner;
	};

	template<class NODE, class OWNER>
	GenericNode<NODE, OWNER>::GenericNode(OWNER* pOwner) :
		mpParent(nullptr),
		mstrText("[ROOT]"),
		mpOwner(pOwner)
	{
		MYGUI_DEBUG_ASSERT(mpOwner, "GenericNode<NODE, OWNER>::GenericNode pOwner is nullptr");
	}

	template<class NODE, class OWNER>
	GenericNode<NODE, OWNER>::GenericNode(const UString& strText, NODE* pParent) :
		mpParent(pParent),
		mstrText(strText)
	{
		if (pParent)
		{
			mpOwner = pParent->mpOwner;
			pParent->mChildren.push_back(static_cast<NODE*>(this));

			invalidate();
		}
		else
			mpOwner = nullptr;
	}

	template<class NODE, class OWNER>
	GenericNode<NODE, OWNER>::~GenericNode()
	{
		if (mpParent)
			mpParent->remove(static_cast<NODE*>(this), false);

		while (!mChildren.empty())
		{
			NODE* pChild = mChildren.back();
			mChildren.pop_back();

			pChild->mpParent = nullptr;
			delete pChild;
		}

		invalidate();
	}

	template<class NODE, class OWNER>
	void GenericNode<NODE, OWNER>::setText(const UString& strText)
	{
		mstrText = strText;

		invalidate();
	}

	template<class NODE, class OWNER>
	void GenericNode<NODE, OWNER>::setOwner(OWNER* pOwner)
	{
		mpOwner = pOwner;

		for (typename VectorGenericNodePtr::iterator Iterator = mChildren.begin(); Iterator != mChildren.end(); ++Iterator)
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
		for (typename VectorGenericNodePtr::const_iterator Iterator = mChildren.begin(); Iterator != mChildren.end(); ++Iterator)
		{
			if (*Iterator == pNode || (*Iterator)->hasDescendant(pNode))
				return true;
		}

		return false;
	}

	template<class NODE, class OWNER>
	void GenericNode<NODE, OWNER>::add(NODE* pNode)
	{
		MYGUI_DEBUG_ASSERT(pNode, "GenericNode<NODE, OWNER>::add pNode is nullptr");

		if (pNode->mpParent)
			pNode->mpParent->remove(pNode, false);

		pNode->mpParent = static_cast<NODE*>(this);
		pNode->setOwner(mpOwner);
		mChildren.push_back(pNode);

		invalidate();
	}

	template<class NODE, class OWNER>
	void GenericNode<NODE, OWNER>::remove(NODE* pNode, bool bDelete)
	{
		MYGUI_DEBUG_ASSERT(pNode, "GenericNode<NODE, OWNER>::remove pNode is nullptr");

		for (typename VectorGenericNodePtr::iterator Iterator = mChildren.begin(); Iterator != mChildren.end(); ++Iterator)
		{
			if (*Iterator != pNode)
				continue;

			mChildren.erase(Iterator);
			pNode->mpParent = nullptr;

			if (bDelete)
				delete pNode;

			invalidate();

			return;
		}

		MYGUI_DEBUG_ASSERT(pNode, "GenericNode<NODE, OWNER>::remove pNode not child");
	}

	template<class NODE, class OWNER>
	void GenericNode<NODE, OWNER>::removeAll(bool bDelete)
	{
		while (!mChildren.empty())
		{
			NODE* pChild = mChildren.back();
			mChildren.pop_back();

			pChild->mpParent = nullptr;
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
	{
		mpParent = nullptr;
		mpOwner = nullptr;
	}
	template<class NODE, class OWNER>
	inline const std::vector<NODE*>& GenericNode<NODE, OWNER>::getChildren() const
	{
		return mChildren;
	}
	template<class NODE, class OWNER>
	inline std::vector<NODE*>& GenericNode<NODE, OWNER>::getChildren()
	{
		return mChildren;
	}
	template<class NODE, class OWNER>
	inline bool GenericNode<NODE, OWNER>::hasChildren() const
	{
		return !mChildren.empty();
	}
	template<class NODE, class OWNER>
	inline const UString& GenericNode<NODE, OWNER>::getText() const
	{
		return mstrText;
	}
	template<class NODE, class OWNER>
	inline OWNER* GenericNode<NODE, OWNER>::getOwner() const
	{
		return mpOwner;
	}
	template<class NODE, class OWNER>
	NODE* GenericNode<NODE, OWNER>::getParent() const
	{
		return mpParent;
	}
}

#endif // __GENERIC_NODE_H__

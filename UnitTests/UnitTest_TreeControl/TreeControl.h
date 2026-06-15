/*!
	@file
	@author     Pavel Turin
	@date       08/2009
*/
#ifndef TREE_CONTROL_H_
#define TREE_CONTROL_H_

#include "GenericNode.h"

namespace MyGUI
{
	class TreeControlItem;

	class TreeControl : public Widget
	{
		MYGUI_RTTI_DERIVED(TreeControl)

	public:
		class Node;

		using EventHandle_TreeControlPtrNodePtr = delegates::MultiDelegate<TreeControl*, Node*>;
		using EventHandle_TreeControlPtrSizeT = delegates::MultiDelegate<TreeControl*, size_t>;

		class Node : public GenericNode<Node, TreeControl>
		{
		public:
			Node();
			Node(TreeControl* pOwner);
			Node(const UString& strText, Node* pParent = nullptr);
			Node(const UString& strText, const UString& strImage, Node* pParent = nullptr);

			bool isPrepared() const;
			void setPrepared(bool bIsPrepared);
			void prepare();
			size_t prepareChildren();

			bool isExpanded() const;
			void setExpanded(bool bIsExpanded);

			const UString& getImage() const;
			void setImage(const UString& strImage);

			void setData(Any Data);
			template<typename TYPE>
			TYPE* getData() const;

		private:
			bool mbIsPrepared;
			bool mbIsExpanded;
			UString mstrImage;
			Any mData;
		};

		using VectorNodePtr = Node::VectorGenericNodePtr;

		Node* getRoot() const;
		void setRootVisible(bool bValue);
		bool isRootVisible() const;

		Node* getSelection() const;
		void setSelection(Node* pSelection);

		void invalidate();

		void setSize(const IntSize& Size) override;
		void setCoord(const IntCoord& Bounds) override;

		EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseSetFocus;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseLostFocus;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeSelected;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeActivated;
		EventHandle_TreeControlPtrNodePtr eventTreeNodeContextMenu;
		EventHandle_TreeControlPtrNodePtr eventTreeNodePrepare;
		EventHandle_TreeControlPtrSizeT eventTreeScrolled;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void notifyMousePressed(Widget* pSender, int nLeft, int nTop, MouseButton nID);
		void notifyMouseWheel(Widget* pSender, int nValue);
		void notifyMouseDoubleClick(Widget* pSender);
		void notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget);
		void notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget);
		void notifyScrollChangePosition(ScrollBar* pSender, size_t nPosition);
		void notifyExpandCollapse(Widget* pSender);
		void notifyFrameEntered(float nTime);

		void onMouseWheel(int nValue) override;
		void onKeyButtonPressed(KeyCode Key, Char Character) override;

	private:
		using VectorTreeItemPtr = std::vector<TreeControlItem*>;

		void validate();

		void updateScroll();
		void updateItems();

		void scrollTo(size_t nPosition);
		void sendScrollingEvents(size_t nPosition);

	private:
		ScrollBar* mpWidgetScroll{nullptr};
		VectorTreeItemPtr mItemWidgets;
		UString mstrSkinLine;
		bool mbScrollAlwaysVisible{true};
		bool mbInvalidated{false};
		bool mbRootVisible{false};
		int mnItemHeight{1};
		int mnScrollRange{-1};
		int mnTopIndex{0};
		int mnTopOffset{0};
		size_t mnFocusIndex{ITEM_NONE};
		Node* mpSelection{nullptr};
		Node* mpRoot{nullptr};
		size_t mnExpandedNodes{0};
		int mnLevelOffset{0};
	};


	inline TreeControl::Node::Node() :
		mbIsPrepared(false),
		mbIsExpanded(true)
	{
	}
	inline bool TreeControl::Node::isPrepared() const
	{
		return mbIsPrepared;
	}
	inline void TreeControl::Node::setPrepared(bool bIsPrepared)
	{
		mbIsPrepared = bIsPrepared;
	}
	inline bool TreeControl::Node::isExpanded() const
	{
		return mbIsExpanded;
	}
	inline const UString& TreeControl::Node::getImage() const
	{
		return mstrImage;
	}
	inline void TreeControl::Node::setData(Any Data)
	{
		mData = std::move(Data);
	}
	template<typename TYPE>
	TYPE* TreeControl::Node::getData() const
	{
		return mData.castType<TYPE>(true);
	}

	inline TreeControl::Node* TreeControl::getRoot() const
	{
		return mpRoot;
	}
	inline bool TreeControl::isRootVisible() const
	{
		return mbRootVisible;
	}
	inline TreeControl::Node* TreeControl::getSelection() const
	{
		return mpSelection;
	}
}

#endif // TREE_CONTROL_H_

/*!
	@file
	@author     Pavel Turin
	@date       08/2009
	@module
*/
#ifndef __TREE_CONTROL_H__
#define __TREE_CONTROL_H__

#include "GenericNode.h"

namespace MyGUI
{
    class TreeControlItem;

    class TreeControl : public Widget
    {
        MYGUI_RTTI_DERIVED( TreeControl )

    public:
        class Node;

        typedef delegates::CDelegate2<TreeControl*, Node*> EventHandle_TreeControlPtrNodePtr;
        typedef delegates::CDelegate2<TreeControl*, size_t> EventHandle_TreeControlPtrSizeT;

        class Node : public GenericNode<Node, TreeControl>
        {
        public:
            Node();
            Node(TreeControl* pOwner);
			Node(const UString& strText, Node* pParent = nullptr);
            Node(const UString& strText, const UString& strImage, Node* pParent = nullptr);
            virtual ~Node();

            bool isPrepared() const;
            void setPrepared(bool bIsPrepared);
            void prepare();
            size_t prepareChildren();

            bool isExpanded() const;
            void setExpanded(bool bIsExpanded);

            const UString& getImage() const;
            void setImage(const UString& strImage);

            void setData(Any Data);
            template <typename TYPE>
            TYPE* getData() const;

        private:
            bool mbIsPrepared;
            bool mbIsExpanded;
            UString mstrImage;
            Any mData;
        };

        typedef Node::VectorGenericNodePtr VectorNodePtr;

        TreeControl();

        virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, const std::string& _name);

        Node* getRoot() const;
        void setRootVisible(bool bValue);
        bool isRootVisible() const;

        Node* getSelection() const;
        void setSelection(Node* pSelection);

        void invalidate();

        virtual void setSize(const IntSize& Size);
        virtual void setCoord(const IntCoord& Bounds);

        EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseSetFocus;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseLostFocus;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeSelected;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeActivated;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeContextMenu;
        EventHandle_TreeControlPtrNodePtr eventTreeNodePrepare;
        EventHandle_TreeControlPtrSizeT eventTreeScrolled;

    protected:
        virtual ~TreeControl();

        void notifyMousePressed(Widget* pSender, int nLeft, int nTop, MouseButton nID);
        void notifyMouseWheel(Widget* pSender, int nValue);
        void notifyMouseDoubleClick(Widget* pSender);
        void notifyMouseSetFocus(Widget* pSender, Widget* pPreviousWidget);
        void notifyMouseLostFocus(Widget* pSender, Widget* pNextWidget);
        void notifyScrollChangePosition(VScroll* pSender, size_t nPosition);
        void notifyExpandCollapse(Widget* pSender);
        void notifyFrameEntered(float nTime);

        virtual void onMouseWheel(int nValue);
        virtual void onKeySetFocus(Widget* pWidget);
        virtual void onKeyLostFocus(Widget* pWidget);
        virtual void onKeyButtonPressed(KeyCode Key, Char Character);

        virtual void baseChangeWidgetSkin(ResourceSkin* pSkinInformation);

    private:
        typedef std::vector<TreeControlItem*> VectorTreeItemPtr;

        void initialiseWidgetSkin(ResourceSkin* pSkinInformation);
        void shutdownWidgetSkin();

        void validate();

        void updateState();
        void updateScroll();
        void updateItems();

        void scrollTo(size_t nPosition);
        void sendScrollingEvents(size_t nPosition);

        VScroll* mpWidgetScroll;
        VectorTreeItemPtr mItemWidgets;
        UString mstrSkinLine;
        bool mbScrollAlwaysVisible;
        bool mbHasFocus;
        bool mbInvalidated;
        bool mbRootVisible;
        int mnItemHeight;
        int mnScrollRange;
        int mnTopIndex;
        int mnTopOffset;
        int mnFocusIndex;
        Node* mpSelection;
        Node* mpRoot;
        size_t mnExpandedNodes;
    };


inline TreeControl::Node::Node()
    { /* Random Initialization */ }
inline bool TreeControl::Node::isPrepared() const
    { return mbIsPrepared; }
inline void TreeControl::Node::setPrepared(bool bIsPrepared)
    { mbIsPrepared = bIsPrepared; }
inline bool TreeControl::Node::isExpanded() const
    { return mbIsExpanded; }
inline const UString& TreeControl::Node::getImage() const
    { return mstrImage; }
inline void TreeControl::Node::setData(Any Data)
    { mData = Data; }
template <typename TYPE>
TYPE* TreeControl::Node::getData() const
    { return mData.castType<TYPE>(true); }

inline TreeControl::Node* TreeControl::getRoot() const
    { return mpRoot; }
inline bool TreeControl::isRootVisible() const
    { return mbRootVisible; }
inline TreeControl::Node* TreeControl::getSelection() const
    { return mpSelection; }
}

#endif // __TREE_CONTROL_H__

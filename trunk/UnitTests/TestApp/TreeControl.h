
#pragma once

#include "GenericNode.h"

namespace MyGUI
{
    class TreeControlItem;

    class TreeControl : public MyGUI::Widget
    {
        MYGUI_RTTI_DERIVED(TreeControl);

    public:
        class Node;

        typedef MyGUI::delegates::CDelegate2<TreeControl*, Node*> EventHandle_TreeControlPtrNodePtr;
        typedef MyGUI::delegates::CDelegate2<TreeControl*, size_t> EventHandle_TreeControlPtrSizeT;

        class Node : public GenericNode<Node, TreeControl>
        {
        public:
            Node();
            Node(TreeControl* pOwner);
			Node(const UString& strText, Node* pParent = NULL);
            Node(const UString& strText, const UString& strImage, Node* pParent = NULL);
            virtual ~Node();

            bool isPrepared() const;
            void setPrepared(bool bIsPrepared);
            void prepare();
            size_t prepareChildren();

            bool isExpanded() const;
            void setExpanded(bool bIsExpanded);

            const UString& getImage() const;
            void setImage(const UString& strImage);

            void setData(MyGUI::Any Data);
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

        virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

        Node* getRoot() const;
        void setRootVisible(bool bValue);
        bool isRootVisible() const;

        Node* getSelection() const;
        void setSelection(Node* pSelection);

        void invalidate();

        virtual void setSize(const MyGUI::IntSize& Size);
        virtual void setCoord(const MyGUI::IntCoord& Bounds);

        EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseSetFocus;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeMouseLostFocus;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeSelected;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeActivated;
        EventHandle_TreeControlPtrNodePtr eventTreeNodeContextMenu;
        EventHandle_TreeControlPtrNodePtr eventTreeNodePrepare;
        EventHandle_TreeControlPtrSizeT eventTreeScrolled;

    protected:
        virtual ~TreeControl();

        void notifyMousePressed(MyGUI::WidgetPtr pSender, int nLeft, int nTop, MyGUI::MouseButton nID);
        void notifyMouseWheel(MyGUI::WidgetPtr pSender, int nValue);
        void notifyMouseDoubleClick(MyGUI::WidgetPtr pSender);
        void notifyMouseSetFocus(MyGUI::WidgetPtr pSender, MyGUI::WidgetPtr pPreviousWidget);
        void notifyMouseLostFocus(MyGUI::WidgetPtr pSender, MyGUI::WidgetPtr pNextWidget);
        void notifyScrollChangePosition(MyGUI::VScrollPtr pSender, size_t nPosition);
        void notifyExpandCollapse(MyGUI::WidgetPtr pSender);
        void notifyFrameEntered(float nTime);

        virtual void onMouseWheel(int nValue);
        virtual void onKeySetFocus(MyGUI::WidgetPtr pWidget);
        virtual void onKeyLostFocus(MyGUI::WidgetPtr pWidget);
        virtual void onKeyButtonPressed(MyGUI::KeyCode Key, MyGUI::Char Character);

        virtual void baseChangeWidgetSkin(MyGUI::ResourceSkin* pSkinInformation);

    private:
        typedef std::vector<TreeControlItem*> VectorTreeItemPtr;

        void initialiseWidgetSkin(MyGUI::ResourceSkin* pSkinInformation);
        void shutdownWidgetSkin();

        void validate();

        void updateState();
        void updateScroll();
        void updateItems();

        void scrollTo(size_t nPosition);
        void sendScrollingEvents(size_t nPosition);

        MyGUI::VScrollPtr mpWidgetScroll;
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
inline void TreeControl::Node::setData(MyGUI::Any Data)
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
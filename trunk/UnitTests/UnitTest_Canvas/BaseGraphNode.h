/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __BASE_GRAPH_NODE_H__
#define __BASE_GRAPH_NODE_H__

#include <MyGUI.h>
#include "BaseGraphConnection.h"

namespace wraps
{

	class IGraphController
	{
	public:
		virtual void eraseView() = 0;
		virtual void startDrag(BaseGraphConnection* _node) = 0;
		virtual void stopDrag(BaseGraphConnection* _node) = 0;
		virtual void updateDrag(BaseGraphConnection* _node) = 0;
	};

	class BaseGraphNode : public BaseLayout
	{
	public:
		BaseGraphNode(const std::string& _layout) :
			mLayout(_layout),
	  		BaseLayout("", nullptr),
			mView(nullptr)
		{
		}

		// все точки данного узла
		size_t getConnectionCount() { return mListConnection.size(); }
		EnumeratorConnection getConnectionEnumerator() { return EnumeratorConnection(mListConnection); }

	/*internal:*/
		void _initialise(MyGUI::WidgetPtr _parent, IGraphController* _view)
		{
			mView = _view;
			if ( ! mLayout.empty()) {
				BaseLayout::initialise(mLayout, _parent);
			}
			initialise();

			MyGUI::WindowPtr window = mMainWidget->castType<MyGUI::Window>(false);
			if (window != nullptr)
			{
				window->eventWindowChangeCoord = MyGUI::newDelegate(this, &BaseGraphNode::notifyWindowChangeCoord);
			}
		}

		void _shutdown()
		{
			removeConnections();
			BaseLayout::shutdown();
			shutdown();
		}

	protected:
		virtual void initialise() = 0;
		virtual void shutdown() = 0;

		void notifyWindowChangeCoord(MyGUI::WindowPtr _sender)
		{
			mView->eraseView();
		}

		template <typename T>
		void assignBase(T * & _widget, const std::string & _name, bool _throw = true)
		{
			BaseLayout::assignBase<T>(_widget, _name, _throw);
			mListConnection.push_back(_widget);
			addConnection(_widget);
		}

	private:
		void addConnection(BaseGraphConnection* _connection)
		{
			_connection->_setOwnerNode(this);
			_connection->_getMainWidget()->eventMouseButtonPressed = MyGUI::newDelegate(this, &BaseGraphNode::notifyMouseButtonPressed);
			_connection->_getMainWidget()->eventMouseButtonReleased = MyGUI::newDelegate(this, &BaseGraphNode::notifyMouseButtonReleased);
			_connection->_getMainWidget()->eventMouseDrag = MyGUI::newDelegate(this, &BaseGraphNode::notifyMouseDrag);
			_connection->_getMainWidget()->setUserData(_connection);
		}

		void removeConnections()
		{
			for (size_t index=0; index<mListConnection.size(); ++index)
			{
				mListConnection[index];
			}
		}

		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Left)
			{
				mView->startDrag(*_sender->getUserData<BaseGraphConnection*>());
			}
		}

		void notifyMouseButtonReleased(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			mView->stopDrag(*_sender->getUserData<BaseGraphConnection*>());
		}

		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
		{
			mView->updateDrag(*_sender->getUserData<BaseGraphConnection*>());
		}

	private:
		std::string mLayout;
		VectorConnection mListConnection;
		IGraphController* mView;
	};

} // namespace wraps

#endif // __BASE_GRAPH_NODE_H__

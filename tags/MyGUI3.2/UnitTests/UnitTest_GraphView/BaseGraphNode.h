/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __BASE_GRAPH_NODE_H__
#define __BASE_GRAPH_NODE_H__

#include <MyGUI.h>
#include "BaseGraphConnection.h"
#include "IGraphController.h"

namespace wraps
{

	class BaseGraphNode :
		public BaseLayout
	{
	public:
		BaseGraphNode(const std::string& _layout) :
			BaseLayout("", nullptr),
			mLayout(_layout),
			mView(nullptr)
		{
		}

		// энумератор всех точек узла
		EnumeratorConnection getConnectionEnumerator() const
		{
			return EnumeratorConnection(mListConnection);
		}

		bool isAnyConnection() const
		{
			EnumeratorConnection point = getConnectionEnumerator();
			while (point.next())
			{
				if (point->isAnyConnection())
					return true;
			}
			return false;
		}

		BaseGraphConnection* getConnectionByName(const std::string& _name, const std::string& _type = "")
		{
			EnumeratorConnection point = getConnectionEnumerator();
			while (point.next())
			{
				if (point->getName() == _name && (_type.empty() || point->getType() == _type))
				{
					return point.current();
				}
			}
			return nullptr;
		}

		const MyGUI::IntCoord& getCoord()
		{
			return mMainWidget->getCoord();
		}

		void setCoord(const MyGUI::IntCoord& _coord)
		{
			mMainWidget->setCoord(_coord);
			mView->changePosition(this);
		}

		MyGUI::IntPoint getPosition()
		{
			return mMainWidget->getPosition();
		}

		void setPosition(const MyGUI::IntPoint& _point)
		{
			mMainWidget->setPosition(_point);
			mView->changePosition(this);
		}

		const MyGUI::IntPoint& getAbsolutePosition()
		{
			return mMainWidget->getAbsolutePosition();
		}

		void setAbsolutePosition(const MyGUI::IntPoint& _point)
		{
			setPosition(MyGUI::IntPoint(_point.left - mMainWidget->getParent()->getAbsoluteLeft(), _point.top - mMainWidget->getParent()->getAbsoluteTop()));
		}

	/*internal:*/
		void _initialise(MyGUI::Widget* _parent, IGraphController* _view)
		{
			mView = _view;
			if ( ! mLayout.empty())
			{
				BaseLayout::initialise(mLayout, _parent);
			}
			initialise();

			MyGUI::Window* window = mMainWidget->castType<MyGUI::Window>(false);
			if (window != nullptr)
			{
				window->eventWindowChangeCoord += MyGUI::newDelegate(this, &BaseGraphNode::notifyWindowChangeCoord);
				window->eventWindowButtonPressed += MyGUI::newDelegate(this, &BaseGraphNode::notifyWindowButtonPressed);
			}

			// перекрывающийся стиль
			mMainWidget->setWidgetStyle(MyGUI::WidgetStyle::Overlapped);
		}

		void _shutdown()
		{
			BaseLayout::shutdown();
			shutdown();
		}

	protected:
		virtual void initialise() = 0;
		virtual void shutdown() = 0;

		void notifyWindowChangeCoord(MyGUI::Window* _sender)
		{
			MyGUI::IntCoord coord = _sender->getCoord();
			if ((coord.left < 0) || (coord.top < 0))
			{
				if (coord.left < 0) coord.left = 0;
				if (coord.top < 0) coord.top = 0;
				_sender->setCoord(coord);
			}

			mView->changePosition(this);
		}

		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name)
		{
			if (_name == "close")
				mView->close(this);
		}

		template <typename T>
		void assignBase(T * & _widget, const std::string& _name, bool _throw = true)
		{
			BaseLayout::assignBase<T>(_widget, _name, _throw);
			mListConnection.push_back(_widget);
			addConnection(_widget);
		}

	private:
		void addConnection(BaseGraphConnection* _connection)
		{
			_connection->_setOwnerNode(this);
			_connection->_getMainWidget()->eventMouseButtonPressed += MyGUI::newDelegate(this, &BaseGraphNode::notifyMouseButtonPressed);
			_connection->_getMainWidget()->eventMouseButtonReleased += MyGUI::newDelegate(this, &BaseGraphNode::notifyMouseButtonReleased);
			_connection->_getMainWidget()->eventMouseDrag += MyGUI::newDelegate(this, &BaseGraphNode::notifyMouseDrag);
			_connection->_getMainWidget()->setUserData(_connection);
		}

		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Left)
				mView->startDrag(*_sender->getUserData<BaseGraphConnection*>());
		}

		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			mView->stopDrag(*_sender->getUserData<BaseGraphConnection*>());
		}

		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
		{
			if (_id == MyGUI::MouseButton::Left)
				mView->updateDrag(*_sender->getUserData<BaseGraphConnection*>());
		}

	private:
		std::string mLayout;
		VectorConnection mListConnection;
		IGraphController* mView;
	};

} // namespace wraps

#endif // __BASE_GRAPH_NODE_H__

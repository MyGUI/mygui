/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef BASE_GRAPH_CONNECTION_H_
#define BASE_GRAPH_CONNECTION_H_

#include <MyGUI.h>
#include <BaseLayout/BaseLayout.h>

namespace wraps
{
	class BaseGraphNode;
	class BaseGraphConnection;
	typedef std::vector<BaseGraphConnection*> VectorConnection;
	typedef MyGUI::Enumerator<VectorConnection> EnumeratorConnection;

	class BaseGraphConnection :
		public BaseLayout
	{
	public:
		BaseGraphConnection(MyGUI::Widget* _widget) :
			BaseLayout("", _widget),
			mOwnerNode(nullptr)
		{
			mType = mMainWidget->getUserString("Type");
			mName = mMainWidget->getUserString("Name");

			const int offset = 30;
			mOffset = MyGUI::IntSize::parse(mMainWidget->getUserString("Direction"));
			mOffset.width *= offset;
			mOffset.height *= offset;
		}

		BaseGraphNode* getOwnerNode() const
		{
			return mOwnerNode;
		}
		const std::string& getType() const
		{
			return mType;
		}
		const std::string& getName() const
		{
			return mName;
		}

		MyGUI::IntCoord getAbsoluteCoord() const
		{
			return mMainWidget->getAbsoluteCoord();
		}
		MyGUI::IntSize getOffset() const
		{
			return mOffset;
		}

		// все присоединениые к нам точки
		bool isAnyConnection() const
		{
			return !mConnection.empty();
		}
		EnumeratorConnection getConnectionEnumerator() const
		{
			return EnumeratorConnection(mConnection);
		}

		bool isAnyReverseConnection() const
		{
			return !mReverseConnection.empty();
		}
		EnumeratorConnection getReverseConnectionEnumerator() const
		{
			return EnumeratorConnection(mReverseConnection);
		}

		void addConnectionPoint(BaseGraphConnection* _point)
		{
			mConnection.push_back(_point);
			_point->_addReverseConnectionPoint(this);
		}

		void removeConnectionPoint(BaseGraphConnection* _point)
		{
			VectorConnection::iterator item = std::find(mConnection.begin(), mConnection.end(), _point);
			MYGUI_ASSERT(item != mConnection.end(), "Item not found");
			mConnection.erase(item);
			_point->_removeReverseConnectionPoint(this);
		}

	/*internal:*/
		void _setOwnerNode(BaseGraphNode* _owner)
		{
			mOwnerNode = _owner;
		}

		MyGUI::Widget* _getMainWidget() const
		{
			return mMainWidget;
		}

		void _addReverseConnectionPoint(BaseGraphConnection* _point)
		{
			mReverseConnection.push_back(_point);
		}

		void _removeReverseConnectionPoint(BaseGraphConnection* _point)
		{
			VectorConnection::iterator item = std::find(mReverseConnection.begin(), mReverseConnection.end(), _point);
			MYGUI_ASSERT(item != mReverseConnection.end(), "Item not found");
			mReverseConnection.erase(item);
		}

	private:
		BaseGraphNode* mOwnerNode;
		std::string mType;
		std::string mName;
		VectorConnection mConnection;
		VectorConnection mReverseConnection;
		MyGUI::IntSize mOffset;

	};

}

#endif // BASE_GRAPH_CONNECTION_H_

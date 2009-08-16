/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __BASE_GRAPH_CONNECTION_H__
#define __BASE_GRAPH_CONNECTION_H__

#include <MyGUI.h>
#include <BaseLayout/BaseLayout.h>

namespace wraps
{
	class BaseGraphNode;
	class BaseGraphConnection;
	typedef std::vector<BaseGraphConnection*> VectorConnection;
	typedef MyGUI::Enumerator<VectorConnection> EnumeratorConnection;

	class BaseGraphConnection : public BaseLayout
	{
	public:
		BaseGraphConnection(MyGUI::WidgetPtr _widget) :
	  		BaseLayout("", _widget),
			mOwnerNode(nullptr)
		{
			mType = mMainWidget->getUserString("Type");

			const int offset = 30;
			mOffset = MyGUI::IntSize::parse(mMainWidget->getUserString("Direction"));
			mOffset.width *= offset;
			mOffset.height *= offset;
		}

		BaseGraphNode* getOwnerNode() { return mOwnerNode; }
		const std::string& getConnectionType() { return mType; }

		MyGUI::IntCoord getAbsoluteCoord() { return mMainWidget->getAbsoluteCoord(); }
		MyGUI::IntSize getOffset() { return mOffset; }

		// все присоединениые к нам точки
		size_t getConnectionCount() { return mConnection.size(); }
		EnumeratorConnection getConnectionEnumerator() { return EnumeratorConnection(mConnection); }

		size_t getReverseConnectionCount() { return mReverseConnection.size(); }
		EnumeratorConnection getReverseConnectionEnumerator() { return EnumeratorConnection(mReverseConnection); }

		void addConnectionPoint(BaseGraphConnection* _point)
		{
			mConnection.push_back(_point);
			_point->_addReverseConnectionPoint(this);
		}

		void removeConnectionPoint(BaseGraphConnection* _point)
		{
			mConnection.erase(std::find(mConnection.begin(), mConnection.end(), _point));
			_point->_removeReverseConnectionPoint(this);
		}

	/*internal:*/
		void _setOwnerNode(BaseGraphNode* _owner) { mOwnerNode = _owner; }

		MyGUI::WidgetPtr _getMainWidget() { return mMainWidget; }

		void _addReverseConnectionPoint(BaseGraphConnection* _point)
		{
			mReverseConnection.push_back(_point);
		}

		void _removeReverseConnectionPoint(BaseGraphConnection* _point)
		{
			mReverseConnection.erase(std::find(mReverseConnection.begin(), mReverseConnection.end(), _point));
		}

	private:
		BaseGraphNode* mOwnerNode;
		std::string mType;
		VectorConnection mConnection;
		VectorConnection mReverseConnection;
		MyGUI::IntSize mOffset;

	};

}

#endif // __BASE_GRAPH_CONNECTION_H__

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
		BaseGraphConnection(MyGUI::WidgetPtr _widget, const std::string& _type) :
	  		BaseLayout("", _widget),
			mOwnerNode(nullptr),
			mType(_type),
			mDirectIn(false),
			mDirectOut(false)
		{
			// высчитываем, куда у нас смотрит узел
			int x = mMainWidget->getLeft() + (mMainWidget->getWidth() / 2);
			int y = mMainWidget->getTop() + (mMainWidget->getHeight() / 2);

			const int tolerance = 15;
			const int offset = 30;

			if (x < tolerance)
			{
				mOffset.width = -offset;
				mDirectIn = true;
			}
			else if (mMainWidget->getParent()->getWidth() - tolerance < x)
			{
				mOffset.width = offset;
				mDirectOut = true;
			}
			if (y < tolerance) mOffset.height = -offset;
			else if (mMainWidget->getParent()->getHeight() - tolerance < y) mOffset.height = offset;
		}

		BaseGraphNode* getOwnerNode() { return mOwnerNode; }
		const std::string& getConnectionType() { return mType; }

		MyGUI::IntCoord getAbsoluteCoord() { return mMainWidget->getAbsoluteCoord(); }
		MyGUI::IntSize getOffset() { return mOffset; }

		bool isDirectIn() { return mDirectIn; }
		void setDirectIn(bool _direct) { mDirectIn = _direct; }

		bool isDirectOut() { return mDirectOut; }
		void setDirectOut(bool _direct) { mDirectOut = _direct; }

		// все присоединениые к нам точки
		size_t getConnectionCount() { return mConnection.size(); }
		EnumeratorConnection getConnectionEnumerator() { return EnumeratorConnection(mConnection); }

		void addConnectionPoint(BaseGraphConnection* _point)
		{
			mConnection.push_back(_point);
		}

	/*internal:*/
		void _setOwnerNode(BaseGraphNode* _owner) { mOwnerNode = _owner; }
		MyGUI::WidgetPtr _getMainWidget() { return mMainWidget; }

	private:
		BaseGraphNode* mOwnerNode;
		std::string mType;
		VectorConnection mConnection;
		MyGUI::IntSize mOffset;
		bool mDirectIn;
		bool mDirectOut;

	};

}

#endif // __BASE_GRAPH_CONNECTION_H__

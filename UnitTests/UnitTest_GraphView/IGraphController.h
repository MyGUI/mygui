/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef I_GRAPH_CONTROLLER_H_
#define I_GRAPH_CONTROLLER_H_

namespace wraps
{

	class BaseGraphConnection;
	class BaseGraphNode;

	class IGraphController
	{
	public:
		virtual void startDrag(BaseGraphConnection* _connection) = 0;
		virtual void stopDrag(BaseGraphConnection* _connection) = 0;
		virtual void updateDrag(BaseGraphConnection* _connection) = 0;
		virtual void changePosition(BaseGraphNode* _node) = 0;
		virtual void close(BaseGraphNode* _node) = 0;
	};

} // namespace wraps

#endif // I_GRAPH_CONTROLLER_H_

/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef __I_GRAPH_CONTROLLER_H__
#define __I_GRAPH_CONTROLLER_H__

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

#endif // __I_GRAPH_CONTROLLER_H__

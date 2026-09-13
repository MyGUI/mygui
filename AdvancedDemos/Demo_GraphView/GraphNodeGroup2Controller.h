/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef GRAPH_NODE_GROUP2_CONTROLLER_H_
#define GRAPH_NODE_GROUP2_CONTROLLER_H_

#include <MyGUI.h>
#include "BaseAnimationNode.h"

namespace demo
{

	class GraphNodeGroup2Controller : public BaseAnimationNode
	{
	public:
		GraphNodeGroup2Controller(std::string_view _name) :
			BaseAnimationNode("GraphNodeGroup2Controller.layout", "Group2Controller", _name)
		{
		}

	private:
		void initialise() override
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());

			assignBase(mStart1In, "Start1In");
			assignBase(mStart2In, "Start2In");
			assignBase(mStart1Out, "Start1Out");
			assignBase(mStop1Out, "Stop1Out");
			assignBase(mStart2Out, "Start2Out");
			assignBase(mStop2Out, "Stop2Out");
		}

		void shutdown() override
		{
		}

	private:
		wraps::BaseGraphConnection* mStart1In{nullptr};
		wraps::BaseGraphConnection* mStart2In{nullptr};
		wraps::BaseGraphConnection* mStart1Out{nullptr};
		wraps::BaseGraphConnection* mStop1Out{nullptr};
		wraps::BaseGraphConnection* mStart2Out{nullptr};
		wraps::BaseGraphConnection* mStop2Out{nullptr};
	};

} // namespace demo

#endif // GRAPH_NODE_GROUP2_CONTROLLER_H_

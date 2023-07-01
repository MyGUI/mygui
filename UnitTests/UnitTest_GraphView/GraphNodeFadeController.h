/*!
	@file
	@author		Albert Semenov
	@date		08/2009
*/
#ifndef GRAPH_NODE_FADE_CONTROLLER_H_
#define GRAPH_NODE_FADE_CONTROLLER_H_

#include <MyGUI.h>
#include "BaseAnimationNode.h"

namespace demo
{

	class GraphNodeFadeController : public BaseAnimationNode
	{
	public:
		GraphNodeFadeController(std::string_view _name) :
			BaseAnimationNode("GraphNodeFadeController.layout", "FadeController", _name)
		{
		}

	private:
		void initialise() override
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());

			assignBase(mStartIn, "StartIn");
			assignBase(mStopIn, "StopIn");
			assignBase(mStartOut, "StartOut");
			assignBase(mStopOut, "StopOut");
			assignBase(mWeightOut, "WeightOut");
		}

		void shutdown() override
		{
		}

	private:
		wraps::BaseGraphConnection* mStartIn{nullptr};
		wraps::BaseGraphConnection* mStopIn{nullptr};
		wraps::BaseGraphConnection* mStartOut{nullptr};
		wraps::BaseGraphConnection* mStopOut{nullptr};
		wraps::BaseGraphConnection* mWeightOut{nullptr};
	};

} // namespace demo

#endif // GRAPH_NODE_FADE_CONTROLLER_H_

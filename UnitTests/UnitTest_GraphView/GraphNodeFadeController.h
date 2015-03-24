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

	class GraphNodeFadeController :
		public BaseAnimationNode
	{
	public:
		GraphNodeFadeController(const std::string& _name) :
			BaseAnimationNode("GraphNodeFadeController.layout", "FadeController", _name),
			mStartIn(nullptr),
			mStopIn(nullptr),
			mStartOut(nullptr),
			mStopOut(nullptr),
			mWeightOut(nullptr)
		{
		}

	private:
		virtual void initialise()
		{
			mMainWidget->castType<MyGUI::Window>()->setCaption(getName());

			assignBase(mStartIn, "StartIn");
			assignBase(mStopIn, "StopIn");
			assignBase(mStartOut, "StartOut");
			assignBase(mStopOut, "StopOut");
			assignBase(mWeightOut, "WeightOut");
		}

		virtual void shutdown()
		{
		}

	private:
		wraps::BaseGraphConnection* mStartIn;
		wraps::BaseGraphConnection* mStopIn;
		wraps::BaseGraphConnection* mStartOut;
		wraps::BaseGraphConnection* mStopOut;
		wraps::BaseGraphConnection* mWeightOut;
	};

} // namespace demo

#endif // GRAPH_NODE_FADE_CONTROLLER_H_

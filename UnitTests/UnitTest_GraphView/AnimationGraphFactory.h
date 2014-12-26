#ifndef ANIMATION_GRAPH_FACTORY_H_
#define ANIMATION_GRAPH_FACTORY_H_

#include "AnimationGraph.h"
#include "AnimationNodeFactory.h"

namespace animation
{

	class AnimationGraphFactory
	{
	public:
		AnimationGraphFactory();

		AnimationGraph* createGraph(const std::string& _filename);

	private:
		AnimationNodeFactory mNodeFactory;
	};

} // namespace animation

#endif // ANIMATION_GRAPH_FACTORY_H_

#ifndef __ANIMATION_GRAPH_FACTORY_H__
#define __ANIMATION_GRAPH_FACTORY_H__

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

#endif // __ANIMATION_GRAPH_FACTORY_H__

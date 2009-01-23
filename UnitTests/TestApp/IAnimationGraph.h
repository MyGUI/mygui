/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __I_ANIMATION_GRAPH_H__
#define __I_ANIMATION_GRAPH_H__

#include <MyGUI.h>

namespace anim
{

	// передается время старта события
	typedef MyGUI::delegates::CMultiDelegate1<float> DelegateLinkEvent;
	// передается время и вес или позиция от 0 до 1 и 
	typedef MyGUI::delegates::CMultiDelegate2<float, float> DelegateLinkValue;

	class IAnimationGraph
	{
	public:
		virtual ~IAnimationGraph() { }
		virtual float getAnimationLength(const std::string& _name) = 0;
	};

} // namespace anim

#endif // __I_ANIMATION_GRAPH_H__

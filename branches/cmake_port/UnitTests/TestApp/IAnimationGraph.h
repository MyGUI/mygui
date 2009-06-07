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

	// ���������� ����� ������ �������
	typedef MyGUI::delegates::CMultiDelegate1<float> DelegateLinkEvent;
	// ���������� ����� � ��� ��� ������� �� 0 �� 1 � 
	typedef MyGUI::delegates::CMultiDelegate2<float, float> DelegateLinkValue;

	class IAnimationGraph
	{
	public:
		virtual ~IAnimationGraph() { }
		virtual float getAnimationLength(const std::string& _name) = 0;
	};

} // namespace anim

#endif // __I_ANIMATION_GRAPH_H__

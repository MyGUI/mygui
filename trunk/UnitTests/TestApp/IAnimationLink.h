/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __I_ANIMATION_LINK_H__
#define __I_ANIMATION_LINK_H__

#include <MyGUI.h>

namespace anim
{

	// передается время старта события
	typedef MyGUI::delegates::CMultiDelegate1<float> DelegateLinkEvent;
	// передается время и вес или позиция от 0 до 1 и 
	typedef MyGUI::delegates::CMultiDelegate2<float, float> DelegateLinkValue;

	class IAnimationLink
	{
	public:
		virtual ~IAnimationLink() { }
		virtual const std::string& getName() = 0;
		virtual DelegateLinkEvent * getLinkEvent(const std::string& _name) = 0;
		virtual DelegateLinkValue * getLinkValue(const std::string& _name) = 0;
		virtual void setLinkEvent(DelegateLinkEvent * _delegate, const std::string& _name) = 0;
		virtual void setLinkValue(DelegateLinkValue * _delegate, const std::string& _name) = 0;
	};

	typedef std::vector<anim::IAnimationLink*> VectorLink;

} // namespace anim

#endif // __I_ANIMATION_LINK_H__

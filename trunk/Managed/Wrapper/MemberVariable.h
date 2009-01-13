/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_VARIABLE_H__
#define __MEMBER_VARIABLE_H__

#include "Member.h"

namespace wrapper
{

	class MemberVariable : public Member
	{
	public:
		MemberVariable(MyGUI::xml::ElementPtr _element) : Member(_element)
		{
		}
	};

} // namespace wrapper

#endif // __MEMBER_VARIABLE_H__

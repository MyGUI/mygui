#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "Prerequest.h"

namespace widget
{
	// ����� ����������� ������ ��������� =)

	#define INSTANCE(T) \
	static T & getInstance() \
	{ \
		static T instance; \
		return instance; \
	}

}

#endif
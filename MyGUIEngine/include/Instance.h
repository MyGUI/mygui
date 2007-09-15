#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "Prerequest.h"

namespace widget
{
	// супер облегченный шаблон синглетон =)

	#define INSTANCE(T) \
	static T & getInstance() \
	{ \
		static T instance; \
		return instance; \
	}

}

#endif
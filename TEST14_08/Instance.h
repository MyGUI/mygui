#pragma once

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
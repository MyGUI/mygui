#pragma once

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
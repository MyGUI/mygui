/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

// �������������� �������������
#define MMYGUI_INITIALISE

// ������������ ��������� ������� ����
//#define MMYGUI_USING_EXTERNAL_TYPE

#ifdef MMYGUI_USING_EXTERNAL_TYPE
// ��������� ������� �����
#    define MMYGUI_EXTERNAL_NAMESPACE
#else
// ��������� ������� �����
#    define MMYGUI_EXTERNAL_NAMESPACE
#endif

//------------------------------------------------------------------------------//
#define MMYGUI_CHECK_NATIVE(ptr) \
	if (ptr == nullptr) throw gcnew System::NullReferenceException();

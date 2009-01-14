/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Compound.h"
#include "Member.h"

namespace wrapper
{
	namespace utility
	{

		//--------------------------------------------------------------------------------------//
		// ���������� ������ ��� � �����, ���� ��� ��������� � ����������
		//--------------------------------------------------------------------------------------//
		std::string getTypeNamespace(const std::string& _type, Compound * _root, const std::string& _namespace);

		//--------------------------------------------------------------------------------------//
		// ���������� ������ ��� � �����, ������� ��������� �� ��������� ��� � ��������� � ����������
		//--------------------------------------------------------------------------------------//
		std::string getFullDefinition(const std::string& _type, Compound * _root, const std::string& _namespace);

		//--------------------------------------------------------------------------------------//
		// �������, ������� ��� �� ���� ������ � ������ ����
		//--------------------------------------------------------------------------------------//
		Member* getByRef(const std::string& _compound, const std::string& _member);

		//--------------------------------------------------------------------------------------//
		// ���������� ������ ��� � �����, ������� ��������� �� ���������
		//--------------------------------------------------------------------------------------//
		std::string getTypedef(const std::string& _type, Compound * _root);

		//--------------------------------------------------------------------------------------//
		// ���������� ���������� �� ���� � �� �����
		//--------------------------------------------------------------------------------------//
		Compound* getCompound(const std::string& _kind, const std::string& _name, Compound * _root);


		inline bool first(const std::string& _original, const std::string& _value)
		{
			return _original.find(_value) == 0;
		}

	} // namespace utility
} // namespace wrapper

#endif // __UTILITY_H__

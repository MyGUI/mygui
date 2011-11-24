/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "StringUtility.h"
#include "Compound.h"
#include "Member.h"

namespace wrapper
{

	std::string replaceTags(const std::string& _data);

	void addTag(const std::string& _tag, const std::string& _data);
	void clearTags();

	void initialise();
	void shutdown();

	//--------------------------------------------------------------------------------------//
	// ���������� ������ ��� � �����, ���� ��� ��������� � ����������
	//--------------------------------------------------------------------------------------//
	std::string getTypeNamespace(const std::string& _type, Compound* _root, const std::string& _namespace);

	//--------------------------------------------------------------------------------------//
	// ���������� ������ ��� � �����, ������� ��������� �� ��������� ��� � ��������� � ����������
	//--------------------------------------------------------------------------------------//
	std::string getFullDefinition(const std::string& _type, Compound* _root, const std::string& _namespace);

	//--------------------------------------------------------------------------------------//
	// �������, ������� ��� �� ���� ������ � ������ ����
	//--------------------------------------------------------------------------------------//
	Member* getByRef(const std::string& _compound, const std::string& _member);

	//--------------------------------------------------------------------------------------//
	// ���������� ������ ��� � �����, ������� ��������� �� ���������
	//--------------------------------------------------------------------------------------//
	std::string getTypedef(const std::string& _type, Compound* _root);

	//--------------------------------------------------------------------------------------//
	// ���������� ���������� �� ���� � �� �����
	//--------------------------------------------------------------------------------------//
	Compound* getCompound(const std::string& _kind, const std::string& _name, Compound* _root);


	//--------------------------------------------------------------------------------------//
	// ���������� �� ������ � ������������ ������
	//--------------------------------------------------------------------------------------//
	inline bool first(const std::string& _original, const std::string& _value)
	{
		return _original.find(_value) == 0;
	}

	//--------------------------------------------------------------------------------------//
	// ��������� �� ������ ����������
	//--------------------------------------------------------------------------------------//
	std::vector<std::string> split_params(const std::string& _name);


	//--------------------------------------------------------------------------------------//
	// ��� �������� ���������� �����
	//--------------------------------------------------------------------------------------//
	class TypeInfo
	{
	public:
		TypeInfo(const std::string& _type)
		{
			token_type = _type;

			std::string::size_type pos = token_type.find("const");
			if (pos == 0)
			{
				token_const = "const";
				token_type.erase(pos, token_const.size());
			}

			pos = token_type.find("*");
			if (pos != std::string::npos)
			{
				token_star = "*";
				token_type.erase(pos, token_star.size());
			}

			pos = token_type.find("&");
			if (pos != std::string::npos)
			{
				token_amp = "&";
				token_type.erase(pos, token_amp.size());
			}

			utility::trim(token_type);
		}

		std::string toString() const
		{
			std::string result;
			if (!token_const.empty())
				result = token_const + " ";

			result += token_type;

			if (!token_star.empty())
				result += " " + token_star;

			if (!token_amp.empty())
				result += " " + token_amp;

			return result;
		}
		const std::string& getType() const
		{
			return token_type;
		}
		void setOnlyType(const std::string& _type)
		{
			token_type = _type;
		}

	private:
		std::string token_const;
		std::string token_type;
		std::string token_amp;
		std::string token_star;
	};

} // namespace wrapper

#endif // __UTILITY_H__

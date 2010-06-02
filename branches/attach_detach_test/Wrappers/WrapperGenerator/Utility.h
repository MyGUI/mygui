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
	// возвращает полное им€ у типов, если они наход€тс€ в неймспейсе
	//--------------------------------------------------------------------------------------//
	std::string getTypeNamespace(const std::string& _type, Compound * _root, const std::string& _namespace);

	//--------------------------------------------------------------------------------------//
	// возвращает полное им€ у типов, которые пр€чутьс€ за тайпдифом или в наход€тс€ в неймспейсе
	//--------------------------------------------------------------------------------------//
	std::string getFullDefinition(const std::string& _type, Compound * _root, const std::string& _namespace);

	//--------------------------------------------------------------------------------------//
	// фабрика, создает тип по айди класса и своему айди
	//--------------------------------------------------------------------------------------//
	Member* getByRef(const std::string& _compound, const std::string& _member);

	//--------------------------------------------------------------------------------------//
	// возвращает полное им€ у типов, которые пр€чутьс€ за тайпдифом
	//--------------------------------------------------------------------------------------//
	std::string getTypedef(const std::string& _type, Compound * _root);

	//--------------------------------------------------------------------------------------//
	// возвращает информацию по типу и по имени
	//--------------------------------------------------------------------------------------//
	Compound* getCompound(const std::string& _kind, const std::string& _name, Compound * _root);


	//--------------------------------------------------------------------------------------//
	// начинаетс€ ли строка с определенной строки
	//--------------------------------------------------------------------------------------//
	inline bool first(const std::string& _original, const std::string& _value)
	{
		return _original.find(_value) == 0;
	}

	//--------------------------------------------------------------------------------------//
	// раздел€ет на список параметров
	//--------------------------------------------------------------------------------------//
	std::vector<std::string> split_params(const std::string& _name);


	//--------------------------------------------------------------------------------------//
	// дл€ удобного управлени€ типом
	//--------------------------------------------------------------------------------------//
	class TypeInfo
	{
	public:
		TypeInfo(const std::string& _type)
		{
			std::vector<std::string> tokens = utility::split(_type);
			if (tokens.empty()) return;

			if (tokens.front() == "const") {
				token_const = tokens.front();
				tokens.erase(tokens.begin());
			}
			if (tokens.empty()) return;

			if (tokens.back() == "&") {
				token_amp = tokens.back();
				tokens.erase(tokens.begin() + tokens.size() - 1);
			}
			if (tokens.empty()) return;

			if (tokens.back() == "*") {
				token_amp = tokens.back();
				tokens.erase(tokens.begin() + tokens.size() - 1);
			}
			if (tokens.empty()) return;

			token_type = tokens.front();
		}

		std::string toString() { return token_const + " " + token_type + " " + token_amp; }
		const std::string& getType() const { return token_type; }
		void setOnlyType(const std::string& _type) { token_type = _type; }

	private:
		std::string token_const;
		std::string token_type;
		std::string token_amp;
	};

} // namespace wrapper

#endif // __UTILITY_H__

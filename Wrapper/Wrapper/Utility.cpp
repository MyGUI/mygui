/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#if WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "Utility.h"
#include "Compound.h"
#include "Member.h"
#include "MemberFunction.h"
#include "MemberVariable.h"
#include <map>
#include <set>

namespace wrapper
{

	typedef std::map<std::string, std::string> MapString;
	MapString* mUserMapLanguage;

	void initialise()
	{
		mUserMapLanguage = new MapString();
	}

	void shutdown()
	{
		delete mUserMapLanguage;
		mUserMapLanguage = 0;
	}

	std::string replaceTags(const std::string& _data)
	{
		assert(mUserMapLanguage);

		// вот хз, что быстрее, итераторы или математика указателей,
		// дл€ непон€тно какого размера одного символа UTF8
		std::string line(_data);

		std::string::iterator end = line.end();
		for (std::string::iterator iter=line.begin(); iter!=end; )
		{
			if (*iter == '#')
			{
				++iter;
				if (iter == end)
				{
					return line;
				}
				else
				{
					if (*iter != '{')
					{
						++iter;
						continue;
					}
					std::string::iterator iter2 = iter;
					++iter2;
					while (true)
					{
						if (iter2 == end) 
						{
							return line;
						}
						if (*iter2 == '}')
						{
							size_t start = iter - line.begin();
							size_t len = (iter2 - line.begin()) - start - 1;
							const std::string& tag = line.substr(start + 1, len);

							MapString::iterator replace = mUserMapLanguage->find(tag);
							replace != mUserMapLanguage->end();

							iter = line.erase(iter - size_t(1), iter2 + size_t(1));
							size_t pos = iter - line.begin();
							if (replace != mUserMapLanguage->end())
							{
								line.insert(pos, replace->second);
								iter = line.begin() + pos + replace->second.length();
							}
							end = line.end();
							if (iter == end) 
							{
								return line;
							}
							break;
						}
						++iter2;
					};
				}
			}
			else
			{
				++iter;
			}
		}

		return line;
	}

	void clearTags()
	{
		assert(mUserMapLanguage);
		mUserMapLanguage->clear();
	}

	void addTag(const std::string& _tag, const std::string& _data)
	{
		assert(mUserMapLanguage);
		(*mUserMapLanguage)[_tag] = _data;
	}

	//--------------------------------------------------------------------------------------//
	// фабрика, создает тип по айди класса и своему айди
	//--------------------------------------------------------------------------------------//
	Member* getByRef(const std::string& _compound, const std::string& _member)
	{
		xml::ElementPtr element = 0;

		xml::Document doc;
		const std::string filename = "doxygen/xml/" + _compound + ".xml";

		if ( !doc.open(filename) )
		{
			std::cout << doc.getLastError() << std::endl;
			return 0;
		}

		xml::ElementEnumerator compound = doc.getRoot()->getElementEnumerator();
		while (compound.next("compounddef"))
		{
			if (compound->findAttribute("id") != _compound) continue;

			xml::ElementEnumerator sectiondef = compound->getElementEnumerator();
			while (sectiondef.next("sectiondef"))
			{

				xml::ElementEnumerator memberdef = sectiondef->getElementEnumerator();
				while (memberdef.next("memberdef"))
				{
					if (memberdef->findAttribute("id") != _member) continue;
					element = memberdef.current();
					break;
				}
			}
		}

		if (element == 0) {
			std::cout << "'" << _member << "' not found in '" << _compound << "'" << std::endl;
			return 0;
		}

		// а вот тут типа фабрика
		std::string kind = element->findAttribute("kind");
		if (kind == "function") return new MemberFunction(element);
		else if (kind == "variable") return new MemberVariable(element);
		return new Member(element);
	}

	std::string correctPlatformType(const std::string& _namespace, const std::string& _type)
	{
		std::string std_token = (_type.size() > 5) ? _type.substr(0, 5) : "";

		if (std_token == "std::" ||
			_type == "int" ||
			_type == "unsigned int" ||
			_type == "short" ||
			_type == "unsigned short" ||
			_type == "char" ||
			_type == "unsigned char" ||
			_type == "size_t" ||
			_type == "float"
			) return _type;
		return _namespace + "::" + _type;
	}

	//--------------------------------------------------------------------------------------//
	// возвращает полное им€ у типов, которые пр€чутьс€ за тайпдифом
	//--------------------------------------------------------------------------------------//
	std::string getTypedef(const std::string& _type, Compound * _root)
	{
		TypeInfo type(_type);

		Compound::Enumerator enumerator = _root->getEnumerator();
		while (enumerator.next())
		{
			if (enumerator->getType() != "compound" ||
				enumerator->getKind() != "namespace") continue;

			Compound::Enumerator enumerator2 = enumerator->getEnumerator();
			while (enumerator2.next())
			{
				if (enumerator2->getKind() != "typedef" ||
					enumerator2->getName() != type.getType()) continue;

				Member * member = getByRef(enumerator->getId(), enumerator2->getId());
				type.setOnlyType(correctPlatformType(enumerator->getName(), member->getType()));
				delete member;
				member = 0;

				return type.toString();
			}

		}
		return _type;
	}

	//--------------------------------------------------------------------------------------//
	// возвращает полное им€ у типов, если они наход€тс€ в неймспейсе
	//--------------------------------------------------------------------------------------//
	std::string getTypeNamespace(const std::string& _type, Compound * _root, const std::string& _namespace)
	{
		TypeInfo type(_type);
		type.setOnlyType(_namespace + "::" + type.getType());

		Compound::Enumerator enumerator = _root->getEnumerator();
		while (enumerator.next())
		{
			if (enumerator->getType() != "compound" ||
				enumerator->getName() != type.getType()) continue;
			return type.toString();
		}

		return _type;
	}

	//--------------------------------------------------------------------------------------//
	// возвращает полное им€ у типов, которые пр€чутьс€ за тайпдифом или в наход€тс€ в неймспейсе
	//--------------------------------------------------------------------------------------//
	std::string getFullDefinition(const std::string& _type, Compound * _root, const std::string& _namespace)
	{
		std::string type = getTypedef(_type, _root);
		if (type == _type)
		{
			type = getTypeNamespace(type, _root, _namespace);
		}

		// обрабатываем вложенные тайпдифы
		size_t start = type.find_first_of("<");
		size_t end = type.find_last_of(">");
		if (start != std::string::npos && end != std::string::npos && start < end)
		{
			std::string inner_type = type.substr(start + 1, end - start - 1);
			std::vector<std::string> inner_types = utility::split(inner_type, ",");
			size_t count = inner_types.size();
			if (count != 0) {
				for (size_t index=0; index<count; ++index)
				{
					utility::trim(inner_types[index]);
					inner_types[index] = " " + getFullDefinition(inner_types[index], _root, _namespace) + " ";
				}

				if (count == 1)
				{
					inner_type = inner_types[0];
				}
				else
				{
					inner_type.clear();
					for (size_t index=0; index<count-1; ++index)
					{
						inner_type += inner_types[index] + " , ";
					}
					inner_type += inner_types[count-1];
				}

				type.erase(start + 1, end - start - 1);
				type.insert(type.begin() + start + 1, inner_type.begin(), inner_type.end());

			}
		}

		/*if (type == "Message::ViewInfo")
		{
			int test = 0;
		}*/

		return type;
	}

	//--------------------------------------------------------------------------------------//
	// возвращает информацию по типу и по имени
	//--------------------------------------------------------------------------------------//
	Compound* getCompound(const std::string& _kind, const std::string& _name, Compound * _root)
	{
		Compound::Enumerator enumerator = _root->getEnumerator();
		while (enumerator.next())
		{
			if (enumerator->getType() != "compound" ||
				enumerator->getKind() != _kind ||
				enumerator->getName() != _name) continue;

			return enumerator.current();
		}
		return 0;
	}

	std::vector<std::string> split_params(const std::string& _name)
	{
		std::vector<std::string> vec;
		size_t cov = 0;
		size_t start = 0;

		for (size_t index=0; index<_name.size(); ++index)
		{
			if (_name[index] == '<')
			{
				cov++;
			}
			else if (_name[index] == '>')
			{
				if (cov == 0)
				{
					vec.clear();
					break;
				}
				cov--;
			}
			else if (_name[index] == ',')
			{
				if (cov == 0)
				{
					vec.push_back(_name.substr(start, index - start - 1));
					utility::trim(vec.back());
					start = index + 1;
				}
			}
		}

		if (cov == 0)
		{
			vec.push_back(_name.substr(start));
			utility::trim(vec.back());
		}
		else
		{
			vec.clear();
		}

		return vec;
	}

} // namespace wrapper

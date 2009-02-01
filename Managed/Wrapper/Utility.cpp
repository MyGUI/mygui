/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "Utility.h"
#include "Compound.h"
#include "Member.h"
#include "MemberFunction.h"
#include "MemberVariable.h"

namespace wrapper
{
	namespace utility
	{

		void fixXmlFile(const std::string& _filename)
		{
			static std::set<std::string> cache;
			if (cache.find(_filename) != cache.end()) return;
			cache.insert(_filename);

			std::fstream file;
			file.open(_filename.c_str(), std::ios::in | std::ios::out | std::ios::binary);
			if (!file.is_open()) return;

			int pos = 0;
			while ( ! file.eof() )
			{
				file.seekp(pos);

				char data;
				file.read(&data, 1);

				if (data == '&')
				{
					file.seekp(pos + 1);
					file.read(&data, 1);
					if (data != 'a' && data != 'l' && data != 'g' && data != 'q')
					{
						file.seekp(pos);
						file.write(" ", 1);
					}
				}

				pos++;
			}

			file.close();
		}

		//--------------------------------------------------------------------------------------//
		// фабрика, создает тип по айди класса и своему айди
		//--------------------------------------------------------------------------------------//
		Member* getByRef(const std::string& _compound, const std::string& _member)
		{
			MyGUI::xml::ElementPtr element = nullptr;

			MyGUI::xml::Document doc;
			const std::string filename = "doxygen/xml/" + _compound + ".xml";

			fixXmlFile(filename);

			if ( !doc.open(filename) )
			{
				std::cout << doc.getLastError() << std::endl;
				return nullptr;
			}

			MyGUI::xml::ElementEnumerator compound = doc.getRoot()->getElementEnumerator();
			while (compound.next("compounddef"))
			{
				if (compound->findAttribute("id") != _compound) continue;

				MyGUI::xml::ElementEnumerator sectiondef = compound->getElementEnumerator();
				while (sectiondef.next("sectiondef"))
				{

					MyGUI::xml::ElementEnumerator memberdef = sectiondef->getElementEnumerator();
					while (memberdef.next("memberdef"))
					{
						if (memberdef->findAttribute("id") != _member) continue;
						element = memberdef.current();
						break;
					}
				}
			}

			if (element == nullptr) {
				std::cout << "'" << _member << "' not found in '" << _compound << "'" << std::endl;
				return nullptr;
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
				std::vector<std::string> inner_types = MyGUI::utility::split(inner_type, ",");
				size_t count = inner_types.size();
				if (count != 0) {
					for (size_t index=0; index<count; ++index)
					{
						MyGUI::utility::trim(inner_types[index]);
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
			return nullptr;
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
						MyGUI::utility::trim(vec.back());
						start = index + 1;
					}
				}
			}

			if (cov == 0)
			{
				vec.push_back(_name.substr(start));
				MyGUI::utility::trim(vec.back());
			}
			else
			{
				vec.clear();
			}

			return vec;
		}

	} // namespace utility
} // namespace wrapper

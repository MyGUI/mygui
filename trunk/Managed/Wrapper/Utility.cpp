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

		//--------------------------------------------------------------------------------------//
		// фабрика, создает тип по айди класса и своему айди
		//--------------------------------------------------------------------------------------//
		Member* getByRef(const std::string& _compound, const std::string& _member)
		{
			MyGUI::xml::ElementPtr element = nullptr;

			MyGUI::xml::Document doc;
			const std::string filename = "doxygen/xml/" + _compound + ".xml";
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


		//--------------------------------------------------------------------------------------//
		// возвращает полное им€ у типов, которые пр€чутьс€ за тайпдифом
		//--------------------------------------------------------------------------------------//
		std::string getTypedef(const std::string& _type, Compound * _root)
		{
			Compound::Enumerator enumerator = _root->getEnumerator();
			while (enumerator.next())
			{
				if (enumerator->getType() != "compound" ||
					enumerator->getKind() != "namespace") continue;

				Compound::Enumerator enumerator2 = enumerator->getEnumerator();
				while (enumerator2.next())
				{
					if (enumerator2->getKind() != "typedef" ||
						enumerator2->getName() != _type) continue;

					Member * member = getByRef(enumerator->getId(), enumerator2->getId());
					return enumerator->getName() + "::" + member->getType();
				}

			}
			return _type;
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

	} // namespace utility
} // namespace wrapper

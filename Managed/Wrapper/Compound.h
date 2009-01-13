/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __COMPOUND_H__
#define __COMPOUND_H__

#include <MyGUI.h>
#include "Member.h"
#include "MemberVariable.h"
#include "MemberFunction.h"
#include "MemberProperty.h"

namespace wrapper
{

	class Compound
	{
	public:
		typedef std::vector<std::string> VectorString;
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;

		typedef std::vector<Member*> VctorMember;

		Compound(const std::string& _name) : mName(_name)
		{
		}

		void addTagPair(const std::string& _key, const std::string& _value)
		{
			mTags.push_back(PairString(_key, _value));
		}

		void addTemplate(const std::string& _name, const std::string& _out)
		{
			mTemplates.push_back(PairString(_name, _out));
		}

		void initialise()
		{
			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();
			for (VectorPairString::const_iterator item=mTags.begin(); item!=mTags.end(); ++item) {
				manager.addUserTag(item->first, item->second);
			}

			// создаем файлы шаблонов и настраиваем их
			for (VectorPairString::const_iterator item=mTemplates.begin(); item!=mTemplates.end(); ++item) {
				std::ifstream infile;
				infile.open(item->first.c_str());
				if ( ! infile.is_open() ) {
					std::cout << "error open file " << item->first << std::endl;
					continue;
				}

				std::ofstream outfile;
				outfile.open(item->second.c_str());
				if ( ! outfile.is_open() ) {
					std::cout << "error open file " << item->second << std::endl;
					infile.close();
					continue;
				}

				std::string read;
				std::string data;

				while (false == infile.eof()) {
					std::getline(infile, read);
					read = manager.replaceTags(read);
					data += read + "\n";
				}

				outfile << data;

				infile.close();
				outfile.close();
			}

			// ну а теперь парсим методы
			generate();
		}

	private:
		void generate()
		{

			MyGUI::xml::Document doc;
			const std::string filename = "doxygen/xml/index.xml";
			if ( !doc.open(filename) )
			{
				std::cout << doc.getLastError() << std::endl;
				return;
			}

			MyGUI::xml::ElementEnumerator compound = doc.getRoot()->getElementEnumerator();
			while (compound.next("compound"))
			{
				std::string name = getItemName(compound.current());
				if (name != mName) continue;
				mId = compound->findAttribute("refid");

				VctorMember items;
				MyGUI::xml::ElementEnumerator member = compound->getElementEnumerator();
				while (member.next("member"))
				{
					Member * item = createMember(member.current(), mId);
					items.push_back( item );
				}

				// дополнительная обработка
				postProccesing(items);

				// вставляем в шаблон
				for (VctorMember::iterator item=items.begin(); item!=items.end(); ++item) {
					(*item)->insertToTemplates(mTemplates);
				}

				// удаляем теперь все
				for (VctorMember::iterator item=items.begin(); item!=items.end(); ++item) { delete (*item); }
				items.clear();

				break;
			}
		}

		// типа фабрика
		Member* createMember(MyGUI::xml::ElementPtr _element, const std::string& _id)
		{
			std::string kind = _element->findAttribute("kind");
			if (kind == "function") return new MemberFunction(_element, _id);
			else if (kind == "variable") return new MemberVariable(_element, _id);
			return new Member(_element, _id);
		}

		std::string getItemName(MyGUI::xml::ElementPtr _element)
		{
			MyGUI::xml::ElementEnumerator child_item = _element->getElementEnumerator();
			while (child_item.next("name"))
				return child_item->getContent();
			return "";
		}

		bool isSetProperty(Member* _item)
		{
			if (_item->getKind() != "function") return false;
			std::string name = _item->getName();
			if (name.size() > 3 && 
				name.substr(0, 3) == "set" &&
				static_cast<MemberFunction*>(_item)->getParamCount() == 1 &&
				static_cast<MemberFunction*>(_item)->getResultType() == "void")
			{
				return true;
			}
			return false;
		}

		bool isGetProperty(Member* _item)
		{
			if (_item->getKind() != "function") return false;
			std::string name = _item->getName();
			if (name.size() > 3 && 
				(name.substr(0, 3) == "get" || name.substr(0, 2) == "is") &&
				static_cast<MemberFunction*>(_item)->getParamCount() == 0 &&
				static_cast<MemberFunction*>(_item)->getResultType() != "void")
			{
				return true;
			}
			return false;
		}

		bool isPairProperty(Member* _itemSet, Member* _itemGet)
		{
			if ( ! isSetProperty(_itemSet) || ! isGetProperty(_itemGet) ) return false;
			MemberFunction* set = static_cast<MemberFunction*>(_itemSet);
			MemberFunction* get = static_cast<MemberFunction*>(_itemGet);

			std::string name = set->getName().substr(3);
			if (("get" + name) != get->getName() && ("is" + name) != get->getName()) return false;
			if (set->getParamType(0) != get->getResultType()) return false;

			return true;
		}

		void postProccesing(VctorMember& _items)
		{
			// ищем проперти
			for (size_t index=0; index<_items.size(); ++index) {
				if ( ! isSetProperty(_items[index]) ) continue;

				// теперь ищем гетеры
				for (size_t index2=index+1; index2<_items.size(); ++index2) {
					if ( ! isGetProperty(_items[index2]) ) continue;
					if ( ! isPairProperty(_items[index], _items[index2]) ) continue;

					// создаем проперти
					MemberProperty * prop = new MemberProperty(
						static_cast<MemberFunction*>(_items[index]),
						static_cast<MemberFunction*>(_items[index2]));
					_items[index] = prop;
					_items[index2] = _items[_items.size()-1];
					_items.pop_back();
				}

			}
		}

	private:
		std::string mName;
		std::string mId;
		VectorPairString mTemplates;
		VectorPairString mTags;
	};

} // namespace MMyGUI

#endif // __COMPOUND_H__

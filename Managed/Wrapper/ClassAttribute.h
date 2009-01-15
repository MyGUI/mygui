/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __CLASS_ATTRIBUTE_H__
#define __CLASS_ATTRIBUTE_H__

#include <MyGUI.h>

#include "Utility.h"
#include "ITypeHolder.h"

namespace wrapper
{

	class ClassAttribute : public ITypeHolder
	{
	public:
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;
		typedef std::vector<Member*> VectorMember;

	public:
		ClassAttribute(MyGUI::xml::ElementPtr _element)
		{
			mName = _element->findAttribute("name");
			mNamespace = _element->findAttribute("namespace");

			MyGUI::xml::ElementEnumerator child = _element->getElementEnumerator();
			while (child.next())
			{
				if (child->getName() == "Template")
				{
					mTemplates.push_back( PairString(child->findAttribute("name"), child->findAttribute("output")) );
				}
				else if (child->getName() == "ReplaceTag")
				{
					mPairTag.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
				else if (child->getName() == "ReplaceType")
				{
					mPairType.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
				else if (child->getName() == "ReplaceMethod")
				{
					mPairMethods.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
			}
		}

		virtual std::string getTypeDescription(const std::string& _type)
		{
			for (VectorPairString::const_iterator item=mPairType.begin(); item!=mPairType.end(); ++item) {
				if (item->first == _type) return item->second;
			}
			return utility::getFullDefinition(_type, mRoot, mNamespace);
		}

		void wrap(Compound * _root)
		{
			mRoot = _root;
			createTemplates();

			Compound * item = utility::getCompound("class", mName, _root);
			if (item == nullptr)
			{
				std::cout << mName << " not found" << std::endl;
				return;
			}

			// список всех мембероф
			VectorMember items;

			Compound::Enumerator enumerator = item->getEnumerator();
			while (enumerator.next())
			{
				Member* member = utility::getByRef(item->getId(), enumerator->getId());
				if (member == nullptr)
				{
					std::cout << enumerator->getId() << " not found in " << item->getId() << std::endl;
				}
				else
				{
					bool need = true;
					for (VectorPairString::const_iterator item=mPairMethods.begin(); item!=mPairMethods.end(); ++item) {
						if (item->first == member->getName())
						{
							if (item->second == "")
								need = false;
							else
								member->setName(item->second);
							break;
						}
					}

					if (need)
						items.push_back(member);
				}
			}

			// пост обработка
			for (size_t index=0; index<items.size(); ++index)
			{
				for (size_t index2=0; index2<items.size(); ++index2)
				{
					if (index == index2) continue;
					if ( ! items[index]->postProccesing(items[index2]) ) continue;

					// нас обработали, выкидываем второй указатель
					items.erase(items.begin() + index2);
					if (index > index2) index--;
				}
			}

			std::cout << std::endl << "class : " << mName << std::endl << std::endl;

			// вставка айтемов в файл
			for (size_t index=0; index<items.size(); ++index)
			{
				wrapMember(items[index], this);
			}

			std::cout << std::endl << std::endl << std::endl;

		}

	private:
		void createTemplates()
		{
			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();
			for (VectorPairString::const_iterator item=mPairTag.begin(); item!=mPairTag.end(); ++item) {
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
					if ( ! data .empty() ) data += "\n";
					data += read;
				}

				outfile << data;

				infile.close();
				outfile.close();
			}
		}

		void wrapMember(Member* _member, ITypeHolder * _holder)
		{
			for (VectorPairString::iterator item=mTemplates.begin(); item!=mTemplates.end(); ++item) {
				_member->insertToTemplate(item->second, _holder);
			}
		}

	private:
		std::string mName;
		std::string mNamespace;
		VectorPairString mTemplates;
		VectorPairString mPairTag;
		VectorPairString mPairType;
		VectorPairString mPairMethods;
		Compound * mRoot;
	};

} // namespace wrapper

#endif // __CLASS_ATTRIBUTE_H__

/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __CLASS_ATTRIBUTE_H__
#define __CLASS_ATTRIBUTE_H__

#include "Utility.h"
#include "ITypeHolder.h"

namespace wrapper
{

	class ICommonTypeHolder
	{
	public:
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;

		typedef std::vector<size_t> VectorSizeT;
		typedef std::vector<std::string> VectorString;
		struct CustomTypeInfo
		{
			std::string name;
			VectorSizeT params;
			VectorString templates;
		};

		struct TypeInfo
		{
			VectorPairString tags;
			CustomTypeInfo custom;
		};

		typedef std::map<std::string, TypeInfo> MapTypeInfo;

		virtual VectorPairString getTypeInfo(const std::string& _type) = 0;
		virtual std::string getTemplatePrefix(const std::string& _template, const std::string& _rettype, const VectorParam& _params, const std::string& _namespace) = 0;
	};

	class ClassAttribute : public ITypeHolder
	{
	public:
		typedef std::vector<Member*> VectorMember;

		struct Template
		{
			Template(const std::string& _name, const std::string& _output, const std::string& _type) : name(_name), output(_output), type(_type) { }
			std::string name;
			std::string output;
			std::string type;
		};
		typedef std::vector<Template> VectorTemplate;

	public:
		ClassAttribute(xml::ElementPtr _element)
		{
			mType = _element->findAttribute("type");
			mNamespace = _element->findAttribute("namespace");

			xml::ElementEnumerator child = _element->getElementEnumerator();
			while (child.next())
			{
				if (child->getName() == "Template")
				{
					mTemplates.push_back( Template(child->findAttribute("name"), child->findAttribute("output"), child->findAttribute("type")) );
				}
				//FIXME
				else if (child->getName() == "AddTemplate")
				{
					mAddTemplates.push_back( PairString(child->findAttribute("name"), child->findAttribute("output")) );
				}
				else if (child->getName() == "Tag")
				{
					mPairTag.push_back( PairString(child->findAttribute("key"), child->findAttribute("value")) );
				}
				else if (child->getName() == "Member")
				{
					mPairMemberData.push_back( PairString(child->findAttribute("name"), child->findAttribute("template")) );
				}
			}
		}

		virtual std::string getTypeDescription(const std::string& _type)
		{
			return getFullDefinition(_type, mRoot, mNamespace);
		}

		virtual std::string getMemberData(const std::string& _name)
		{
			for (VectorPairString::const_iterator item=mPairMemberData.begin(); item!=mPairMemberData.end(); ++item) {
				if (item->first == _name) return item->second;
			}
			return "";
		}

		virtual VectorPairString getTypeInfo(const std::string& _type)
		{
			return mCommonTypeHolder->getTypeInfo(_type);
		}

		virtual std::string getTemplatePrefix(const std::string& _template, const std::string& _rettype, const VectorParam& _params)
		{
			return mCommonTypeHolder->getTemplatePrefix(_template, _rettype, _params, mNamespace);
		}

		virtual std::string getMemberName(const std::string& _name)
		{
			if ( ! _name.empty())
			{
				char sim = _name[0];
				if (sim >= 0x61 && sim <= 0x7A)
				{
					sim -= 0x20;
					std::string name = _name;
					name[0] = sim;
					return name;
				}
			}
			return _name;
		}

		void wrap(Compound * _root)
		{
			mRoot = _root;
			createTemplates();
			appendToTemplates();

			// список всех мембероф
			VectorMember items;

			// сначала основной класс
			wrapClass(mType, _root, items);
			wrapItems(_root, items);
		}

		void initialise(Compound * _root, ICommonTypeHolder* _typeholder)
		{
			mCommonTypeHolder = _typeholder;
			for (VectorPairString::const_iterator item=mAddTemplates.begin(); item!=mAddTemplates.end(); ++item) {
				std::ofstream onfile;
				onfile.open(item->second.c_str(), std::ios_base::out);
				if ( ! onfile.is_open() ) {
					std::cout << "error open file " << item->first << std::endl;
					continue;
				}
				onfile.close();
			}
		}

	private:

		void wrapItems(Compound * _root, VectorMember& _items)
		{
			std::cout << std::endl;

			// пост обработка
			for (size_t index=0; index<_items.size(); ++index)
			{
				for (size_t index2=0; index2<_items.size(); ++index2)
				{
					if (index == index2) continue;
					if ( ! _items[index]->postProccesing(_items[index2]) ) continue;

					// нас обработали, выкидываем второй указатель
					_items.erase(_items.begin() + index2);
					if (index > index2) index--;
					index2--;
				}
			}

			// вставка айтемов в файл
			for (size_t index=0; index<_items.size(); ++index)
			{
				wrapMember(_items[index]);
			}

			std::cout << std::endl;

		}

		void wrapClass(const std::string& _name, Compound * _root, VectorMember& _items)
		{
			Compound * item = getCompound("class", _name, _root);
			if (item == nullptr)
			{
				std::cout << mType << " not found" << std::endl;
				return;
			}

			Compound::Enumerator enumerator = item->getEnumerator();
			while (enumerator.next())
			{
				Member* member = getByRef(item->getId(), enumerator->getId());
				if (member == nullptr)
				{
					std::cout << enumerator->getId() << " not found in " << item->getId() << std::endl;
				}
				else
				{
					_items.push_back(member);
				}
			}

			std::cout << std::endl << "class : " << _name << std::endl;

		}

		void createTemplates()
		{
			clearTags();
			for (VectorPairString::const_iterator item=mPairTag.begin(); item!=mPairTag.end(); ++item) {
				addTag(item->first, item->second);
			}

			// создаем файлы шаблонов и настраиваем их
			for (VectorTemplate::const_iterator item=mTemplates.begin(); item!=mTemplates.end(); ++item) {
				std::ifstream infile;
				std::string file = "Data/" + item->type + "/" + item->name;
				infile.open(file.c_str());
				if ( ! infile.is_open() ) {
					std::cout << "error open file " << file << std::endl;
					continue;
				}

				std::ofstream outfile;
				outfile.open(item->output.c_str());
				if ( ! outfile.is_open() ) {
					std::cout << "error open file " << item->output << std::endl;
					infile.close();
					continue;
				}

				std::string read;
				std::string data;

				while (false == infile.eof()) {
					std::getline(infile, read);
					read = replaceTags(read);
					if ( ! data .empty() ) data += "\n";
					data += read;
				}

				outfile << data;

				infile.close();
				outfile.close();
			}
		}

		void appendToTemplates()
		{
			clearTags();
			for (VectorPairString::const_iterator item=mPairTag.begin(); item!=mPairTag.end(); ++item) {
				addTag(item->first, item->second);
			}

			// создаем файлы шаблонов и настраиваем их
			for (VectorPairString::const_iterator item=mAddTemplates.begin(); item!=mAddTemplates.end(); ++item) {
				std::ifstream infile;
				infile.open(item->first.c_str());
				if ( ! infile.is_open() ) {
					std::cout << "error open file " << item->first << std::endl;
					continue;
				}

				std::ofstream outfile;
				outfile.open(item->second.c_str(), std::ios_base::app);
				if ( ! outfile.is_open() ) {
					std::cout << "error open file " << item->second << std::endl;
					infile.close();
					continue;
				}

				std::string read;
				std::string data;

				while (false == infile.eof()) {
					std::getline(infile, read);
					read = replaceTags(read);
					if ( ! data .empty() ) data += "\n";
					data += read;
				}

				// утф заголовки
				if (data.size() > 3) {
					if (data[2] < 32) {
						data[0] = ' ';
						data[1] = ' ';
						data[2] = ' ';
					}
				}

				outfile << data;

				infile.close();
				outfile.close();
			}
		}

		void wrapMember(Member* _member)
		{
			for (VectorTemplate::iterator item=mTemplates.begin(); item!=mTemplates.end(); ++item) {
				clearTags();
				for (VectorPairString::const_iterator item2=mPairTag.begin(); item2!=mPairTag.end(); ++item2) {
					addTag(item2->first, item2->second);
				}
				_member->insertToTemplate(item->output, this, item->type);
			}
		}

	private:
		std::string mType;
		std::string mNamespace;
		VectorTemplate mTemplates;
		VectorPairString mAddTemplates;
		VectorPairString mPairTag;
		VectorPairString mPairMemberData;
		Compound * mRoot;
		ICommonTypeHolder* mCommonTypeHolder;
	};

} // namespace wrapper

#endif // __CLASS_ATTRIBUTE_H__

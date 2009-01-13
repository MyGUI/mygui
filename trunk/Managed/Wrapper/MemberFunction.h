/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_FUNCTION_H__
#define __MEMBER_FUNCTION_H__

#include "Member.h"

namespace wrapper
{

	class MemberFunction : public Member
	{
	public:
		struct Param { std::string type, name, def; };
		typedef std::vector<Param> VectorParam;

	public:
		MemberFunction(MyGUI::xml::ElementPtr _element, const std::string& _id) : Member(_element, _id)
		{
			MyGUI::xml::ElementPtr element = nullptr;

			MyGUI::xml::Document doc;
			const std::string filename = "doxygen/xml/" + _id + ".xml";
			if ( !doc.open(filename) )
			{
				std::cout << doc.getLastError() << std::endl;
				return;
			}

			MyGUI::xml::ElementEnumerator compound = doc.getRoot()->getElementEnumerator();
			while (compound.next("compounddef"))
			{
				if (compound->findAttribute("id") != _id) continue;

				MyGUI::xml::ElementEnumerator sectiondef = compound->getElementEnumerator();
				while (sectiondef.next("sectiondef"))
				{
					if (sectiondef->findAttribute("kind") != "public-func") continue;

					MyGUI::xml::ElementEnumerator memberdef = sectiondef->getElementEnumerator();
					while (memberdef.next("memberdef"))
					{
						if (memberdef->findAttribute("id") != mId) continue;
						element = memberdef.current();
						break;
					}
				}
			}

			if (element == nullptr) {
				std::cout << mId << " not found" << std::endl;
				return;
			}

			mProtection = element->findAttribute("prot") != "public";
			mStatic = element->findAttribute("static") != "no";
			mConst = element->findAttribute("const") != "no";
			mExplicit = element->findAttribute("explicit") != "no";
			mInline = element->findAttribute("inline") == "yes";
			mVirtual = element->findAttribute("virt") == "virtual";

			MyGUI::xml::ElementEnumerator info = element->getElementEnumerator();
			while (info.next())
			{
				if (info->getName() == "type")
				{
					mType = info->getContent();
				}
				else if (info->getName() == "param")
				{
					Param pair_param;
					MyGUI::xml::ElementEnumerator param = info->getElementEnumerator();
					while (param.next())
					{
						if (param->getName() == "type")
							pair_param.type = param->getContent();
						else if (param->getName() == "declname")
							pair_param.name = param->getContent();
						else if (param->getName() == "defval")
							pair_param.def = param->getContent();
					}
					mParams.push_back(pair_param);
				}
			}
		}

		size_t getParamCount() { return mParams.size(); }
		const std::string& getResultType() { return mType; }
		const std::string& getParamType(size_t _index) { return mParams.at(_index).type; }

	private:
		virtual void insertToTemplate(const std::string& _template)
		{
			VectorString file_data;

			// сначала все считываем
			std::string filename = _template;
			std::ifstream infile;
			infile.open(filename.c_str());
			if ( ! infile.is_open() ) {
				std::cout << "error open file " << filename << std::endl;
				return;
			}

			std::string read;
			while (false == infile.eof()) {
				std::getline(infile, read);
				file_data.push_back(read);
			}
			infile.close();

			// теперь все записываем
			std::ofstream outfile;
			outfile.open(filename.c_str());
			if ( ! outfile.is_open() ) {
				std::cout << "error open file " << filename << std::endl;
				return;
			}

			for (VectorString::iterator item = file_data.begin(); item!=file_data.end(); ++item) {
				outfile << *item << "\n";
				if (item->find("/*InsertPoint*/") != std::string::npos) {
					insert(outfile);
				}
			}

			outfile.close();

		}

		void insert(std::ofstream& _stream)
		{
			if (mType.empty() || // шаблоны и конструкторы
				mStatic ||
				mProtection ||
				( ! mName.empty() && mName[0] == '_' ) )
			{
				return;
			}

			std::string template_name = MyGUI::utility::toString("Data/Templates/Function", (mType == "void" ? "" : "Return"), mParams.size(), "_template.txt");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();

			manager.addUserTag("FunctionName", mName);
			manager.addUserTag("ValueTypeReturn", mType);


			for (size_t index=0; index<mParams.size(); ++index) {
				manager.addUserTag(MyGUI::utility::toString("ValueType", index + 1), mParams[index].type);
				manager.addUserTag(MyGUI::utility::toString("ValueName", index + 1), mParams[index].name);
			}

			std::string data, read;
			std::ifstream infile;
			infile.open(template_name.c_str());
			if ( ! infile.is_open() ) {
				std::cout << "error open file " << template_name << std::endl;
				return;
			}

			while (false == infile.eof()) {
				std::getline(infile, read);
				data += read + "\n";
			}

			infile.close();

			// утф заголовки
			if (data.size() > 3) {
				if (data[2] < 32) {
					data[0] = ' ';
					data[1] = ' ';
					data[2] = ' ';
				}
			}

			data = manager.replaceTags(data);

			_stream << data;
		}

	private:
		bool mProtection;
		bool mStatic;
		bool mConst;
		bool mExplicit;
		bool mInline;
		bool mVirtual;

		std::string mType;
		VectorParam mParams;
	};

} // namespace wrapper

#endif // __MEMBER_FUNCTION_H__

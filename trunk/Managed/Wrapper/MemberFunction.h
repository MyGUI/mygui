/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_FUNCTION_H__
#define __MEMBER_FUNCTION_H__

#include "Member.h"
#include "Utility.h"

namespace wrapper
{

	class MemberFunction : public Member
	{
	public:
		struct Param { std::string type, name, def; };
		typedef std::vector<Param> VectorParam;
		typedef MyGUI::Enumerator<VectorParam> EnumeratorParam;

	public:
		MemberFunction(MyGUI::xml::ElementPtr _element) :
			Member(_element),
			mGetProperty(nullptr)
		{
			mProtection = _element->findAttribute("prot") != "public";
			mStatic = _element->findAttribute("static") != "no";
			mConst = _element->findAttribute("const") != "no";
			mExplicit = _element->findAttribute("explicit") != "no";
			mInline = _element->findAttribute("inline") == "yes";
			mVirtual = _element->findAttribute("virt") == "virtual";

			MyGUI::xml::ElementEnumerator info = _element->getElementEnumerator();
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

			mIsSetProperty = isSetProperty();
		}

		~MemberFunction()
		{
			delete mGetProperty;
		}

		size_t getParamCount() { return mParams.size(); }
		const std::string& getParamType(size_t _index) { return mParams.at(_index).type; }
		EnumeratorParam getParamEnumerator() { return EnumeratorParam(mParams.begin(), mParams.end()); }

		virtual void insertToTemplate(const std::string& _template, Compound * _root)
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
				if (item->find("//InsertPoint") != std::string::npos) {
					insert(outfile, _root);
				}
			}

			outfile.close();
		}

		bool isEnabledMethod()
		{
			if (mType.empty() || // шаблоны и конструкторы
				mStatic ||
				mProtection ||
				( ! mName.empty() && mName[0] == '_' ) )
			{
				return false;
			}
			return true;
		}

		// обработка других елементов, если вернется true то елемент удаляется
		virtual bool postProccesing(Member* _member)
		{
			if ( ! isEnabledMethod() ) return false;

			if (mIsSetProperty && isGetProperty(_member))
			{
				mGetProperty = static_cast<MemberFunction*>(_member);
				return true;
			}

			return false;
		}

	private:

		bool isSetProperty()
		{
			if (mName.size() > 3 && 
				mName.substr(0, 3) == "set" &&
				mParams.size() == 1 &&
				mType == "void")
			{
				return true;
			}
			return false;
		}

		bool isGetProperty(Member* _item)
		{
			if ( ! mIsSetProperty ) return false;
			if (_item->getKind() != "function") return false;

			MemberFunction* get = static_cast<MemberFunction*>(_item);

			std::string name = _item->getName();
			if (name.size() > 3 && 
				(name.substr(0, 3) == "get" || name.substr(0, 2) == "is") &&
				get->getParamCount() == 0 &&
				get->getType() != "void")
			{

				// соответсвие нам
				std::string name = mName.substr(3);
				if (("get" + name) != get->getName() && ("is" + name) != get->getName()) return false;
				if (mParams.at(0).type != get->getType()) return false;

				return true;
			}
			return false;
		}

		void insert(std::ofstream& _stream, Compound * _root)
		{
			if ( ! isEnabledMethod() ) return;

			if (mGetProperty != nullptr) insertProperty(_stream, _root);
			else insertMethod(_stream, _root);
		}

		void insertMethod(std::ofstream& _stream, Compound * _root)
		{
			std::string template_name = MyGUI::utility::toString("Data/Templates/Function", (mType == "void" ? "" : "Return"), mParams.size(), "_template.txt");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();

			manager.addUserTag("FunctionName", mName);
			manager.addUserTag("ValueTypeReturn", utility::getTypedef(mType, _root));


			for (size_t index=0; index<mParams.size(); ++index) {
				manager.addUserTag(MyGUI::utility::toString("ValueType", index + 1), utility::getTypedef(mParams[index].type, _root));
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

			std::cout << "function  : " << getName() << std::endl;
		}

		void insertProperty(std::ofstream& _stream, Compound * _root)
		{
			std::string template_name = MyGUI::utility::toString("Data/Templates/Property", (mGetProperty->getName().at(0) == 'i' ? "Is" : "Get"), "_template.txt");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();

			std::string property_name = mName.substr(3);

			manager.addUserTag("PropertyName", property_name);
			manager.addUserTag("PropertyType", utility::getTypedef(mParams.at(0).type, _root));

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

			std::cout << "property  : " << property_name << std::endl;
		}

	private:
		bool mProtection;
		bool mStatic;
		bool mConst;
		bool mExplicit;
		bool mInline;
		bool mVirtual;
		bool mIsSetProperty;

		MemberFunction* mGetProperty;

		VectorParam mParams;
	};


	/*class MemberFunction3 : public Member2
	{
	public:
		struct Param { std::string type, name, def; };
		typedef std::vector<Param> VectorParam;

	public:
		MemberFunction3(MyGUI::xml::ElementPtr _element, const std::string& _id) : Member2(_element, _id)
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
				if (item->find("/*InsertPoint* /") != std::string::npos) {
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
	};*/

	/*class MemberProperty : public Member2
	{
	public:
		MemberProperty() :
			Member2(),
			mFuncGet(nullptr),
			mFuncSet(nullptr)
		{
		}

		MemberProperty(MemberFunction3 * _func_set, MemberFunction3 * _func_get) :
			Member2(),
			mFuncGet(_func_get),
			mFuncSet(_func_set)
		{
			mProperyName = mFuncSet->getName().substr(3);
			mProperyType = mFuncGet->getResultType();
			mTokenIs = _func_get->getName()[0] == 'i';
		}

		~MemberProperty()
		{
			delete mFuncSet;
			delete mFuncGet;
		}

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
				if (item->find("/*InsertPoint* /") != std::string::npos) {
					insert(outfile);
				}
			}

			outfile.close();
		}

		void insert(std::ofstream& _stream)
		{
			std::string template_name = MyGUI::utility::toString("Data/Templates/Property", (mTokenIs ? "Is" : "Get"), "_template.txt");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();

			manager.addUserTag("PropertyName", mProperyName);
			manager.addUserTag("PropertyType", mProperyType);

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
		std::string mProperyName;
		std::string mProperyType;
		bool mTokenIs;

		MemberFunction3* mFuncGet;
		MemberFunction3* mFuncSet;
	};*/

} // namespace wrapper

#endif // __MEMBER_FUNCTION_H__

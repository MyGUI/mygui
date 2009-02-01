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
			mGetProperty(nullptr),
			mTemplate(false),
			mDeprecated(false)
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
				else if (info->getName() == "templateparamlist")
				{
					mTemplate = true;
					Param pair_param;
					MyGUI::xml::ElementEnumerator template_info = info->getElementEnumerator();
					while (template_info.next("param"))
					{
						MyGUI::xml::ElementEnumerator param = template_info->getElementEnumerator();
						while (param.next())
						{
							if (param->getName() == "type")
								pair_param.type = param->getContent();
							else if (param->getName() == "declname")
								pair_param.name = param->getContent();
							//else if (param->getName() == "defval")
								//pair_param.def = param->getContent();
						}
						mTemplateParams.push_back(pair_param);
					}
				}
				else if (info->getName() == "detaileddescription")
				{
					MyGUI::xml::ElementEnumerator para = info->getElementEnumerator();
					while (para.next("para"))
					{
						MyGUI::xml::ElementEnumerator xrefsect = para->getElementEnumerator();
						while (xrefsect.next("xrefsect"))
						{
							MyGUI::xml::ElementEnumerator xreftitle = xrefsect->getElementEnumerator();
							while (xreftitle.next("xreftitle"))
							{
								if (xreftitle->getContent() == "Deprecated")
								{
									mDeprecated = true;
									break;
								}
							}
						}
					}
				}
			}

			if (mName == "setSize" || mName == "getSize")
			{
				int test = 0;
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
		MemberFunction* getGetFunction() { return mGetProperty; }

		virtual void insertToTemplate(const std::string& _template, ITypeHolder * _holder)
		{
			if ( ! isNeedInsert() ) return;

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
				if (item != file_data.begin()) outfile << "\n";
				outfile << *item;

				if (item->find("//InsertPoint") != std::string::npos) {
					outfile << std::endl << std::endl;
					insert(outfile, _holder);
				}
			}

			outfile.close();
		}

		// обработка других елементов, если вернется true то елемент удаляется
		virtual bool postProccesing(Member* _member)
		{

			if (mName == "setSize" && _member->getName() == "getSize")
			{
				int test = 0;
			}

			// дубликат
			if (mName == _member->getName() &&
				mType == _member->getType() &&
				mKind == _member->getKind()
				)
			{
				MemberFunction* member = static_cast<MemberFunction*>(_member);
				if (member->getParamCount() == mParams.size())
				{
					bool eqviv = true;
					for (size_t index=0; index<mParams.size(); ++index)
					{
						if (mParams[index].type != member->getParamType(index))
						{
							eqviv = false;
							break;
						}
					}
					if (eqviv)
					{
						//MemberFunction* get = member->getGetFunction();
						//if (get) mGetProperty = get;
						return true;
					}
				}
			}

			// мы себе уже нашли пропертю
			if (mGetProperty == nullptr && mIsSetProperty && isGetProperty(_member))
			{
				mGetProperty = static_cast<MemberFunction*>(_member);
				return true;
			}

			//if ( ! isNeedInsert() ) return false;

			return false;
		}

	private:

		bool isNeedInsert()
		{
			if (
				mType.empty() || // конструкторы
				mStatic ||
				mProtection ||
				mDeprecated || // устаревшая функция
				mTemplate || // шаблоны
				( ! mName.empty() && mName[0] == '_' )  || // для внутреннего использования
				( ! mName.empty() && mName[0] == '~' ) // деструкторы
				)
			{
				return false;
			}
			return true;
		}

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

		bool compireParam(const std::string& _type1, const std::string& _type2)
		{
			utility::TypeInfo type1(_type1);
			utility::TypeInfo type2(_type2);
			return type1.getType() == type2.getType();
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
				if ( ! compireParam(mParams.at(0).type, get->getType()) ) return false;

				return true;
			}
			return false;
		}

		void insert(std::ofstream& _stream, ITypeHolder * _holder)
		{
			if (mGetProperty != nullptr)
			{
				insertProperty(_stream, _holder);
			}
			else if (_holder->getMemberData(mName) == "GetPropertyOnly")
			{
				insertGetProperty(_stream, _holder);
			}
			else
			{
				// метод со всеми параметрами
				size_t count  = mParams.size();
				insertMethod(_stream, _holder, count);

				// пробуем сгенерить методы без дефолтных параметров
				while (count > 0)
				{
					--count;
					if (mParams[count].def.empty()) break;
					_stream << std::endl;
					insertMethod(_stream, _holder, count);
				}
			}
		}

		void insertMethod(std::ofstream& _stream, ITypeHolder * _holder, size_t _count)
		{
			std::string template_name = MyGUI::utility::toString("Data/Templates/Function", (mType == "void" ? "" : "Return"), _count, "_template.h");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();

			std::string type = _holder->getTypeDescription(mType);
			if (type.empty()) return;
			std::string member_name = _holder->getMemberName(mName);
			if (member_name.empty()) return;
			manager.addUserTag("ValueTypeReturn", type);
			manager.addUserTag("FunctionName", mName);
			manager.addUserTag("NewFunctionName", member_name);


			for (size_t index=0; index<_count; ++index) {
				std::string type = _holder->getTypeDescription(mParams[index].type);
				if (type.empty()) return;
				manager.addUserTag(MyGUI::utility::toString("ValueType", index + 1), type);
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

			std::cout << "function  : " << member_name << std::endl;
		}

		void insertProperty(std::ofstream& _stream, ITypeHolder * _holder)
		{
			std::string template_name = MyGUI::utility::toString("Data/Templates/Property", (mGetProperty->getName().at(0) == 'i' ? "Is" : "Get"), "_template.h");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();

			std::string property_name = mName.substr(3);

			std::string type = _holder->getTypeDescription(mParams.at(0).type);
			if (type.empty()) return;
			manager.addUserTag("PropertyType", type);
			manager.addUserTag("PropertyName", property_name);

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

		void insertGetProperty(std::ofstream& _stream, ITypeHolder * _holder)
		{
			bool get_property = mName.at(0) != 'i';
			std::string template_name = MyGUI::utility::toString("Data/Templates/Property", (get_property ? "Get" : "Is"), "Only_template.h");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();

			std::string property_name = mName.substr(get_property ? 3 : 2);

			std::string type = _holder->getTypeDescription(mType);
			if (type.empty()) return;
			manager.addUserTag("PropertyType", type);
			manager.addUserTag("PropertyName", property_name);

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

			std::cout << "get property  : " << property_name << std::endl;
		}

	private:
		bool mProtection;
		bool mStatic;
		bool mConst;
		bool mExplicit;
		bool mInline;
		bool mVirtual;
		bool mIsSetProperty;
		bool mTemplate;
		bool mDeprecated;

		MemberFunction* mGetProperty;

		VectorParam mParams;
		VectorParam mTemplateParams;
	};

} // namespace wrapper

#endif // __MEMBER_FUNCTION_H__

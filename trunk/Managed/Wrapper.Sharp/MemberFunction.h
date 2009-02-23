/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_FUNCTION_H__
#define __MEMBER_FUNCTION_H__

#include "Enumerator.h"
#include "Member.h"
#include "Utility.h"

namespace wrapper
{

	class MemberFunction : public Member
	{
	public:
		struct Param { std::string type, name, def; };
		typedef std::vector<Param> VectorParam;
		typedef utility::Enumerator<VectorParam> EnumeratorParam;

	public:
		MemberFunction(xml::ElementPtr _element) :
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

			xml::ElementEnumerator info = _element->getElementEnumerator();
			while (info.next())
			{
				if (info->getName() == "type")
				{
					mType = info->getContent();
				}
				else if (info->getName() == "param")
				{
					Param pair_param;
					xml::ElementEnumerator param = info->getElementEnumerator();
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
					xml::ElementEnumerator template_info = info->getElementEnumerator();
					while (template_info.next("param"))
					{
						xml::ElementEnumerator param = template_info->getElementEnumerator();
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
					xml::ElementEnumerator para = info->getElementEnumerator();
					while (para.next("para"))
					{
						xml::ElementEnumerator xrefsect = para->getElementEnumerator();
						while (xrefsect.next("xrefsect"))
						{
							xml::ElementEnumerator xreftitle = xrefsect->getElementEnumerator();
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

		virtual void insertToTemplate(const std::string& _template, ITypeHolder * _holder, const std::string& _type)
		{
			if ( ! isNeedInsert() ) return;

			VectorString file_data;

			// ������� ��� ���������
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

			// ������ ��� ����������
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
					insert(outfile, _holder, _type);
				}
			}

			outfile.close();
		}

		// ��������� ������ ���������, ���� �������� true �� ������� ���������
		virtual bool postProccesing(Member* _member)
		{

			// ��������
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

			// �� ���� ��� ����� ��������
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
				mType.empty() || // ������������
				mStatic ||
				mProtection ||
				mDeprecated || // ���������� �������
				mTemplate || // �������
				( ! mName.empty() && mName[0] == '_' )  || // ��� ����������� �������������
				( ! mName.empty() && mName[0] == '~' ) // �����������
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
			TypeInfo type1(_type1);
			TypeInfo type2(_type2);
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

				// ����������� ���
				std::string name = mName.substr(3);
				if (("get" + name) != get->getName() && ("is" + name) != get->getName()) return false;
				if ( ! compireParam(mParams.at(0).type, get->getType()) ) return false;

				return true;
			}
			return false;
		}

		void insert(std::ofstream& _stream, ITypeHolder * _holder, const std::string& _type)
		{
			if (mGetProperty != nullptr)
			{
				insertProperty(_stream, _holder, _type);
			}
			else if (_holder->getMemberData(mName) == "GetPropertyOnly")
			{
				insertGetProperty(_stream, _holder, _type);
			}
			else
			{
				// ����� �� ����� �����������
				size_t count  = mParams.size();
				insertMethod(_stream, _holder, _type, count);

				// ������� ��������� ������ ��� ��������� ����������
				while (count > 0)
				{
					--count;
					if (mParams[count].def.empty()) break;
					_stream << std::endl;
					insertMethod(_stream, _holder, _type, count);
				}
			}
		}

		void insertMethod(std::ofstream& _stream, ITypeHolder * _holder, const std::string& _type, size_t _count)
		{
			std::string type = _holder->getTypeDescription(mType);
			if (type.empty()) return;

			std::string template_name = utility::toString("Data/", _type, "/Method", (mType == "void" ? "" : "Return"), _count, ".txt");

			std::string member_name = _holder->getMemberName(mName);
			if (member_name.empty()) return;

			addTag("MethodName", member_name);
			addTag("OriginalMethodName", mName);
			addTag("TypeName", type);
			addTag("OriginalTypeName", utility::trim_result(type));
			//addTag("MarshalTypeName", utility::trim_result(type));
			// ������ ��������� ���� ������ ����� ��������� � xml
			const ITypeHolder::VectorPairString& info = _holder->getTypeInfo(type);
			for(size_t index2=0; index2<info.size(); ++index2)
			{
				addTag(utility::toString(info[index2].first), info[index2].second);
			}


			for (size_t index=0; index<_count; ++index) {
				std::string type = _holder->getTypeDescription(mParams[index].type);
				if (type.empty()) return;

				addTag(utility::toString("TypeName", index + 1), type);
				addTag(utility::toString("OriginalTypeName", index + 1), utility::trim_result(type));
				//addTag(utility::toString("MarshalTypeName", index + 1), utility::trim_result(type));
				addTag(utility::toString("ValueName", index + 1), mParams[index].name);

				// ������ ��������� ���� ������ ����� ��������� � xml
				const ITypeHolder::VectorPairString& info = _holder->getTypeInfo(type);
				for(size_t index2=0; index2<info.size(); ++index2)
				{
					addTag(utility::toString(info[index2].first, index + 1), info[index2].second);
				}

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

			// ��� ���������
			if (data.size() > 3) {
				if (data[2] < 32) {
					data[0] = ' ';
					data[1] = ' ';
					data[2] = ' ';
				}
			}

			data = replaceTags(data);

			_stream << data;

			std::cout << "function  : " << member_name <<  "    (" << _type << ")" << std::endl;
		}

		void insertProperty(std::ofstream& _stream, ITypeHolder * _holder, const std::string& _type)
		{
			std::string template_name = utility::toString("Data/", _type, "/Property", (mGetProperty->getName().at(0) == 'i' ? "IsSet" : "GetSet"), ".txt");

			std::string property_name = mName.substr(3);

			std::string type = _holder->getTypeDescription(mParams.at(0).type);
			if (type.empty()) return;

			addTag("PropertyName", property_name);
			addTag("TypeName", type);
			addTag("OriginalTypeName", utility::trim_result(type));
			// ������ ��������� ���� ������ ����� ��������� � xml
			const ITypeHolder::VectorPairString& info = _holder->getTypeInfo(type);
			for(size_t index2=0; index2<info.size(); ++index2)
			{
				addTag(utility::toString(info[index2].first), info[index2].second);
				// ��� ������� ��������� �������
				addTag(utility::toString(info[index2].first, "1"), info[index2].second);
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

			// ��� ���������
			if (data.size() > 3) {
				if (data[2] < 32) {
					data[0] = ' ';
					data[1] = ' ';
					data[2] = ' ';
				}
			}

			data = replaceTags(data);

			_stream << data;

			std::cout << "property  : " << property_name <<  "    (" << _type << ")" << std::endl;
		}

		void insertGetProperty(std::ofstream& _stream, ITypeHolder * _holder, const std::string& _type)
		{
			bool get_property = mName.at(0) != 'i';
			std::string template_name = utility::toString("Data/", _type, "/Property", (get_property ? "Get" : "Is"), ".txt");

			std::string property_name = mName.substr(get_property ? 3 : 2);

			std::string type = _holder->getTypeDescription(mType);
			if (type.empty()) return;

			addTag("PropertyName", property_name);
			addTag("TypeName", type);
			addTag("OriginalTypeName", utility::trim_result(type));
			// ������ ��������� ���� ������ ����� ��������� � xml
			const ITypeHolder::VectorPairString& info = _holder->getTypeInfo(type);
			for(size_t index2=0; index2<info.size(); ++index2)
			{
				addTag(utility::toString(info[index2].first), info[index2].second);
				// ��� ������� ��������� �������
				addTag(utility::toString(info[index2].first, "1"), info[index2].second);
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

			// ��� ���������
			if (data.size() > 3) {
				if (data[2] < 32) {
					data[0] = ' ';
					data[1] = ' ';
					data[2] = ' ';
				}
			}

			data = replaceTags(data);

			_stream << data;

			std::cout << "get property  : " << property_name <<  "    (" << _type << ")" << std::endl;
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

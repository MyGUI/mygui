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
		MemberFunction(xml::ElementPtr _element) :
			Member(_element),
			mGetProperty(0),
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
					std::string real_name = "";
					// тип может быть сложно записан с сылкой
					xml::ElementEnumerator ref = info->getElementEnumerator();
					while (ref.next("ref"))
					{
						real_name = ref->getContent();
						break;
					}
					
					if (real_name.empty())
						mType = info->getContent();
					else
						mType = info->getContent() + real_name + info->getContent2();
				}
				else if (info->getName() == "param")
				{
					Param pair_param;
					xml::ElementEnumerator param = info->getElementEnumerator();
					while (param.next())
					{
						if (param->getName() == "type")
						{
							std::string real_name = "";
							// тип может быть сложно записан с сылкой
							xml::ElementEnumerator ref = param->getElementEnumerator();
							while (ref.next("ref"))
							{
								real_name = ref->getContent();
								break;
							}
							
							if (real_name.empty())
								pair_param.type = param->getContent();
							else
								pair_param.type = param->getContent() + real_name + param->getContent2();

						}
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

		virtual ~MemberFunction()
		{
		}

		size_t getParamCount() { return mParams.size(); }
		const std::string& getParamType(size_t _index) { return mParams.at(_index).type; }
		EnumeratorParam getParamEnumerator() { return EnumeratorParam(mParams.begin(), mParams.end()); }
		MemberFunction* getGetFunction() { return mGetProperty; }

		virtual void insertToTemplate(const std::string& _template, ITypeHolder * _holder, const std::string& _type)
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
					insert(outfile, _holder, _type);
				}
			}

			outfile.close();
		}

		// обработка других елементов, если вернется true то елемент удаляется
		virtual void postProccesing(Member* _member)
		{
			if (_member == this)
				return;

			// дубликат
			if (mName == _member->getName() &&
				mType == _member->getType() &&
				mKind == _member->getKind()
				)
			{
				MemberFunction* member = dynamic_cast<MemberFunction*>(_member);
				assert(member);

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
						member->setNeedPrint(false);
						return;
					}
				}
			}

			// мы себе уже нашли пропертю
			if (mGetProperty == 0 && mIsSetProperty && isGetProperty(_member))
			{
				mGetProperty = dynamic_cast<MemberFunction*>(_member);
				assert(mGetProperty);

				mGetProperty->setNeedPrint(false);
				return;
			}

			return;
		}

	private:
		void setNeedPrint(bool _value)
		{
			mNeedPrint = _value;
		}

		bool isNeedInsert()
		{
			if (
				!mNeedPrint ||
				mType.empty() || // конструкторы
				mStatic ||
				mProtection ||
				mDeprecated || // устаревшая функция
				//mTemplate || // шаблоны
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
			TypeInfo type1(_type1);
			TypeInfo type2(_type2);
			return type1.getType() == type2.getType();
		}

		bool isGetProperty(Member* _item)
		{
			if ( ! mIsSetProperty ) return false;
			if (_item->getKind() != "function") return false;

			MemberFunction* get = dynamic_cast<MemberFunction*>(_item);
			assert(get);

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

		void insert(std::ofstream& _stream, ITypeHolder * _holder, const std::string& _type)
		{
			if (mGetProperty != 0)
			{
				insertProperty(_stream, _holder, _type);
			}
			else
			{
				// метод со всеми параметрами
				size_t count  = mParams.size();
				insertMethod(_stream, _holder, _type, count);

				// пробуем сгенерить методы без дефолтных параметров
				/*while (count > 0)
				{
					--count;
					if (mParams[count].def.empty()) break;
					_stream << std::endl;
					insertMethod(_stream, _holder, _type, count);
				}*/
			}
		}

		void insertMethod(std::ofstream& _stream, ITypeHolder * _holder, const std::string& _type, size_t _count)
		{
			std::string type = _holder->getTypeDescription(mType);
			if (type.empty()) return;

			std::string templ = _holder->getMemberData(mName);
			if (templ.empty()) templ = utility::toString("Method", (mType == "void" ? "" : "Return"), _count, ".txt");

			std::string template_name = utility::toString("Data/", _type, "/", _holder->getTemplatePrefix(_type, mType, mParams), templ);

			std::string member_name = convertMemberName(mName);
			if (member_name.empty()) return;

			std::string property_name = mName.size() > 3 ? mName.substr(3) : mName;
			addTag("PropertyName", property_name);

			addTag("MethodName", member_name);
			addTag("OriginalMethodName", mName);
			addTag("OriginalTypeName", utility::trim_result(type));
			// теперь вставляем теги замены типов указанные в xml
			const ITypeHolder::VectorPairString& info = _holder->getTypeInfo(type);
			for(size_t index2=0; index2<info.size(); ++index2)
			{
				addTag(utility::toString(info[index2].first), info[index2].second);
			}


			for (size_t index=0; index<_count; ++index) {
				std::string type = _holder->getTypeDescription(mParams[index].type);
				if (type.empty()) return;

				addTag(utility::toString("OriginalTypeName", index + 1), utility::trim_result(type));
				addTag(utility::toString("ValueName", index + 1), mParams[index].name);

				// теперь вставляем теги замены типов указанные в xml
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

			// утф заголовки
			if (data.size() > 3) {
				if (data[2] < 32) {
					data[0] = ' ';
					data[1] = ' ';
					data[2] = ' ';
				}
			}

			data = replaceTags(data);

			_stream << data;

			std::cout << "method  : " << member_name <<  "    '" << template_name << "'" << std::endl;
		}

		void insertProperty(std::ofstream& _stream, ITypeHolder * _holder, const std::string& _type)
		{
			std::string templ = _holder->getMemberData(mName);
			if (templ.empty()) templ = utility::toString("Property", (mGetProperty->getName().at(0) == 'i' ? "IsSet" : "GetSet"), ".txt");

			std::string template_name = utility::toString("Data/", _type, "/", _holder->getTemplatePrefix(_type, mGetProperty->getType(), mParams), templ);

			std::string property_name = mName.substr(3);

			// тип в геттере
			std::string typeret = _holder->getTypeDescription(mGetProperty->getType());
			if (typeret.empty()) return;

			// тип в сеттере
			std::string type = _holder->getTypeDescription(mParams.at(0).type);
			if (type.empty()) return;

			addTag("PropertyName", property_name);
			addTag("OriginalTypeName", utility::trim_result(typeret));
			// для первого параметра сеттера
			addTag("OriginalTypeName1", utility::trim_result(type));
			// теперь вставляем теги замены типов указанные в xml
			const ITypeHolder::VectorPairString& info = _holder->getTypeInfo(type);
			for(size_t index2=0; index2<info.size(); ++index2)
			{
				addTag(utility::toString(info[index2].first), info[index2].second);
				// для первого параметра сеттера
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

			// утф заголовки
			if (data.size() > 3) {
				if (data[2] < 32) {
					data[0] = ' ';
					data[1] = ' ';
					data[2] = ' ';
				}
			}

			data = replaceTags(data);

			_stream << data;

			std::cout << "property  : " << property_name <<  "    '" << template_name << "'" << std::endl;
		}

		std::string convertMemberName(const std::string& _name)
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
		bool mNeedPrint;

		// мы не забираем указатель
		MemberFunction* mGetProperty;

		VectorParam mParams;
		VectorParam mTemplateParams;
	};

} // namespace wrapper

#endif // __MEMBER_FUNCTION_H__

/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_VARIABLE_H__
#define __MEMBER_VARIABLE_H__

#include "Member.h"

namespace wrapper
{

	class MemberVariable : public Member
	{
	public:
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;
		typedef std::vector<std::string> VectorString;

	public:
		MemberVariable(MyGUI::xml::ElementPtr _element) : Member(_element)
		{
			mProtection = _element->findAttribute("prot") != "public";

			// ищем дефолтные имена эвентов
			MyGUI::xml::ElementEnumerator info = _element->getElementEnumerator();
			while (info.next("detaileddescription"))
			{
				MyGUI::xml::ElementEnumerator para = info->getElementEnumerator();
				while (para.next("para"))
				{
					MyGUI::xml::ElementEnumerator parameterlist = para->getElementEnumerator();
					while (parameterlist.next("parameterlist"))
					{
						if (parameterlist->findAttribute("kind") != "param") continue;
						MyGUI::xml::ElementEnumerator parameteritem = parameterlist->getElementEnumerator();
						while (parameteritem.next("parameteritem"))
						{
							MyGUI::xml::ElementEnumerator parameternamelist = parameteritem->getElementEnumerator();
							while (parameternamelist.next("parameternamelist"))
							{
								MyGUI::xml::ElementEnumerator parametername = parameternamelist->getElementEnumerator();
								while (parametername.next("parametername"))
								{
									mFindParamsName.push_back(parametername->getContent());
								}
							}
						}
					}
				}
			}

		}

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

	private:

		bool isNeedInsert()
		{
			return ! mProtection;
		}

		void removePair(std::string & _name)
		{
			if ( ! utility::first(_name, "EventPair") ) return;

			size_t start = _name.find_first_of("<");
			size_t end = _name.find_last_of(">");

			if (start != std::string::npos && end != std::string::npos && start < end)
			{
				std::string inner_type = _name.substr(start + 1, end - start - 1);
				std::vector<std::string> inner_types = utility::split_params(inner_type);
				size_t count = inner_types.size();
				if (count == 2)
				{
					_name = inner_types[1];
					MyGUI::utility::trim(_name);
				}
			}
		}

		void insert(std::ofstream& _stream, ITypeHolder * _holder)
		{
			removePair(mType);

			std::string type = _holder->getTypeDescription(mType);
			MyGUI::utility::trim(type);


			size_t start = type.find_first_of("<");
			size_t end = type.find_last_of(">");
			if (start != std::string::npos && end != std::string::npos && start < end)
			{
				std::string inner_type = type.substr(start + 1, end - start - 1);
				std::vector<std::string> inner_types = utility::split_params(inner_type);
				size_t count = inner_types.size();
				for (size_t index=0; index<count; ++index)
				{
					MyGUI::utility::trim(inner_types[index]);
					mParams.push_back( PairString( inner_types[index],
						(index < mFindParamsName.size() && !mFindParamsName[index].empty()) ? mFindParamsName[index] : MyGUI::utility::toString("_value", index+1)
						));
				}
			}
			else
			{
				return;
			}

			const std::string prefix = "MyGUI::delegates::CDelegate";
			if (type.size() < prefix.size()+1 || type.substr(0, prefix.size()+1) != MyGUI::utility::toString(prefix, mParams.size()))
			{
				mParams.clear();
				return;
			}

			bool prefix_event = true;
			const std::string prefix1 = "event";
			const std::string prefix2 = "request";

			std::string event_name = mName;

			// внутрение евенты
			if (utility::first(event_name, "_"))
			{
				return;
			}
			else if (utility::first(event_name, prefix1))
			{
				prefix_event = true;
				event_name = event_name.substr(prefix1.size());
			}
			else if (utility::first(event_name, prefix2))
			{
				prefix_event = false;
				event_name = event_name.substr(prefix2.size());
			}
			

			std::string template_name = MyGUI::utility::toString("Data/Templates/Delegate", (prefix_event ? "Event" : "Request"), mParams.size(), "_template.h");

			MyGUI::LanguageManager& manager = MyGUI::LanguageManager::getInstance();
			manager.addUserTag("DelegateName", event_name);

			for (size_t index=0; index<mParams.size(); ++index) {
				manager.addUserTag(MyGUI::utility::toString("ValueType", index + 1), mParams[index].first);
				manager.addUserTag(MyGUI::utility::toString("ValueName", index + 1), mParams[index].second);
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

			std::cout << (prefix_event ? "event  : " : "request  : ")  << event_name << std::endl;
		}

		private:
			bool mProtection;
			VectorPairString mParams;
			VectorString mFindParamsName;
	};

} // namespace wrapper

#endif // __MEMBER_VARIABLE_H__

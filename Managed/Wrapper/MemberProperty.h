/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_PROPERTY_H__
#define __MEMBER_PROPERTY_H__

#include "MemberFunction.h"

namespace wrapper
{

	class MemberProperty : public Member
	{
	public:
		MemberProperty() :
			Member(),
			mFuncGet(nullptr),
			mFuncSet(nullptr)
		{
		}

		MemberProperty(MemberFunction * _func_set, MemberFunction * _func_get) :
			Member(),
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
				if (item->find("/*InsertPoint*/") != std::string::npos) {
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

		MemberFunction* mFuncGet;
		MemberFunction* mFuncSet;
	};

} // namespace wrapper

#endif // __MEMBER_PROPERTY_H__

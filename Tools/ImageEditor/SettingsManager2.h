/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f620c4fc_5a1d_4280_86ca_ac59deeae6e0_
#define _f620c4fc_5a1d_4280_86ca_ac59deeae6e0_

#include <MyGUI.h>
#include "pugixml.hpp"
#include "StringUtility.h"
#include "sigslot.h"

namespace tools
{

	class SettingsManager :
		public MyGUI::Singleton<SettingsManager>
	{
	public:
		SettingsManager();
		virtual ~SettingsManager();

		bool loadSettingsFile(const std::string& _fileName);
		void saveSettingsFile(const std::string& _fileName);

		bool loadUserSettingsFile(const std::string& _fileName);
		void saveUserSettingsFile();

		bool getExistValue(const std::string& _path);

		template <typename Type>
		bool tryGetValue(const std::string& _path, Type& _result)
		{
			_result = Type();
			if (getExistValue(_path))
			{
				std::string value = getValueString(_path);
				return MyGUI::utility::parseComplex<Type>(value, _result);
			}
			return false;
		}

		std::string getValueString(const std::string& _path);
		void setValueString(const std::string& _path, const std::string& _value);

		template <typename Type>
		Type getValue(const std::string& _path)
		{
			return MyGUI::utility::parseValue<Type>(getValueString(_path));
		}

		template <typename Type>
		void setValue(const std::string& _path, const Type& value)
		{
			setValueString(_path, MyGUI::utility::toString(value));
		}

		typedef std::vector<std::string> VectorString;
		VectorString getValueListString(const std::string& _path);

		template <typename Type>
		std::vector<Type> getValueList(const std::string& _path)
		{
			VectorString resultString = getValueListString(_path);
			std::vector<Type> result;
			result.reserve(resultString.size());

			for (auto item = resultString.begin(); item != resultString.end(); item ++)
				result.push_back(MyGUI::utility::parseValue<Type>(*item));

			return result;
		}

		void setValueListString(const std::string& _path, const VectorString& _values);

		template <typename Type>
		void setValueList(const std::string& _path, const std::vector<Type>& _values)
		{
			VectorString values;
			values.reserve(_values.size());

			for (auto item = _values.begin(); item != _values.end(); item ++)
				values.push_back(MyGUI::utility::toString(*item));

			setValueListString(_path, values);
		}

		pugi::xml_node getRootNode() { return mDocument->document_element(); }

		sigslot::signal1<const std::string&> eventSettingsChanged;

	private:
		void mergeNodes(pugi::xml_node _node1, pugi::xml_node _node2);
		void mergeAttributes(pugi::xml_node _node1, pugi::xml_node _node2);

	private:
		pugi::xml_document* mDocument;
		pugi::xml_document* mUserDocument;
		std::string mUserSettingsFileName;
	};

}

#endif

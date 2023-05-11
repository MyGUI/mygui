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
#include <type_traits>

namespace tools
{

	class MYGUI_EXPORT_DLL SettingsManager
	{
		MYGUI_SINGLETON_DECLARATION(SettingsManager);
	public:
		SettingsManager();
		virtual ~SettingsManager();

		bool loadSettingsFile(std::string_view _fileName);
		void saveSettingsFile(std::string_view _fileName);

		bool loadUserSettingsFile(std::string_view _fileName);
		void saveUserSettingsFile();

		bool getExistValue(std::string_view _path);

		template <typename Type>
		bool tryGetValue(std::string_view _path, Type& _result)
		{
			_result = Type();
			if (getExistValue(_path))
			{
				std::string value = getValue(_path);
				return MyGUI::utility::parseComplex<Type>(value, _result);
			}
			return false;
		}

		std::string getValue(std::string_view _path);
		void setValue(std::string_view _path, std::string_view _value);

		template <typename Type>
		Type getValue(std::string_view _path)
		{
			return MyGUI::utility::parseValue<Type>(getValue(_path));
		}

		template <class Type, typename = std::enable_if_t<!std::is_convertible_v<Type, std::string_view>>>
		void setValue(std::string_view _path, const Type& value)
		{
			setValue(_path, MyGUI::utility::toString(value));
		}

		typedef std::vector<std::string> VectorString;
		VectorString getValueList(std::string_view _path);

		template <typename Type>
		std::vector<Type> getValueList(std::string_view _path)
		{
			VectorString resultString = getValueList(_path);
			std::vector<Type> result;
			result.reserve(resultString.size());

			for (VectorString::const_iterator item = resultString.begin(); item != resultString.end(); item ++)
				result.push_back(MyGUI::utility::parseValue<Type>(*item));

			return result;
		}

		void setValueList(std::string_view _path, const VectorString& _values);

		template <typename Type>
		void setValueList(std::string_view _path, const std::vector<Type>& _values)
		{
			VectorString values;
			values.reserve(_values.size());

			for (typename std::vector<Type>::const_iterator item = _values.begin(); item != _values.end(); item ++)
				values.push_back(MyGUI::utility::toString(*item));

			setValueList(_path, values);
		}

		pugi::xpath_node_set getValueNodeList(std::string_view _path);

		sigslot::signal1<std::string_view> eventSettingsChanged;

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

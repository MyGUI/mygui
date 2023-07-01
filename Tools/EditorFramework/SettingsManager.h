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
	class NullTerminatedStringView
	{
		std::string_view mValue;

	public:
		NullTerminatedStringView(const std::string& string) :
			mValue(string)
		{
		}
		template<size_t num>
		NullTerminatedStringView(const char (&str)[num]) :
			mValue(str, num)
		{
		}
		NullTerminatedStringView(const MyGUI::UString& string) :
			mValue(string)
		{
		}

		std::string_view c_str() const
		{
			return mValue;
		}
	};

	class CString
	{
		const char* mValue;

	public:
		CString(const std::string& string) :
			mValue(string.data())
		{
		}
		CString(const char* string) :
			mValue(string)
		{
		}
		CString(NullTerminatedStringView string) :
			mValue(string.c_str().data())
		{
		}

		const char* c_str() const
		{
			return mValue;
		}
	};

	class MYGUI_EXPORT_DLL SettingsManager
	{
		MYGUI_SINGLETON_DECLARATION(SettingsManager);

	public:
		SettingsManager();
		virtual ~SettingsManager();

		bool loadSettingsFile(CString _fileName);
		void saveSettingsFile(CString _fileName);

		bool loadUserSettingsFile(std::string_view _fileName);
		void saveUserSettingsFile();

		bool getExistValue(CString _path);

		template<typename Type>
		bool tryGetValue(CString _path, Type& _result)
		{
			_result = Type();
			if (getExistValue(_path))
			{
				std::string value = getValue(_path);
				return MyGUI::utility::parseComplex<Type>(value, _result);
			}
			return false;
		}

		std::string getValue(CString _path);

		template<typename Type>
		Type getValue(CString _path)
		{
			return MyGUI::utility::parseValue<Type>(getValue(_path));
		}

		void setValue(NullTerminatedStringView _path, NullTerminatedStringView _value)
		{
			setValueImpl(_path.c_str(), _value);
		}

		template<class Type, typename = std::enable_if_t<!std::is_convertible_v<Type, std::string_view>>>
		void setValue(NullTerminatedStringView _path, const Type& value)
		{
			setValueImpl(_path.c_str(), MyGUI::utility::toString(value));
		}

		using VectorString = std::vector<std::string>;
		VectorString getValueList(std::string_view _path);

		template<typename Type>
		std::vector<Type> getValueList(std::string_view _path)
		{
			VectorString resultString = getValueList(_path);
			std::vector<Type> result;
			result.reserve(resultString.size());

			for (const auto& item : resultString)
				result.push_back(MyGUI::utility::parseValue<Type>(item));

			return result;
		}

		void setValueList(NullTerminatedStringView _path, const VectorString& _values)
		{
			setValueListImpl(_path.c_str(), _values);
		}

		template<typename Type>
		void setValueList(NullTerminatedStringView _path, const std::vector<Type>& _values)
		{
			VectorString values;
			values.reserve(_values.size());

			for (typename std::vector<Type>::const_iterator item = _values.begin(); item != _values.end(); item++)
				values.push_back(MyGUI::utility::toString(*item));

			setValueList(_path, values);
		}

		pugi::xpath_node_set getValueNodeList(std::string_view _path);

		sigslot::signal1<std::string_view> eventSettingsChanged;

	private:
		void setValueImpl(std::string_view _path, CString _value);
		void setValueListImpl(std::string_view _path, const VectorString& _values);
		void mergeNodes(pugi::xml_node _nodeTarget, pugi::xml_node _nodeSource);
		void mergeAttributes(pugi::xml_node _nodeTarget, pugi::xml_node _nodeSource);

	private:
		pugi::xml_document* mDocument{nullptr};
		pugi::xml_document* mUserDocument{nullptr};
		std::string mUserSettingsFileName;
	};

}

#endif

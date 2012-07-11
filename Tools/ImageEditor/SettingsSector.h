/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SETTINGS_SECTOR_H__
#define __SETTINGS_SECTOR_H__

#include <MyGUI.h>

namespace tools
{
	class SettingsSector;
	typedef MyGUI::delegates::CDelegate2<SettingsSector*, const MyGUI::UString&> EventSettingsChanged;
	typedef std::vector<SettingsSector*> VectorSettingsSector;
	typedef MyGUI::Enumerator<VectorSettingsSector> EnumeratorSettingsSector;

	class SettingsSector :
		public MyGUI::ISerializable
	{
	public:
		SettingsSector();
		virtual ~SettingsSector();

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);

		const MyGUI::UString& getName() const;
		void setName(const MyGUI::UString& _value);

		bool getExistProperty(const MyGUI::UString& _propertyName);

		const MyGUI::UString& getPropertyValue(const MyGUI::UString& _propertyName);

		template <typename Type>
		Type getPropertyValue(const MyGUI::UString& _propertyName)
		{
			return MyGUI::utility::parseValue<Type>(getPropertyValue(_propertyName));
		}

		void setPropertyValue(const MyGUI::UString& _propertyName, const MyGUI::UString& _propertyValue);

		void setPropertyValue(const MyGUI::UString& _propertyName, const std::string& _propertyValue);

		typedef std::vector<MyGUI::UString> VectorUString;
		void setPropertyValueList(const MyGUI::UString& _propertyName, const VectorUString& _propertyValue);

		typedef std::vector<std::string> VectorString;
		void setPropertyValueList(const MyGUI::UString& _propertyName, const VectorString& _propertyValue);

		template <typename Type>
		void setPropertyValue(const MyGUI::UString& _propertyName, Type _value)
		{
			return setPropertyValue(_propertyName, MyGUI::utility::toString(_value));
		}

		VectorUString getPropertyValueList(const MyGUI::UString& _propertyName);

		template <typename Type>
		std::vector<Type> getPropertyValueList(const MyGUI::UString& _propertyName)
		{
			std::vector<Type> result;

			VectorUString values = getPropertyValueList(_propertyName);
			for (VectorUString::const_iterator item = values.begin(); item != values.end(); ++item)
				result.push_back(MyGUI::utility::parseValue<Type>(*item));

			return result;
		}

		void clearProperty(const MyGUI::UString& _propertyName);

		EventSettingsChanged eventSettingsChanged;

	private:
		MyGUI::UString mName;
		typedef std::map<MyGUI::UString, MyGUI::UString> MapUString;
		MapUString mProperties;
	};

} // namespace tools

#endif // __SETTINGS_SECTOR_H__

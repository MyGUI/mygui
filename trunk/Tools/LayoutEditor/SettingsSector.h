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

		const MyGUI::UString& getName();
		void setName(const MyGUI::UString& _value);

		bool getExistProperty(const MyGUI::UString& _propertName);

		const MyGUI::UString& getPropertyValue(const MyGUI::UString& _propertName);
		void setPropertyValue(const MyGUI::UString& _propertName, const MyGUI::UString& _propertValue);
		void setPropertyValue(const MyGUI::UString& _propertName, const std::string& _propertValue) { setPropertyValue(_propertName, MyGUI::UString(_propertValue)); }

		template <typename Type>
		Type getPropertyValue(const MyGUI::UString& _propertyName)
		{
			return MyGUI::utility::parseValue<Type>(getPropertyValue(_propertyName));
		}

		template <typename Type>
		void setPropertyValue(const MyGUI::UString& _propertyName, Type _value)
		{
			return setPropertyValue(_propertyName, MyGUI::utility::toString(_value));
		}

		EventSettingsChanged eventSettingsChanged;

	private:
		MyGUI::UString mName;
		typedef std::map<MyGUI::UString, MyGUI::UString> MapUString;
		MapUString mProperties;
	};

} // namespace tools

#endif // __SETTINGS_SECTOR_H__

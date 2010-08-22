/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SETTINGS_MANAGER_H__
#define __SETTINGS_MANAGER_H__

#include <MyGUI.h>

namespace tools
{

	typedef std::vector<MyGUI::UString> VectorUString;
	typedef MyGUI::delegates::CMultiDelegate2<const MyGUI::UString&, const MyGUI::UString&> EventSettingsChanged;

	class SettingsManager :
		public MyGUI::Singleton<SettingsManager>
	{
	public:
		SettingsManager();
		virtual ~SettingsManager();

		void initialise();
		void shutdown();

		const VectorUString& getRecentFiles() { return mRecentFiles; }
		const VectorUString& getAdditionalPaths() { return mAdditionalPaths; }

		void addRecentFile(const MyGUI::UString& _fileName);

		void setProperty(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName, const MyGUI::UString& _propertyValue);
		MyGUI::UString getProperty(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName);

		template <typename Type>
		Type getPropertyValue(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName)
		{
			return MyGUI::utility::parseValue<Type>(getProperty(_sectionName, _propertyName));
		}

		template <typename Type>
		void setPropertyValue(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName, Type _value)
		{
			return setProperty(_sectionName, _propertyName, MyGUI::utility::toString(_value));
		}

		EventSettingsChanged eventSettingsChanged;

	private:
		void loadSettings(const MyGUI::UString& _fileName, bool _internal);
		void saveSettings(const MyGUI::UString& _fileName);

		void setProperty(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName, const MyGUI::UString& _propertyValue, bool _event);

		bool isNeedSolutionLoad(MyGUI::xml::ElementEnumerator _field);

	private:
		std::vector<MyGUI::UString> mRecentFiles;
		std::vector<MyGUI::UString> mAdditionalPaths;

		typedef std::map<MyGUI::UString, MyGUI::UString> MapUString;
		typedef std::map<MyGUI::UString, MapUString> MapSection;
		MapSection mSections;
	};

} // namespace tools

#endif // __SETTINGS_MANAGER_H__

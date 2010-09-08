/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __BACKWARD_COMPATIBILITY_MANAGER_H__
#define __BACKWARD_COMPATIBILITY_MANAGER_H__

#include <MyGUI.h>

namespace tools
{
	class BackwardCompatibilityManager :
		public MyGUI::Singleton<BackwardCompatibilityManager>
	{
	public:
		BackwardCompatibilityManager();
		virtual ~BackwardCompatibilityManager();

		void initialise();
		void shutdown();

		const MyGUI::VectorString& getVersions();
		const std::string& getDefaultVersion();

		const std::string& getCurrentVersion();
		void setCurrentVersion(const std::string& _value);

		std::string getPropertyName(const std::string& _propertyName);
		void serialiseProperty(MyGUI::xml::Element* _node, const std::string& _widgetType, const MyGUI::PairString& _property, bool _compatibility);

	private:
		void notifySettingsChanged(const MyGUI::UString& _sectionName, const MyGUI::UString& _propertyName);

	private:
		MyGUI::VectorString mVersions;
		std::string mCurrentVersion;
		MyGUI::MapString mPropertyNames;
	};

} // namespace tools

#endif // __BACKWARD_COMPATIBILITY_MANAGER_H__

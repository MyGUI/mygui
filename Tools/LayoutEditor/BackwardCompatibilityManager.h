/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _af635c59_a426_4646_98a5_1e3a7fe03726_
#define _af635c59_a426_4646_98a5_1e3a7fe03726_

#include <MyGUI.h>
#include "sigslot.h"

namespace tools
{

	class BackwardCompatibilityManager : public sigslot::has_slots<>
	{
		MYGUI_SINGLETON_DECLARATION(BackwardCompatibilityManager);

	public:
		BackwardCompatibilityManager();

		void initialise();
		void shutdown();

		const MyGUI::VectorString& getVersions() const;
		const std::string& getDefaultVersion() const;

		const std::string& getCurrentVersion() const;
		void setCurrentVersion(std::string_view _value);

		void serialiseProperty(
			MyGUI::xml::Element* _node,
			std::string_view _widgetType,
			const MyGUI::PairString& _property,
			bool _compatibility);

	private:
		void notifySettingsChanged(std::string_view _path);

	private:
		MyGUI::VectorString mVersions;
		std::string mCurrentVersion;
		MyGUI::MapString mPropertyNames;
	};

}

#endif

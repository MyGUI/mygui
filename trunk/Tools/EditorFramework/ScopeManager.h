/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _a7965834_942c_4633_8ac1_01eedbd48d34_
#define _a7965834_942c_4633_8ac1_01eedbd48d34_

#include <MyGUI_Singleton.h>
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ScopeManager :
		public MyGUI::Singleton<ScopeManager>,
		public sigslot::has_slots<>
	{
	public:
		ScopeManager();
		virtual ~ScopeManager();

		void initialise();
		void shutdown();

		const std::string& getCurrentScope() const;

		sigslot::signal1<const std::string&> eventChangeScope;

	private:
		void commandChangeScope(const MyGUI::UString& _commandName, bool& _result);

	private:
		std::string mCurrentScope;
	};

}

#endif

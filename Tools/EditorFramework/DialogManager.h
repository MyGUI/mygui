/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _625c2a75_11e0_40d3_9ce1_78e805050dcf_
#define _625c2a75_11e0_40d3_9ce1_78e805050dcf_

#include <MyGUI.h>
#include "Dialog.h"

namespace tools
{

	class MYGUI_EXPORT_DLL DialogManager
	{
		MYGUI_SINGLETON_DECLARATION(DialogManager);

	public:
		DialogManager() :
			mSingletonHolder(this)
		{
		}
		void initialise();
		void shutdown();

		bool getAnyDialog();
		void endTopDialog(bool _result = false);

	public:
		void _addDialog(Dialog* _modal);
		void _removeDialog(Dialog* _modal);

	private:
		using VectorDialog = std::vector<Dialog*>;
		VectorDialog mDialogs;
	};

}

#endif

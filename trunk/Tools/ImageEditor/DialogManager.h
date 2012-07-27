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

	class MYGUI_EXPORT_DLL DialogManager :
		public MyGUI::Singleton<DialogManager>
	{
	public:
		DialogManager();
		virtual ~DialogManager();

		void initialise();
		void shutdown();

		bool getAnyDialog();
		void endTopDialog(bool _result = false);

	public:
		void _addDialog(Dialog* _modal);
		void _removeDialog(Dialog* _modal);

	private:
		typedef std::vector<Dialog*> VectorDialog;
		VectorDialog mDialogs;
	};

}

#endif

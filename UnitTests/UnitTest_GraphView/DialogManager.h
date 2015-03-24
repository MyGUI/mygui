/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef DIALOG_MANAGER_H_
#define DIALOG_MANAGER_H_

#include <MyGUI.h>
#include "Dialog.h"

namespace tools
{
	class DialogManager :
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

} // namespace tools

#endif // DIALOG_MANAGER_H_

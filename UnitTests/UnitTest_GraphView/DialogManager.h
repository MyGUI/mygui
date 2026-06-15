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
	class DialogManager
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

} // namespace tools

#endif // DIALOG_MANAGER_H_

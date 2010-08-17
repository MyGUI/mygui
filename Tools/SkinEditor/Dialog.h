/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <MyGUI.h>

namespace tools
{
	class Dialog
	{
	public:
		typedef MyGUI::delegates::CDelegate2<Dialog*, bool> EventHandle_Result;

	public:
		Dialog();
		virtual ~Dialog();

		virtual void setVisible(bool _value) = 0;
		virtual bool getVisible() = 0;

		EventHandle_Result eventEndDialog;

		static bool getAnyDialog();
		static void endTopDialog();

	protected:
		void addDialog(Dialog* _modal);
		void removeDialog(Dialog* _modal);

	private:
		static std::vector<Dialog*> mDialogs;
	};

} // namespace tools

#endif // __DIALOG_H__

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

		void setDialogRoot(MyGUI::Widget* _root);
		bool isDialogModal();

		void doModal();
		void endModal();

		EventHandle_Result eventEndDialog;

	protected:
		virtual void onDoModal() { }
		virtual void onEndModal() { }

	private:
		bool mModal;
		MyGUI::Widget* mRootWidget;
	};

} // namespace tools

#endif // __DIALOG_H__

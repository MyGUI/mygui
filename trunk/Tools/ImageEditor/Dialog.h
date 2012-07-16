/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/

#ifndef _1b12f1d3_3f8d_4436_a78b_bac3214b47db_
#define _1b12f1d3_3f8d_4436_a78b_bac3214b47db_

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

}

#endif

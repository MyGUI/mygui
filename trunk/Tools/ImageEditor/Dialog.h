/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/

#ifndef _1b12f1d3_3f8d_4436_a78b_bac3214b47db_
#define _1b12f1d3_3f8d_4436_a78b_bac3214b47db_

#include <MyGUI.h>
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL Dialog
	{
	public:
		Dialog();
		virtual ~Dialog();

		void setDialogRoot(MyGUI::Widget* _root);
		bool isDialogModal();

		void doModal();
		void endModal();

		sigslot::signal2<Dialog*, bool> eventEndDialog;

	protected:
		virtual void onDoModal() { }
		virtual void onEndModal() { }

	private:
		bool mModal;
		MyGUI::Widget* mRootWidget;
	};

}

#endif

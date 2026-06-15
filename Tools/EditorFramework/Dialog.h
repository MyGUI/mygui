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
		virtual ~Dialog() = default;

		void setDialogRoot(MyGUI::Widget* _root);
		bool isDialogModal() const;

		void doModal();
		void endModal();

		sigslot::signal2<Dialog*, bool> eventEndDialog;

	protected:
		virtual void onDoModal()
		{
		}
		virtual void onEndModal()
		{
		}

	private:
		bool mModal{false};
		MyGUI::Widget* mRootWidget{nullptr};
	};

}

#endif

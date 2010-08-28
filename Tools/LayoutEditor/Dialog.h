/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __DIALOG_H__
#define __DIALOG_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class Dialog :
		public wraps::BaseLayout
	{
	public:
		typedef MyGUI::delegates::CDelegate2<Dialog*, bool> EventHandle_Result;

	public:
		Dialog();
		Dialog(const std::string& _layout);
		virtual ~Dialog();

		void doModal();
		void endModal();

		EventHandle_Result eventEndDialog;

	protected:
		virtual void onDoModal() { }
		virtual void onEndModal() { }

	private:
		bool mModal;
	};

} // namespace tools

#endif // __DIALOG_H__

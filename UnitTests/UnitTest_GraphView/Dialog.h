/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef DIALOG_H_
#define DIALOG_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class Dialog : public wraps::BaseLayout
	{
	public:
		using EventHandle_Result = MyGUI::delegates::Delegate<Dialog*, bool>;

	public:
		Dialog();
		Dialog(std::string_view _layout);

		void doModal();
		void endModal();

		EventHandle_Result eventEndDialog;

	protected:
		virtual void onDoModal()
		{
		}
		virtual void onEndModal()
		{
		}

	private:
		bool mModal;
	};

} // namespace tools

#endif // DIALOG_H_

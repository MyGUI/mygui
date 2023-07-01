/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _dc929b1d_40a7_4d4d_b008_318d8f7bc468_
#define _dc929b1d_40a7_4d4d_b008_318d8f7bc468_

#include "Control.h"
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL BackgroundControl : public Control, public sigslot::has_slots<>
	{
	public:
		~BackgroundControl() override;

		MyGUI::Widget* getCanvas();

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

	private:
		void setColour(const MyGUI::Colour& _colour);
		void notifySettingsChanged(std::string_view _path);

	private:
		MyGUI::Widget* mBackground{nullptr};
		MyGUI::Widget* mCanvas{nullptr};
		std::string mColourValueName;
	};

}

#endif

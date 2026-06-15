/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/
#ifndef STACK_PANEL_H_
#define STACK_PANEL_H_

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class StackPanel : public Panel
	{
		MYGUI_RTTI_DERIVED(StackPanel)

	public:
		const IntSize& getSpacer() const;
		void setSpacer(const IntSize& _value);

	protected:
		IntSize overrideMeasure(const IntSize& _sizeAvailable) override;
		void overrideArrange() override;

	private:
		IntSize mSpacer;
	};

} // namespace MyGUI

#endif // STACK_PANEL_H_

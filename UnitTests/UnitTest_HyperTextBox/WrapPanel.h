/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/
#ifndef WRAP_PANEL_H_
#define WRAP_PANEL_H_

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class WrapPanel : public Panel
	{
		MYGUI_RTTI_DERIVED(WrapPanel)

	public:
		Align getContentAlign() const;
		void setContentAlign(Align _value);

		const IntSize& getSpacer() const;
		void setSpacer(const IntSize& _value);

		bool getContentFloat() const;
		void setContentFloat(bool _value);

		Align getSnapFloat() const;
		void setSnapFloat(Align _value);

	protected:
		IntSize overrideMeasure(const IntSize& _sizeAvailable) override;
		void overrideArrange() override;

	private:
		void alignChildLine(size_t _startIndex, size_t _stopIndex, const IntCoord& _coordAvailable, int _lineWidth);

		IntSize simpleMeasure(const IntSize& _sizeAvailable);
		void simpleArrange();

		IntSize floatMeasure(const IntSize& _sizeAvailable);
		void floatArrange();

	private:
		Align mContentAlign;
		Align mSnapFloat;
		bool mContentFloat{false};
		IntSize mSpacer;
	};

} // namespace MyGUI

#endif // WRAP_PANEL_H_

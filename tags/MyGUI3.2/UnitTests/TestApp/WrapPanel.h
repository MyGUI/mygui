/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/
#ifndef __WRAP_PANEL_H__
#define __WRAP_PANEL_H__

#include "MyGUI_Prerequest.h"
#include "Panel.h"

namespace MyGUI
{

	class WrapPanel :
		public Panel
	{
		MYGUI_RTTI_DERIVED( WrapPanel )

	public:
		WrapPanel();

		Align getContentAlign() const;
		void setContentAlign(Align _value);

		const IntSize& getSpacer() const;
		void setSpacer(const IntSize& _value);

		bool getContentFloat() const;
		void setContentFloat(bool _value);

		Align getSnapFloat() const;
		void setSnapFloat(Align _value);

	protected:
		virtual IntSize overrideMeasure(const IntSize& _sizeAvailable);
		virtual void overrideArrange();

	private:
		void alignChildLine(size_t _startIndex, size_t _stopIndex, const IntCoord& _coordAvailable, int _lineWidth);

		IntSize simpleMeasure(const IntSize& _sizeAvailable);
		void simpleArrange();

		IntSize floatMeasure(const IntSize& _sizeAvailable);
		void floatArrange();

	private:
		Align mContentAlign;
		Align mSnapFloat;
		bool mContentFloat;
		IntSize mSpacer;
	};

} // namespace MyGUI

#endif // __WRAP_PANEL_H__

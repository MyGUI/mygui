/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __COLOUR_PANEL_H__
#define __COLOUR_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace demo
{

	ATTRIBUTE_CLASS_LAYOUT(ColourPanel, "ColourPanel.layout");
	class ColourPanel :
		public wraps::BaseLayout
	{
	public:
		ColourPanel();
		virtual ~ColourPanel();

		void setColour(const MyGUI::Colour& _colour);
		const MyGUI::Colour& getColour() { return mCurrentColour; }

		MyGUI::delegates::CDelegate1<ColourPanel*> eventColourAccept;

	private:
		void notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top);
		void notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyScrollChangePosition(MyGUI::VScrollPtr _sender, size_t _position);
		void notifyEditTextChange(MyGUI::EditPtr _sender);
		void notifyMouseButtonClick(MyGUI::WidgetPtr _sender);

		void updateFirst();

		void createTexture();
		void updateTexture(const MyGUI::Colour& _colour);
		void destroyTexture();

		void updateFromPoint(const MyGUI::IntPoint& _point);
		void updateFromColour(const MyGUI::Colour& _colour);

		MyGUI::Colour getSaturate(const MyGUI::Colour& _colour);

		float & byIndex(MyGUI::Colour& _colour, size_t _index);

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mColourRect, "widget_ColourRect");
		MyGUI::StaticImagePtr mColourRect;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mColourView, "widget_ColourView");
		MyGUI::WidgetPtr mColourView;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mImageColourPicker, "image_Picker");
		MyGUI::StaticImagePtr mImageColourPicker;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditRed, "edit_Red");
		MyGUI::EditPtr mEditRed;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditGreen, "edit_Green");
		MyGUI::EditPtr mEditGreen;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditBlue, "edit_Blue");
		MyGUI::EditPtr mEditBlue;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mScrollRange, "scroll_Range");
		MyGUI::VScrollPtr mScrollRange;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mOk, "button_OK");
		MyGUI::ButtonPtr mOk;

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mBaseColour;

		MyGUI::RawRect * mRawColourView;

		std::vector<MyGUI::Colour> mColourRange;

		MyGUI::ITexture* mTexture;
	};

} // namespace demo

#endif // __COLOUR_PANEL_H__

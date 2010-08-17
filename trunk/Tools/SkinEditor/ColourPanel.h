/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __COLOUR_PANEL_H__
#define __COLOUR_PANEL_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"
#include "Dialog.h"

namespace tools
{

	ATTRIBUTE_CLASS_LAYOUT(ColourPanel, "ColourPanel.layout");
	class ColourPanel :
		public wraps::BaseLayout,
		public Dialog
	{
	public:
		ColourPanel();
		virtual ~ColourPanel();

		void setColour(const MyGUI::Colour& _colour);
		const MyGUI::Colour& getColour() { return mCurrentColour; }

		virtual void setVisible(bool _value);
		virtual bool getVisible();

	private:
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyScrollChangePosition(MyGUI::VScroll* _sender, size_t _position);
		void notifyEditTextChange(MyGUI::Edit* _sender);
		void notifyMouseButtonClickOk(MyGUI::Widget* _sender);
		void notifyMouseButtonClickCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

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
		MyGUI::StaticImage* mColourRect;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mColourView, "widget_ColourView");
		MyGUI::Widget* mColourView;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mImageColourPicker, "image_Picker");
		MyGUI::StaticImage* mImageColourPicker;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditRed, "edit_Red");
		MyGUI::Edit* mEditRed;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditGreen, "edit_Green");
		MyGUI::Edit* mEditGreen;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditBlue, "edit_Blue");
		MyGUI::Edit* mEditBlue;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mScrollRange, "scroll_Range");
		MyGUI::VScroll* mScrollRange;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mOk, "button_OK");
		MyGUI::Button* mOk;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mCancel, "button_Cancel");
		MyGUI::Button* mCancel;

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mBaseColour;

		std::vector<MyGUI::Colour> mColourRange;

		MyGUI::ITexture* mTexture;
		MyGUI::UString mTextureName;
	};

} // namespace tools

#endif // __COLOUR_PANEL_H__

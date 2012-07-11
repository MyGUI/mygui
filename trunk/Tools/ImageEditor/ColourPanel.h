/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef __COLOUR_PANEL_H__
#define __COLOUR_PANEL_H__

#include <MyGUI.h>
#include "Tools/Dialog.h"

namespace tools
{
	typedef MyGUI::delegates::CDelegate1<const MyGUI::Colour&> Event_PreviewColour;

	ATTRIBUTE_CLASS_LAYOUT(ColourPanel, "ColourPanel.layout");
	class ColourPanel :
		public Dialog
	{
	public:
		ColourPanel();
		virtual ~ColourPanel();

		void setColour(const MyGUI::Colour& _colour);
		const MyGUI::Colour& getColour() const;

		void setAlphaSupport(bool _value);
		bool getAlphaSupport() const;

		Event_PreviewColour eventPreviewColour;

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

	private:
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyScrollChangePositionAlpha(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyEditTextChange(MyGUI::EditBox* _sender);
		void notifyEditTextChangeAlpha(MyGUI::EditBox* _sender);
		void notifyMouseButtonClickOk(MyGUI::Widget* _sender);
		void notifyMouseButtonClickCancel(MyGUI::Widget* _sender);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);

		void updateFirst();

		void createTexture();
		void updateTexture(const MyGUI::Colour& _colour);
		void destroyTexture();

		void updateFromPoint(const MyGUI::IntPoint& _point);
		void updateFromColour(const MyGUI::Colour& _colour);

		MyGUI::Colour getSaturate(const MyGUI::Colour& _colour) const;

		float& byIndex(MyGUI::Colour& _colour, size_t _index);

		void updateAlphaSupport();

	private:
		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mColourRect, "widget_ColourRect");
		MyGUI::ImageBox* mColourRect;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mColourView, "widget_ColourView");
		MyGUI::Widget* mColourView;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mImageColourPicker, "image_Picker");
		MyGUI::ImageBox* mImageColourPicker;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditRed, "edit_Red");
		MyGUI::EditBox* mEditRed;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditGreen, "edit_Green");
		MyGUI::EditBox* mEditGreen;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditBlue, "edit_Blue");
		MyGUI::EditBox* mEditBlue;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mInputAlpha, "InputAlpha");
		MyGUI::EditBox* mInputAlpha;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mTextAlpha, "TextAlpha");
		MyGUI::TextBox* mTextAlpha;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mScrollRange, "scroll_Range");
		MyGUI::ScrollBar* mScrollRange;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mOk, "button_OK");
		MyGUI::Button* mOk;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mCancel, "button_Cancel");
		MyGUI::Button* mCancel;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mAlphaSliderBack, "AlphaSliderBack");
		MyGUI::Widget* mAlphaSliderBack;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mAlphaSliderPlace, "AlphaSliderPlace");
		MyGUI::Widget* mAlphaSliderPlace;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mAlphaSlider, "AlphaSlider");
		MyGUI::ScrollBar* mAlphaSlider;

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mBaseColour;

		std::vector<MyGUI::Colour> mColourRange;

		MyGUI::ITexture* mTexture;
		MyGUI::UString mTextureName;
		bool mAlphaSupport;
	};

} // namespace tools

#endif // __COLOUR_PANEL_H__

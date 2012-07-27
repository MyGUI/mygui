/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/

#ifndef _09652fa5_fee6_4cc1_9edb_d68f76fe31e6_
#define _09652fa5_fee6_4cc1_9edb_d68f76fe31e6_

#include <MyGUI.h>
#include "Dialog.h"
#include "Control.h"

namespace tools
{

	ATTRIBUTE_CLASS_LAYOUT(ColourPanel, "ColourPanel.layout");
	class MYGUI_EXPORT_DLL ColourPanel :
		public Dialog,
		public Control,
		public sigslot::has_slots<>
	{
	public:
		ColourPanel();
		virtual ~ColourPanel();

		void setColour(const MyGUI::Colour& _colour);
		const MyGUI::Colour& getColour() const;

		void setAlphaSupport(bool _value);
		bool getAlphaSupport() const;

		sigslot::signal1<const MyGUI::Colour&> eventPreviewColour;

	protected:
		virtual void onDoModal();
		virtual void onEndModal();

		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		bool checkCommand();
		void commandColorAccept(const MyGUI::UString& _commandName, bool& _result);
		void commandColorCancel(const MyGUI::UString& _commandName, bool& _result);

		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyScrollChangePositionAlpha(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyEditTextChange(MyGUI::EditBox* _sender);
		void notifyEditTextChangeAlpha(MyGUI::EditBox* _sender);

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

}

#endif

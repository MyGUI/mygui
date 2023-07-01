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
	class MYGUI_EXPORT_DLL ColourPanel : public Dialog, public Control, public sigslot::has_slots<>
	{
	public:
		~ColourPanel() override;

		void setColour(const MyGUI::Colour& _colour);
		const MyGUI::Colour& getColour() const;

		void setAlphaSupport(bool _value);
		bool getAlphaSupport() const;

		sigslot::signal1<const MyGUI::Colour&> eventPreviewColour;

	protected:
		void onDoModal() override;
		void onEndModal() override;

		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

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
		MyGUI::ImageBox* mColourRect{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mColourView, "widget_ColourView");
		MyGUI::Widget* mColourView{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mImageColourPicker, "image_Picker");
		MyGUI::ImageBox* mImageColourPicker{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditRed, "edit_Red");
		MyGUI::EditBox* mEditRed{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditGreen, "edit_Green");
		MyGUI::EditBox* mEditGreen{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mEditBlue, "edit_Blue");
		MyGUI::EditBox* mEditBlue{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mInputAlpha, "InputAlpha");
		MyGUI::EditBox* mInputAlpha{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mTextAlpha, "TextAlpha");
		MyGUI::TextBox* mTextAlpha{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mScrollRange, "scroll_Range");
		MyGUI::ScrollBar* mScrollRange{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mAlphaSliderBack, "AlphaSliderBack");
		MyGUI::Widget* mAlphaSliderBack{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mAlphaSliderPlace, "AlphaSliderPlace");
		MyGUI::Widget* mAlphaSliderPlace{nullptr};

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mAlphaSlider, "AlphaSlider");
		MyGUI::ScrollBar* mAlphaSlider{nullptr};

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mBaseColour;

		std::vector<MyGUI::Colour> mColourRange;

		MyGUI::ITexture* mTexture{nullptr};
		MyGUI::UString mTextureName;
		bool mAlphaSupport{true};
	};

}

#endif

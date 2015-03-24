/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
#ifndef COLOUR_PANEL_H_
#define COLOUR_PANEL_H_

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
		const MyGUI::Colour& getColour() const;

		MyGUI::delegates::CDelegate1<ColourPanel*> eventColourAccept;

	private:
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position);
		void notifyEditTextChange(MyGUI::EditBox* _sender);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void updateFirst();

		void createTexture();
		void updateTexture(const MyGUI::Colour& _colour);
		void destroyTexture();

		void updateFromPoint(const MyGUI::IntPoint& _point);
		void updateFromColour(const MyGUI::Colour& _colour);

		MyGUI::Colour getSaturate(const MyGUI::Colour& _colour) const;

		float& byIndex(MyGUI::Colour& _colour, size_t _index);

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

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mScrollRange, "scroll_Range");
		MyGUI::ScrollBar* mScrollRange;

		ATTRIBUTE_FIELD_WIDGET_NAME(ColourPanel, mOk, "button_OK");
		MyGUI::Button* mOk;

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mBaseColour;

		std::vector<MyGUI::Colour> mColourRange;

		MyGUI::ITexture* mTexture;
	};

} // namespace demo

#endif // COLOUR_PANEL_H_

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

	class ColourPanel : public wraps::BaseLayout, public MyGUI::IManualResourceLoader
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

		virtual void loadResource(MyGUI::IRenderResource* _resource);

	private:
		MyGUI::StaticImagePtr mColourRect;
		MyGUI::WidgetPtr mColourView;
		MyGUI::StaticImagePtr mImageColourPicker;
		MyGUI::EditPtr mEditRed;
		MyGUI::EditPtr mEditGreen;
		MyGUI::EditPtr mEditBlue;
		MyGUI::VScrollPtr mScrollRange;
		MyGUI::ButtonPtr mOk;

		MyGUI::Colour mCurrentColour;
		MyGUI::Colour mBaseColour;

		MyGUI::RawRect * mRawColourView;

		std::vector<MyGUI::Colour> mColourRange;

		MyGUI::ITexture* mTexture;
	};

} // namespace demo

#endif // __COLOUR_PANEL_H__

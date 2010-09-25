/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXTURE_CONTROL_H__
#define __TEXTURE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "SelectorControl.h"
#include "Dialog.h"

namespace tools
{

	class TextureControl :
		public wraps::BaseLayout
	{
	public:
		TextureControl(const std::string& _layout, MyGUI::Widget* _parent);
		virtual ~TextureControl();

	protected:
		void setTextureName(const MyGUI::UString& _value);
		const MyGUI::IntSize& getTextureSize();

		void setTextureRegion(const MyGUI::IntCoord& _value);
		const MyGUI::IntCoord& getTextureRegion();

		template<typename T>
		void addSelectorControl(T * & _control)
		{
			assignBase(_control, "Texture");
			registerSelectorControl(_control);
		}

		void setColour(MyGUI::Colour _value);
		MyGUI::Colour getColour();

		void setScale(double _value);

		virtual void onMouseButtonClick(const MyGUI::IntPoint& _point);

		bool getSelectorsCapture();

		virtual void onMouseWheel(int _rel);

		void saveMouseRelative();
		void loadMouseRelative();

	private:
		void notifyChangePosition();
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseWheel(MyGUI::Widget* _sender, int _rel);

		void updateTexture();
		void updateCoord();

		void updateScale();

		void updateRegionCoord();
		void updateColours();

		void registerSelectorControl(SelectorControl* _control);

	private:
		MyGUI::ScrollView* mView;
		MyGUI::StaticImage* mTexture;
		MyGUI::Widget* mBackground;

		double mScaleValue;
		MyGUI::IntSize mTextureSize;
		MyGUI::IntCoord mTextureRegion;
		MyGUI::Colour mCurrentColour;

		std::vector<SelectorControl*> mSelectors;

		MyGUI::IntPoint mRightMouseClick;
		MyGUI::IntPoint mViewOffset;
		bool mMouseCapture;

		MyGUI::FloatPoint mMouseRelative;
		bool mMouseLeftPressed;
	};

} // namespace tools

#endif // __TEXTURE_CONTROL_H__

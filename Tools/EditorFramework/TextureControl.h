/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _989b726c_13e5_4bc9_853d_4630f7b925dc_
#define _989b726c_13e5_4bc9_853d_4630f7b925dc_

#include "Control.h"
#include "SelectorControl.h"
#include "Dialog.h"

namespace tools
{

	class MYGUI_EXPORT_DLL TextureControl :
		public Control
	{
	public:
		TextureControl();
		virtual ~TextureControl();

		void setTextureValue(const MyGUI::UString& _value);

		void setTextureRegion(const MyGUI::IntCoord& _value);
		void resetTextureRegion();
		const MyGUI::IntCoord& getTextureRegion() const;

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

		const MyGUI::IntSize& getTextureSize() const;

		template<typename T>
		void addSelectorControl(T * & _control)
		{
			_control = new T();
			_control->Initialise(this, mTexture, "");
			registerSelectorControl(_control);
		}

		void addSelectorControl(SelectorControl* _control)
		{
			_control->Initialise(this, mTexture, "");
			registerSelectorControl(_control);
		}

		void removeSelectorControl(SelectorControl* _control);

		void setColour(MyGUI::Colour _value);
		MyGUI::Colour getColour() const;

		void setScale(double _value);
		double getScale() const;

		virtual void onChangeScale();

		virtual void onMouseMove();
		virtual void onMouseWheel(int _rel);
		virtual void onMouseButtonPressed(const MyGUI::IntPoint& _point);
		virtual void onMouseButtonReleased(const MyGUI::IntPoint& _point);
		virtual void onMouseButtonClick(const MyGUI::IntPoint& _point);
		virtual void onMouseDrag(const MyGUI::IntPoint& _point);

		bool getSelectorsCapture();

		void saveMouseRelative();
		void loadMouseRelative();

	private:
		void notifyChangePosition();
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseMove(MyGUI::Widget* _sender, int _left, int _top);
		void notifyMouseWheel(MyGUI::Widget* _sender, int _rel);

		void updateTexture();
		void updateCoord();

		void updateScale();

		void updateRegionCoord();
		void updateColours();

		void registerSelectorControl(SelectorControl* _control);

		MyGUI::IntPoint getMousePosition();

	private:
		MyGUI::ScrollView* mView;
		MyGUI::ImageBox* mTexture;
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

}

#endif

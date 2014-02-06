/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _ddcea3ee_9cb6_479c_94b5_4fadfc82b564_
#define _ddcea3ee_9cb6_479c_94b5_4fadfc82b564_

#include "BaseLayout/BaseLayout.h"
#include "SelectorControl.h"
#include "Dialog.h"

namespace tools
{

	class TextureControlLE :
		public wraps::BaseLayout
	{
	public:
		TextureControlLE(const std::string& _layout, MyGUI::Widget* _parent);
		virtual ~TextureControlLE();

	protected:
		void setTextureName(const MyGUI::UString& _value);
		const MyGUI::IntSize& getTextureSize() const;

		void setTextureRegion(const MyGUI::IntCoord& _value);
		const MyGUI::IntCoord& getTextureRegion() const;

		template<typename T>
		void addSelectorControl(T * & _control)
		{
			assignBase(_control, "Texture");
			registerSelectorControl(_control);
		}

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

		void registerSelectorControl(SelectorControlLE* _control);

		MyGUI::IntPoint getMousePosition();

	private:
		MyGUI::ScrollView* mView;
		MyGUI::ImageBox* mTexture;
		MyGUI::Widget* mBackground;

		double mScaleValue;
		MyGUI::IntSize mTextureSize;
		MyGUI::IntCoord mTextureRegion;
		MyGUI::Colour mCurrentColour;

		std::vector<SelectorControlLE*> mSelectors;

		MyGUI::IntPoint mRightMouseClick;
		MyGUI::IntPoint mViewOffset;
		bool mMouseCapture;

		MyGUI::FloatPoint mMouseRelative;
		bool mMouseLeftPressed;
	};

}

#endif

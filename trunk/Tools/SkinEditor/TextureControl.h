/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXTURE_CONTROL_H__
#define __TEXTURE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "AreaSelectorControl.h"
#include "PropertyAdvisor.h"
#include "SelectorControl.h"
#include "ColourPanel.h"

namespace tools
{

	class TextureControl :
		public wraps::BaseLayout
	{
	public:
		TextureControl(MyGUI::Widget* _parent);
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
			mSelectors.push_back(_control);

			_control->setScale(mScaleValue);
		}

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void notifyChangePosition();
		void notifyEndDialog(wraps::BaseLayout* _sender, bool _result);

		void updateTexture();
		void updateCoord();

		void fillColours(MyGUI::ComboBox* _combo);
		void updateColour(MyGUI::ComboBox* _sender);

		void fillScale();
		void updateScale();

		void updateRegionCoord();
		void updateColours();

	private:
		MyGUI::ScrollView* mView;
		MyGUI::StaticImage* mTexture;
		MyGUI::ComboBox* mBackgroundColour;
		MyGUI::Widget* mBackground;
		MyGUI::ComboBox* mScale;
		MyGUI::Widget* mBackgroundButton;

		demo::ColourPanel* mColourPanel;

		double mScaleValue;
		MyGUI::IntSize mTextureSize;
		MyGUI::IntCoord mTextureRegion;
		MyGUI::Colour mCurrentColour;

		std::vector<SelectorControl*> mSelectors;

		bool mRightMousePressed;
	};

} // namespace tools

#endif // __TEXTURE_CONTROL_H__

/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_TEXTURE_CONTROL_H__
#define __STATE_TEXTURE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "RegionSelectorControl2.h"

namespace tools
{

	class StateTextureControl :
		public wraps::BaseLayout
	{
	public:
		StateTextureControl(MyGUI::Widget* _parent);
		virtual ~StateTextureControl();

	private:
		void notifyChangeSelection();
		void notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner);
		void notifyChangePropertyState(Property* _sender, const MyGUI::UString& _owner);
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		void notifyChangePosition();

		void unadvice();
		void advice();

		void updateAllProperties();
		void updateTexture();
		void updateCoord();

		void updateAllStateProperties();
		void updateVisible();
		void updatePosition();

		void fillColours(MyGUI::ComboBox* _combo);
		void updateColour(MyGUI::ComboBox* _sender);

		void fillScale();
		void updateScale();

		void updateRegionCoord();

		void notifyStateChangeSelection();
		void adviceState();
		void unadviceState();

	private:
		MyGUI::ScrollView* mView;
		MyGUI::StaticImage* mTexture;
		MyGUI::ComboBox* mBackgroundColour;
		MyGUI::Widget* mBackground;
		MyGUI::ComboBox* mScale;

		RegionSelectorControl2* mRegionSelectorControl;

		SkinItem* mCurrentSkin;
		StateItem* mCurrentState;
		MyGUI::UString mTypeName;
		double mScaleValue;
		MyGUI::IntSize mTextureSize;
		MyGUI::IntCoord mCoordValue;

	};

} // namespace tools

#endif // __STATE_TEXTURE_CONTROL_H__

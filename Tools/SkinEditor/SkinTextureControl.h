/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_TEXTURE_CONTROL_H__
#define __SKIN_TEXTURE_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "RegionSelectorControl.h"

namespace tools
{

	class SkinTextureControl :
		public wraps::BaseLayout
	{
	public:
		SkinTextureControl(MyGUI::Widget* _parent);
		virtual ~SkinTextureControl();

	private:
		void notifyChangeSelection();
		void notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner);
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		void notifyChangePosition();

		void unadvice();
		void advice();

		void updateAllProperties();
		void updateTexture();
		void updateCoord();

		void fillColours(MyGUI::ComboBox* _combo);
		void updateColour(MyGUI::ComboBox* _sender);

		void fillScale();
		void updateScale();

		void updateRegionCoord();

	private:
		MyGUI::ScrollView* mView;
		MyGUI::StaticImage* mTexture;
		MyGUI::ComboBox* mBackgroundColour;
		MyGUI::Widget* mBackground;
		MyGUI::ComboBox* mScale;

		RegionSelectorControl* mRegionSelectorControl;

		SkinItem* mCurrentSkin;
		MyGUI::UString mTypeName;
		double mScaleValue;
		MyGUI::IntSize mTextureSize;
		MyGUI::IntCoord mCoordValue;

	};

} // namespace tools

#endif // __SKIN_TEXTURE_CONTROL_H__

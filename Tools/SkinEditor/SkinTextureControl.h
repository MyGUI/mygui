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
#include "PropertyAdvisor.h"

namespace tools
{

	class SkinTextureControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		SkinTextureControl(MyGUI::Widget* _parent);
		virtual ~SkinTextureControl();

	private:
		void notifyChangeSelection();
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSkinProperties();

		void notifyChangePosition();

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

		MyGUI::UString mTypeName;
		double mScaleValue;
		MyGUI::IntSize mTextureSize;
		MyGUI::IntCoord mCoordValue;

	};

} // namespace tools

#endif // __SKIN_TEXTURE_CONTROL_H__

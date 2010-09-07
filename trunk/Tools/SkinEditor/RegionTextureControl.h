/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __REGION_TEXTURE_CONTROL_H__
#define __REGION_TEXTURE_CONTROL_H__

#include "TextureToolControl.h"
#include "PropertyAdvisor.h"
#include "AreaSelectorControl.h"
#include "PositionSelectorControl.h"
#include "PositionSelectorBlackControl.h"

namespace tools
{
	class RegionTextureControl :
		public TextureToolControl,
		public PropertyAdvisor
	{
	public:
		RegionTextureControl(MyGUI::Widget* _parent);
		virtual ~RegionTextureControl();

	protected:
		virtual void updateSkinProperties();
		virtual void updateStateProperties();
		virtual void updateRegionProperties();
		virtual void updateSeparatorProperties();

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateRegionProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSeparatorProperty(Property* _sender, const MyGUI::UString& _owner);

	private:
		void notifyChangePosition();

		void updateTextureControl();
		void updateTextureName();
		void updateRegionSize();
		void updateRegionPosition();
		void updateTextureVisible();

		void updatePosition();
		void updateVisible();

		void updateUnselectedStates();
		void addCoord(std::vector<MyGUI::IntCoord>& _coords, const MyGUI::UString& _coord);
		void drawUnselectedStates(std::vector<MyGUI::IntCoord>& _coords);

	private:
		MyGUI::UString mTextureName;
		MyGUI::IntCoord mTextureRegion;
		bool mTextureVisible;
		MyGUI::UString mTypeName;

		AreaSelectorControl* mAreaSelectorControl;
		PositionSelectorControl* mPositionSelectorControl;

		std::vector<PositionSelectorBlackControl*> mBlackSelectors;
	};

} // namespace tools

#endif // __REGION_TEXTURE_CONTROL_H__

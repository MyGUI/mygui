/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_TEXTURE_CONTROL_H__
#define __SKIN_TEXTURE_CONTROL_H__

#include "TextureToolControl.h"
#include "Property.h"
#include "SkinItem.h"
#include "AreaSelectorControl.h"
#include "PropertyAdvisor.h"

namespace tools
{
	class SkinTextureControl :
		public TextureToolControl,
		public PropertyAdvisor
	{
	public:
		SkinTextureControl(MyGUI::Widget* _parent);
		virtual ~SkinTextureControl();

	protected:
		virtual void onMouseButtonClick(const MyGUI::IntPoint& _point);

	private:
		void notifyChangeSelection();
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateSkinProperties();

		void notifyChangePosition();

		void updateTexture();
		void updateCoord();

		void updateRegionCoord();

	private:
		AreaSelectorControl* mAreaSelectorControl;

		MyGUI::UString mTypeName;
		MyGUI::IntCoord mCoordValue;
	};

} // namespace tools

#endif // __SKIN_TEXTURE_CONTROL_H__

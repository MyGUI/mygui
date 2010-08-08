/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __STATE_TEXTURE_CONTROL_H__
#define __STATE_TEXTURE_CONTROL_H__

#include "TextureControl.h"
#include "Property.h"
#include "SkinItem.h"
#include "RegionSelectorControl2.h"
#include "PropertyAdvisor.h"

namespace tools
{

	class StateTextureControl :
		public TextureControl,
		public PropertyAdvisor
	{
	public:
		StateTextureControl(MyGUI::Widget* _parent);
		virtual ~StateTextureControl();

	private:
		void notifyChangePosition();
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);

		virtual void updateSkinProperties();
		virtual void updateStateProperties();

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);

		void updateTexture();
		void updateCoord();

		void updateVisible();
		void updatePosition();

		void updateRegionCoord();

	private:
		RegionSelectorControl2* mRegionSelectorControl;

		MyGUI::UString mTypeName;
		MyGUI::IntCoord mCoordValue;
	};

} // namespace tools

#endif // __STATE_TEXTURE_CONTROL_H__

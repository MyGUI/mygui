/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_PROPERTY_CONTROL_H__
#define __SKIN_PROPERTY_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "SkinItem.h"
#include "PropertyAdvisor.h"

namespace tools
{

	class SkinPropertyControl :
		public wraps::BaseLayout,
		public PropertyAdvisor
	{
	public:
		SkinPropertyControl(MyGUI::Widget* _parent);
		virtual ~SkinPropertyControl();

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyEditTextChange(MyGUI::Edit* _sender);

		virtual void updateSkinProperties();
		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);

		void fillTextures();

		void updateTexture();
		void updateCoord();

		size_t getComboIndex(const MyGUI::UString& _name);

		bool isCoordValidate();
		MyGUI::UString getClearCoordValue();
		void setColourCoord(bool _validate);

	private:
		MyGUI::ComboBox* mTextures;
		MyGUI::Edit* mCoord;
		MyGUI::UString mTypeName;
	};

} // namespace tools

#endif // __SKIN_PROPERTY_CONTROL_H__

/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_TEXTURE_CONTROL_H__
#define __SEPARATOR_TEXTURE_CONTROL_H__

#include "TextureControl.h"
#include "PropertyAdvisor.h"
#include "HorizontalSelectorControl.h"
#include "VerticalSelectorControl.h"

namespace tools
{

	class SeparatorTextureControl :
		public TextureControl,
		public PropertyAdvisor
	{
	public:
		SeparatorTextureControl(MyGUI::Widget* _parent);
		virtual ~SeparatorTextureControl();

	protected:
		virtual void updateSkinProperties();
		virtual void updateStateProperties();
		virtual void updateSeparatorProperties();

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);
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

	private:
		MyGUI::UString mTextureName;
		MyGUI::IntCoord mTextureRegion;
		bool mTextureVisible;
		MyGUI::UString mTypeName;

		HorizontalSelectorControl* mHorizontalSelectorControl;
		VerticalSelectorControl* mVerticalSelectorControl;
	};

} // namespace tools

#endif // __SEPARATOR_TEXTURE_CONTROL_H__

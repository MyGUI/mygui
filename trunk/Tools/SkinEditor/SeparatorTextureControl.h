/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SEPARATOR_TEXTURE_CONTROL_H__
#define __SEPARATOR_TEXTURE_CONTROL_H__

#include "TextureControl.h"
#include "PropertyAdvisor.h"

namespace tools
{

	class SeparatorTextureControl :
		public TextureControl,
		public PropertyAdvisor
	{
	public:
		SeparatorTextureControl(MyGUI::Widget* _parent);
		virtual ~SeparatorTextureControl();

	private:
		virtual void updateSkinProperties();
		virtual void updateStateProperties();

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner);
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner);

	private:
		void updateTextureControl();
		void updateTextureName();
		void updateRegionSize();
		void updateRegionPosition();

	private:
		MyGUI::UString mTextureName;
		MyGUI::IntCoord mTextureRegion;
	};

} // namespace tools

#endif // __SEPARATOR_TEXTURE_CONTROL_H__

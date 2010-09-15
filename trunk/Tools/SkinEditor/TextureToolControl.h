/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXTURE_TOOL_CONTROL_H__
#define __TEXTURE_TOOL_CONTROL_H__

#include "TextureControl.h"

namespace tools
{
	class TextureToolControl :
		public TextureControl
	{
	public:
		TextureToolControl(MyGUI::Widget* _parent);
		virtual ~TextureToolControl();

	private:
		void notifyComboChangePositionScale(MyGUI::ComboBox* _sender, size_t _index);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		void fillScale();

		void updateScale(MyGUI::ComboBox* _sender);

	private:
		MyGUI::ComboBox* mScale;
	};

} // namespace tools

#endif // __TEXTURE_TOOL_CONTROL_H__

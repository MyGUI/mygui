/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXTURE_TOOL_CONTROL_H__
#define __TEXTURE_TOOL_CONTROL_H__

#include "TextureControl.h"
#include "ColourPanel.h"

namespace tools
{

	class TextureToolControl :
		public TextureControl
	{
	public:
		TextureToolControl(MyGUI::Widget* _parent);
		virtual ~TextureToolControl();

	private:
		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void fillColours(MyGUI::ComboBox* _combo);
		void fillScale();

		void updateColour(MyGUI::ComboBox* _sender);

	private:
		MyGUI::ComboBox* mBackgroundColour;
		MyGUI::ComboBox* mScale;
		MyGUI::Widget* mBackgroundButton;

		ColourPanel* mColourPanel;
	};

} // namespace tools

#endif // __TEXTURE_TOOL_CONTROL_H__

/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __COLOUR_MANAGER_H__
#define __COLOUR_MANAGER_H__

#include <MyGUI.h>
#include "ColourPanel.h"

namespace tools
{
	class ColourManager :
		public MyGUI::Singleton<ColourManager>
	{
	public:
		ColourManager();
		virtual ~ColourManager();

		void initialise();
		void shutdown();

	private:
		void commandChangeColourBackground(const MyGUI::UString& _commandName, bool& _result);
		void commandChangeColourSelector(const MyGUI::UString& _commandName, bool& _result);
		void commandChangeColourSelectorInactive(const MyGUI::UString& _commandName, bool& _result);

		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyPreviewColour(const MyGUI::Colour& _value);

		void showColourDialog();

		void setColour(const MyGUI::Colour& _color);
		MyGUI::Colour getColour();

	private:
		ColourPanel* mColourPanel;
		MyGUI::Colour mPreviewColour;
		std::string mCurrentColourType;
	};

} // namespace tools

#endif // __COLOUR_MANAGER_H__

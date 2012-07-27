/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _aece60c1_4ddf_414b_b272_9e0962156c24_
#define _aece60c1_4ddf_414b_b272_9e0962156c24_

#include <MyGUI.h>
#include "ColourPanel.h"
#include "sigslot.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ColourManager :
		public MyGUI::Singleton<ColourManager>,
		public sigslot::has_slots<>
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

}

#endif

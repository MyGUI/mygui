/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/

#ifndef _b8560ae2_b63d_469a_9007_4562b883e6e1_
#define _b8560ae2_b63d_469a_9007_4562b883e6e1_

#include "Control.h"

namespace tools
{

	class SettingsGeneralControl :
		public Control
	{
	public:
		SettingsGeneralControl();
		virtual ~SettingsGeneralControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
		virtual void OnCommand(const std::string& _command);

	private:
		void loadSettings();
		void saveSettings();

		void notifyNewGridStep(MyGUI::Widget* _sender, MyGUI::Widget* _new = 0);
		void notifyNewGridStepAccept(MyGUI::EditBox* _sender);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		int getGridStep();
		void setGridStep();
		void setLanguageValue(const std::string& _value);
		std::string getLanguageValue();

	private:
		int mGridStep;

		MyGUI::EditBox* mGridEdit;
		MyGUI::Button* mLoadLastProject;
		MyGUI::ComboBox* mInterfaceLanguage;
		MyGUI::EditBox* mWorkspaceSize;
	};

}

#endif

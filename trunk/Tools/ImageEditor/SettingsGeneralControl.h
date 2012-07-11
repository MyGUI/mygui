/*!
	@file
	@author		Albert Semenov
	@date		09/2010
*/
#ifndef __SETTINGS_GENERAL_CONTROL_H__
#define __SETTINGS_GENERAL_CONTROL_H__

#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class SettingsGeneralControl :
		public wraps::BaseLayout
	{
	public:
		SettingsGeneralControl(MyGUI::Widget* _parent = nullptr);
		virtual ~SettingsGeneralControl();

		void loadSettings();
		void saveSettings();

	private:
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
		MyGUI::Button* mSaveLastTexture;
		MyGUI::ComboBox* mInterfaceLanguage;
	};

} // namespace tools

#endif // __SETTINGS_GENERAL_CONTROL_H__

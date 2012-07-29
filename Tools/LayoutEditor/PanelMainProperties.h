/*!
	@file
	@author		Georgiy Evmenov
	@date		09/2008
*/

#ifndef _bdc81da4_7bc6_4966_9409_cdb3d2011d75_
#define _bdc81da4_7bc6_4966_9409_cdb3d2011d75_

#include "BaseLayout/BaseLayout.h"
#include "PanelView/BasePanelViewItem.h"
#include "IPropertyField.h"
#include "WidgetTypes.h"
#include "EditorWidgets.h"

namespace tools
{

	class PanelMainProperties :
		public wraps::BasePanelViewItem
	{
	public:
		PanelMainProperties();

		virtual void initialise();
		virtual void shutdown();

		void update(MyGUI::Widget* _currentWidget);

	private:
		void notifyAction(const std::string& _name, const std::string& _value, bool _final);
		void notifyActionSkin(const std::string& _type, const std::string& _value, bool _final);
		void notifyActionLayer(const std::string& _type, const std::string& _value, bool _final);
		void notifyActionName(const std::string& _type, const std::string& _value, bool _final);
		void notifyActionType(const std::string& _type, const std::string& _value, bool _final);
		void notifyActionAlign(const std::string& _type, const std::string& _value, bool _final);
		void notifyActionTemplate(const std::string& _type, const std::string& _value, bool _final);

		void destroyPropertyFields();
		void hidePropertyFields();
		void updateSize();

		bool isSkinExist(const std::string& _skinName);
		bool checkTemplate(const std::string& _skinName);

		IPropertyField* getPropertyField(MyGUI::Widget* _client, const std::string& _name, const std::string& _type);

	private:
		MyGUI::Widget* mCurrentWidget;
		typedef std::map<std::string, IPropertyField*> MapPropertyField;
		MapPropertyField mFields;
		std::string mUserDataTargetType;
	};

}

#endif

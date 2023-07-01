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

	class PanelMainProperties : public wraps::BasePanelViewItem
	{
	public:
		PanelMainProperties();

		void initialise() override;
		void shutdown() override;

		void update(MyGUI::Widget* _currentWidget);

	private:
		void notifyAction(std::string_view _name, std::string_view _value, bool _final);
		void notifyActionSkin(std::string_view _type, std::string_view _value, bool _final);
		void notifyActionLayer(std::string_view _type, std::string_view _value, bool _final);
		void notifyActionName(std::string_view _type, std::string_view _value, bool _final);
		void notifyActionType(std::string_view _type, std::string_view _value, bool _final);
		void notifyActionAlign(std::string_view _type, std::string_view _value, bool _final);
		void notifyActionTemplate(std::string_view _type, std::string_view _value, bool _final);

		void destroyPropertyFields();
		void hidePropertyFields();
		void updateSize();

		bool isSkinExist(std::string_view _skinName);
		bool checkTemplate(std::string_view _skinName);

		IPropertyField* getPropertyField(MyGUI::Widget* _client, std::string_view _name, std::string_view _type);

	private:
		MyGUI::Widget* mCurrentWidget{nullptr};
		using MapPropertyField = std::map<std::string, IPropertyField*, std::less<>>;
		MapPropertyField mFields;
		std::string mUserDataTargetType;
	};

}

#endif

#ifndef __WIDGET_CONTAINER_H__
#define __WIDGET_CONTAINER_H__

#include "MyGUI.h"

namespace tools
{
	const std::string DEFAULT_EDITOR_LAYER = "LayoutEditor_Widgets";
	const std::string DEFAULT_LAYER = "Back";

	struct ControllerInfo
	{
		std::string mType;
		MyGUI::MapString mProperty;
	};

	struct WidgetContainer
	{
		WidgetContainer();
		WidgetContainer(const std::string& _type, const std::string& _skin, MyGUI::Widget* _widget, const std::string& _name = "");

		~WidgetContainer();

		// not for saving
		std::string position(bool _percent = true);

		void setUserData(const std::string& _key, const std::string& _value);
		std::string getUserData(const std::string& _key);
		void clearUserData(const std::string& _key);
		bool existUserData(const std::string& _key) const;
		typedef MyGUI::Enumerator<MyGUI::VectorStringPairs> UserDataEnumerator;
		UserDataEnumerator getUserDataEnumerator();

		void setLayerName(const std::string& _value);
		const std::string& getLayerName() const;

		MyGUI::Widget* getWidget();
		void setWidget(MyGUI::Widget* _value);

		const std::string& getName() const;
		void setName(const std::string& _value);

		const std::string& getSkin() const;
		void setSkin(const std::string& _value);

		const std::string& getType() const;
		void setType(const std::string& _value);

		const std::string& getAlign() const;
		void setAlign(const std::string& _value);

		const std::string& getStyle() const;
		void setStyle(const std::string& _value);

		bool getRelativeMode() const;
		void setRelativeMode(bool _value);

	public:
		std::vector<WidgetContainer*> childContainers;
		std::vector<ControllerInfo*> mController;
		MyGUI::VectorStringPairs mProperty;

	private:
		MyGUI::VectorStringPairs mUserString;
		bool relative_mode;
		std::string style;
		std::string align;
		std::string type;
		std::string skin;
		std::string name;
		std::string layer;
		MyGUI::Widget* widget;
	};

} // namespace tools

#endif // __WIDGET_CONTAINER_H__

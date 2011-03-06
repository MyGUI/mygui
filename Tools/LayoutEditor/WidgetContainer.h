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

		void setLayerName(const std::string& _layerName);

		const std::string& getLayerName() const;

	public:
		MyGUI::Widget* widget;
		std::vector<WidgetContainer*> childContainers;
		MyGUI::VectorStringPairs mProperty;
		std::vector<ControllerInfo*> mController;
		std::string type;
		std::string skin;
		std::string align;
		std::string name;
		std::string style;
		bool relative_mode;
		MyGUI::VectorStringPairs mUserString;

	private:
		std::string layer;
	};

} // namespace tools

#endif // __WIDGET_CONTAINER_H__

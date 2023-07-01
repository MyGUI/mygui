#ifndef _e44ffa34_9013_4f29_805c_ab84ec753b04_
#define _e44ffa34_9013_4f29_805c_ab84ec753b04_

#include "MyGUI.h"

namespace tools
{

	const std::string_view DEFAULT_EDITOR_LAYER = "LayoutEditor_Widgets";
	const std::string_view DEFAULT_TEST_MODE_LAYER = "Overlapped";

	struct ControllerInfo
	{
		std::string mType;
		MyGUI::MapString mProperty;
	};

	struct WidgetContainer
	{
		WidgetContainer();
		WidgetContainer(
			std::string_view _type,
			std::string_view _skin,
			MyGUI::Widget* _widget,
			std::string_view _name = {});

		~WidgetContainer();

		// not for saving
		std::string position(bool _percent = true);

		void setUserData(std::string_view _key, std::string_view _value);
		std::string_view getUserData(std::string_view _key);
		void clearUserData(std::string_view _key);
		bool existUserData(std::string_view _key) const;
		using UserDataEnumerator = MyGUI::Enumerator<MyGUI::VectorStringPairs>;
		UserDataEnumerator getUserDataEnumerator() const;

		void setProperty(std::string_view _key, std::string_view _value, bool _eraseExist = true);
		void setPropertyByIndex(size_t _index, std::string_view _key, std::string_view _value);
		void clearProperty(std::string_view _key);
		void clearPropertyByIndex(size_t _index, std::string_view _key);
		std::string_view getProperty(std::string_view _key);
		bool existProperty(std::string_view _key) const;
		using PropertyEnumerator = MyGUI::Enumerator<MyGUI::VectorStringPairs>;
		PropertyEnumerator getPropertyEnumerator() const;

		void setLayerName(std::string_view _value);
		const std::string& getLayerName() const;

		MyGUI::Widget* getWidget();
		void setWidget(MyGUI::Widget* _value);

		const std::string& getName() const;
		void setName(std::string_view _value);

		const std::string& getSkin() const;
		void setSkin(std::string_view _value);

		const std::string& getType() const;
		void setType(std::string_view _value);

		const std::string& getAlign() const;
		void setAlign(std::string_view _value);

		const std::string& getStyle() const;
		void setStyle(std::string_view _value);

		bool getRelativeMode() const;
		void setRelativeMode(bool _value);

	public:
		std::vector<WidgetContainer*> childContainers;
		std::vector<ControllerInfo*> mController;

	private:
		MyGUI::VectorStringPairs mUserString;
		MyGUI::VectorStringPairs mProperty;
		bool mRelativeMode;
		std::string mStyle;
		std::string mAlign;
		std::string mType;
		std::string mSkin;
		std::string mName;
		std::string mLayer;
		MyGUI::Widget* mWidget;
	};

}

#endif

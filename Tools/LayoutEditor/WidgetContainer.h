#ifndef _e44ffa34_9013_4f29_805c_ab84ec753b04_
#define _e44ffa34_9013_4f29_805c_ab84ec753b04_

#include "MyGUI.h"

namespace tools
{

	const std::string DEFAULT_EDITOR_LAYER = "LayoutEditor_Widgets";
	const std::string DEFAULT_TEST_MODE_LAYER = "Overlapped";

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
		UserDataEnumerator getUserDataEnumerator() const;

		void setProperty(const std::string& _key, const std::string& _value, bool _eraseExist = true);
		void setPropertyByIndex(size_t _index, const std::string& _key, const std::string& _value);
		void clearProperty(const std::string& _key);
		void clearPropertyByIndex(size_t _index, const std::string& _key);
		std::string getProperty(const std::string& _key);
		bool existProperty(const std::string& _key) const;
		typedef MyGUI::Enumerator<MyGUI::VectorStringPairs> PropertyEnumerator;
		PropertyEnumerator getPropertyEnumerator() const;

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

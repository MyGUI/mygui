#ifndef _2430e622_7ed5_481e_91df_1a6805bc9e49_
#define _2430e622_7ed5_481e_91df_1a6805bc9e49_

#include <sstream>
#include "WidgetContainer.h"

namespace tools
{

	using Event_ChangeWidgets = MyGUI::delegates::MultiDelegate<>;
	using Event_ChangeWidgetCoord =
		MyGUI::delegates::MultiDelegate<MyGUI::Widget*, const MyGUI::IntCoord&, std::string_view>;

	using VectorWidgetContainer = std::vector<WidgetContainer*>;
	using EnumeratorWidgetContainer = MyGUI::Enumerator<VectorWidgetContainer>;

	class EditorWidgets : public MyGUI::IUnlinkWidget
	{
		MYGUI_SINGLETON_DECLARATION(EditorWidgets);

	public:
		EditorWidgets();

		void initialise();
		void shutdown();

		bool load(const MyGUI::UString& _fileName);
		bool save(const MyGUI::UString& _fileName);
		void clear();

		void loadxmlDocument(MyGUI::xml::Document* doc, bool _testMode = false);
		MyGUI::xml::Document* savexmlDocument();
		WidgetContainer* find(MyGUI::Widget* _widget);
		WidgetContainer* find(std::string_view _name);
		void add(WidgetContainer* _container);
		void remove(MyGUI::Widget* _widget);
		void remove(WidgetContainer* _container);

		bool tryToApplyProperty(
			MyGUI::Widget* _widget,
			std::string_view _key,
			std::string_view _value,
			bool _testMode = false);
		void onSetWidgetCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _value, std::string_view _owner) const;

		void invalidateWidgets();
		EnumeratorWidgetContainer getWidgets();

		std::string_view getSkinReplace(std::string_view _skinName);

		const MyGUI::UString& getCurrentFileName() const;
		const MyGUI::UString& getCurrentItemName() const;

		Event_ChangeWidgets eventChangeWidgets;
		Event_ChangeWidgetCoord eventChangeWidgetCoord;

		void _unlinkWidget(MyGUI::Widget* _widget) override;

		MyGUI::MapString& getCodeGeneratorSettings();

	private:
		WidgetContainer* _find(MyGUI::Widget* _widget, std::string_view _name, std::vector<WidgetContainer*> _widgets);

		void parseWidget(MyGUI::xml::ElementEnumerator& _widget, MyGUI::Widget* _parent, bool _testMode = false);
		void serialiseWidget(WidgetContainer* _container, MyGUI::xml::ElementPtr _node, bool _compatibility = false);

		void loadIgnoreParameters(MyGUI::xml::ElementPtr _node, std::string_view _file, MyGUI::Version _version);
		void loadSkinReplace(MyGUI::xml::ElementPtr _node, std::string_view _file, MyGUI::Version _version);

		void notifyFrameStarted(float _time);

		void destroyAllWidgets();

		bool isSkinExist(std::string_view _skinName);
		bool checkTemplate(std::string_view _skinName);

		bool loadFromProject(const MyGUI::UString& _fileName, size_t _index);
		bool saveToProject(const MyGUI::UString& _fileName, size_t _index);

		bool unbind(WidgetContainer* _container);

		void loadWidgetsFromXmlNode(MyGUI::xml::ElementPtr _root, bool _testMode = false);
		void saveWidgetsToXmlNode(MyGUI::xml::ElementPtr _root, bool _compatibility = false);

		void loadCodeGeneratorSettings(MyGUI::xml::ElementPtr _sectorNode);
		void saveCodeGeneratorSettings(MyGUI::xml::ElementPtr _rootNode);

	private:
		bool mWidgetsChanged{false};
		using VectorString = std::vector<std::string>;
		VectorString mIgnoreParameters;
		VectorWidgetContainer mWidgets;

		MyGUI::MapString mSkinReplaces;

		MyGUI::UString mCurrentFileName;
		MyGUI::UString mCurrentItemName;

		MyGUI::MapString mCodeGeneratorSettings;
	};

}

#endif

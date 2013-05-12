#ifndef _2430e622_7ed5_481e_91df_1a6805bc9e49_
#define _2430e622_7ed5_481e_91df_1a6805bc9e49_

#include <sstream>
#include "WidgetContainer.h"

namespace tools
{

	typedef MyGUI::delegates::CMultiDelegate0 Event_ChangeWidgets;
	typedef MyGUI::delegates::CMultiDelegate3<MyGUI::Widget*, const MyGUI::IntCoord&, const std::string&> Event_ChangeWidgetCoord;

	typedef std::vector<WidgetContainer*> VectorWidgetContainer;
	typedef MyGUI::Enumerator<VectorWidgetContainer> EnumeratorWidgetContainer;

	class EditorWidgets :
		public MyGUI::Singleton<EditorWidgets>,
		public MyGUI::IUnlinkWidget
	{
	public:
		EditorWidgets();
		virtual ~EditorWidgets();

		void initialise();
		void shutdown();

		bool load(const MyGUI::UString& _fileName);
		bool save(const MyGUI::UString& _fileName);
		void clear();

		void loadxmlDocument(MyGUI::xml::Document* doc, bool _testMode = false);
		MyGUI::xml::Document* savexmlDocument();
		WidgetContainer* find(MyGUI::Widget* _widget);
		WidgetContainer* find(const std::string& _name);
		void add(WidgetContainer* _container);
		void remove(MyGUI::Widget* _widget);
		void remove(WidgetContainer* _container);

		bool tryToApplyProperty(MyGUI::Widget* _widget, const std::string& _key, const std::string& _value, bool _testMode = false);
		void onSetWidgetCoord(MyGUI::Widget* _widget, const MyGUI::IntCoord& _value, const std::string& _owner);

		void invalidateWidgets();
		EnumeratorWidgetContainer getWidgets();

		std::string getSkinReplace(const std::string& _skinName);

		const MyGUI::UString& getCurrentFileName() const;
		const MyGUI::UString& getCurrentItemName() const;

		Event_ChangeWidgets eventChangeWidgets;
		Event_ChangeWidgetCoord eventChangeWidgetCoord;

		virtual void _unlinkWidget(MyGUI::Widget* _widget);

		MyGUI::MapString& getCodeGeneratorSettings();

	private:
		WidgetContainer* _find(MyGUI::Widget* _widget, const std::string& _name, std::vector<WidgetContainer*> _widgets);

		void parseWidget(MyGUI::xml::ElementEnumerator& _widget, MyGUI::Widget* _parent, bool _testMode = false);
		void serialiseWidget(WidgetContainer* _container, MyGUI::xml::ElementPtr _node, bool _compatibility = false);

		void loadIgnoreParameters(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
		void loadSkinReplace(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

		void notifyFrameStarted(float _time);

		void destroyAllWidgets();

		bool isSkinExist(const std::string& _skinName);
		bool checkTemplate(const std::string& _skinName);

		bool loadFromProject(const MyGUI::UString& _fileName, size_t _index);
		bool saveToProject(const MyGUI::UString& _fileName, size_t _index);

		bool unbind(WidgetContainer* _container);

		void loadWidgetsFromXmlNode(MyGUI::xml::ElementPtr _root, bool _testMode = false);
		void saveWidgetsToXmlNode(MyGUI::xml::ElementPtr _root, bool _compatibility = false);

		void loadCodeGeneratorSettings(MyGUI::xml::ElementPtr _sectorNode);
		void saveCodeGeneratorSettings(MyGUI::xml::ElementPtr _rootNode);

	private:
		bool mWidgetsChanged;
		typedef std::vector<std::string> VectorString;
		VectorString mIgnoreParameters;
		VectorWidgetContainer mWidgets;

		MyGUI::MapString mSkinReplaces;

		MyGUI::UString mCurrentFileName;
		MyGUI::UString mCurrentItemName;

		MyGUI::MapString mCodeGeneratorSettings;
	};

}

#endif

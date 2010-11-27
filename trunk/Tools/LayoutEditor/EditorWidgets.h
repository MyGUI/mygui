#ifndef __EDITOR_WIDGETS_H__
#define __EDITOR_WIDGETS_H__

#include <sstream>
#include "WidgetContainer.h"
#include "SettingsSector.h"

namespace tools
{
	typedef MyGUI::delegates::CMultiDelegate0 Event_ChangeWidgets;

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

		void invalidateWidgets();
		EnumeratorWidgetContainer getWidgets();

		SettingsSector* getSector(const MyGUI::UString& _sectorName);

		std::string getSkinReplace(const std::string& _skinName);

		const MyGUI::UString& getCurrentFileName();
		const MyGUI::UString& getCurrentItemName();

		Event_ChangeWidgets eventChangeWidgets;

		virtual void _unlinkWidget(MyGUI::Widget* _widget);

	private:
		WidgetContainer* _find(MyGUI::Widget* _widget, const std::string& _name, std::vector<WidgetContainer*> _widgets);

		void parseWidget(MyGUI::xml::ElementEnumerator& _widget, MyGUI::Widget* _parent, bool _testMode = false);
		void serialiseWidget(WidgetContainer* _container, MyGUI::xml::ElementPtr _node, bool _compatibility = false);

		void loadIgnoreParameters(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
		void loadSkinReplace(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

		void notifyFrameStarted(float _time);

		void loadSector(MyGUI::xml::ElementPtr _sectorNode);
		void saveSectors(MyGUI::xml::ElementPtr _rootNode);

		void destroyAllSectors();
		void destroyAllWidgets();

		bool isSkinExist(const std::string& _skinName);
		bool checkTemplate(const std::string& _skinName);

		bool loadFromProject(const MyGUI::UString& _fileName, size_t _index);
		bool saveToProject(const MyGUI::UString& _fileName, size_t _index);

		bool unbind(WidgetContainer* _container);

		void loadWidgetsFromXmlNode(MyGUI::xml::ElementPtr _root, bool _testMode = false);
		void saveWidgetsToXmlNode(MyGUI::xml::ElementPtr _root, bool _compatibility = false);
	private:
		bool mWidgetsChanged;
		typedef std::vector<std::string> VectorString;
		VectorString mIgnoreParameters;
		VectorSettingsSector mSettings;
		VectorWidgetContainer mWidgets;

		typedef std::map<std::string, std::string> MapString;
		MapString mSkinReplaces;

		MyGUI::UString mCurrentFileName;
		MyGUI::UString mCurrentItemName;
	};

} // namespace tools

#endif // __EDITOR_WIDGETS_H__

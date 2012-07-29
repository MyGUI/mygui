#ifndef _65c6bab3_d7e3_4691_999e_653b8974ff23_
#define _65c6bab3_d7e3_4691_999e_653b8974ff23_

#include "SkinInfo.h"
#include "WidgetStyle.h"
#include "PossibleValue.h"

namespace tools
{

	class WidgetTypes :
		public MyGUI::Singleton<WidgetTypes>
	{
	public:
		void initialise();
		void shutdown();

		WidgetStyle* findWidgetStyle(const std::string& _type);
		WidgetStyle* findWidgetStyleBySkin(const std::string& _skinName);
		typedef std::vector<std::string> VectorString;
		VectorString findPossibleValues(const std::string& _name);

		void clearAllSkins();

		void addWidgetSkinType(const std::string& _type, const std::string& _skin, const std::string& _group, const std::string& _button_name);

		const SkinGroups& getSkinGroups() const;

		VectorWidgetType getWidgetTypes() const;

	private:
		void loadWidgets(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);
		void loadValues(MyGUI::xml::ElementPtr _node, const std::string& _file, MyGUI::Version _version);

		WidgetStyle* getWidgetType(const std::string& _name);

		PossibleValue* getPossibleValue(const std::string& _name);

		void updateDeep();
		size_t updateDeep(WidgetStyle* _style);

	private:
		VectorWidgetType mWidgetTypes;
		SkinGroups mSkinGroups;
		VectorPossibleValue mPossibleValues;
	};

}

#endif

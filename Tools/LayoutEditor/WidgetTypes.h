#ifndef _65c6bab3_d7e3_4691_999e_653b8974ff23_
#define _65c6bab3_d7e3_4691_999e_653b8974ff23_

#include "SkinInfo.h"
#include "WidgetStyle.h"
#include "PossibleValue.h"

namespace tools
{

	class WidgetTypes
	{
		MYGUI_SINGLETON_DECLARATION(WidgetTypes);

	public:
		WidgetTypes() :
			mSingletonHolder(this)
		{
		}
		void initialise();
		void shutdown();

		WidgetStyle* findWidgetStyle(std::string_view _type);
		WidgetStyle* findWidgetStyleBySkin(std::string_view _skinName);
		using VectorString = std::vector<std::string>;
		VectorString findPossibleValues(std::string_view _name);

		void clearAllSkins();

		void addWidgetSkinType(
			std::string_view _type,
			std::string_view _skin,
			std::string_view _group,
			std::string_view _button_name);

		const SkinGroups& getSkinGroups() const;

		VectorWidgetType getWidgetTypes() const;

	private:
		void loadWidgets(MyGUI::xml::ElementPtr _node, std::string_view _file, MyGUI::Version _version);
		void loadValues(MyGUI::xml::ElementPtr _node, std::string_view _file, MyGUI::Version _version);

		WidgetStyle* getWidgetType(std::string_view _name);

		PossibleValue* getPossibleValue(std::string_view _name);

		void updateDepth();
		size_t updateDepth(WidgetStyle* _style);

	private:
		VectorWidgetType mWidgetTypes;
		SkinGroups mSkinGroups;
		VectorPossibleValue mPossibleValues;
	};

}

#endif

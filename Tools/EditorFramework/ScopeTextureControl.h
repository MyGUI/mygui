/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _31513eda_9f1b_4986_a5a5_466942db581b_
#define _31513eda_9f1b_4986_a5a5_466942db581b_

#include "TextureToolControl.h"
#include "Property.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ScopeTextureControl : public TextureToolControl
	{
	public:
		~ScopeTextureControl() override;

		enum SelectorType
		{
			SelectorNone,
			SelectorCoord,
			SelectorPosition,
			SelectorPositionReadOnly,
			SelectorOffsetH,
			SelectorOffsetV
		};

		void setCoordValue(const MyGUI::IntCoord& _value, SelectorType _type);
		void clearCoordValue();

		using PairCoordType = std::pair<MyGUI::IntCoord, SelectorType>;
		using VectorCoord = std::vector<PairCoordType>;
		void setViewSelectors(const VectorCoord& _selectors);
		void clearViewSelectors();

		void clearAll();

		sigslot::signal1<std::string_view> eventChangeValue;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

		void onMouseButtonClick(const MyGUI::IntPoint& _point) override;

		void onChangeScale() override;

	private:
		void notifyChangeSelection();
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyChangePosition(SelectorControl* _sender);

		void updateFromCoordValue();

		void CommandMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandMoveBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridMoveBottom(const MyGUI::UString& _commandName, bool& _result);

		void CommandSizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandSizeBottom(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeLeft(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeRight(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeTop(const MyGUI::UString& _commandName, bool& _result);
		void CommandGridSizeBottom(const MyGUI::UString& _commandName, bool& _result);

		void updateCaption();

		void setValue(std::string_view _value);

		using PairSelectorType = std::pair<SelectorControl*, SelectorType>;
		using VectorSelector = std::vector<PairSelectorType>;
		SelectorControl* getFreeSelector(
			VectorSelector& _selectors,
			bool _backType,
			SelectorType _type,
			bool& _changes);

	private:
		SelectorControl* mCurrentSelectorControl{nullptr};
		SelectorType mCurrentSelectorType{SelectorNone};

		MyGUI::IntCoord mCoordValue;

		VectorSelector mBlackSelectors;
		VectorSelector mSelectors;
	};

}

#endif

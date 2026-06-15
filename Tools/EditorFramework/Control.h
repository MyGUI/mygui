/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _695efc39_8196_4b64_bc11_76874525dba1_
#define _695efc39_8196_4b64_bc11_76874525dba1_

#include "BaseLayout/BaseLayout.h"
#include "IFactoryItem.h"
#include "IControlController.h"

namespace tools
{

	class MYGUI_EXPORT_DLL Control : public wraps::BaseLayout, public components::IFactoryItem
	{
	public:
		~Control() override;

		MyGUI::Widget* getRoot();

		void Initialise(std::string_view _layoutName = {});
		void Initialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName);

		void Shutdown();

		using VectorControl = std::vector<Control*>;
		const VectorControl& getChilds() const;

		void SendCommand(std::string_view _command);

		template<typename Type>
		Type* findControl()
		{
			Type* result = dynamic_cast<Type*>(this);
			if (result != nullptr)
				return result;

			for (auto& child : mChilds)
			{
				result = child->findControl<Type>();
				if (result != nullptr)
					return result;
			}

			return nullptr;
		}

	protected:
		template<typename Type>
		std::string GetLayoutName(Type* _owner)
		{
			return attribute::AttributeLayout<Type>::getData();
		}

		template<typename Type>
		void InitialiseByAttributes(
			Type* _owner,
			MyGUI::Widget* _parent = nullptr,
			bool _throw = true,
			bool _createFakeWidgets = true)
		{
			typename attribute::AttributeFieldWidgetName<Type>::VectorBindPair& data =
				attribute::AttributeFieldWidgetName<Type>::getData();
			for (typename attribute::AttributeFieldWidgetName<Type>::VectorBindPair::iterator item = data.begin();
				 item != data.end();
				 ++item)
			{
				MyGUI::Widget* value = nullptr;
				assignWidget(value, item->second, _throw, false);

				bool result = item->first->set(_owner, value);

				if (!result && _createFakeWidgets)
				{
					value = CreateFakeWidgetT(item->first->getFieldTypeName(), mMainWidget);
					item->first->set(_owner, value);
				}
			}
		}

		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName);
		virtual void OnCommand(std::string_view _command);

	private:
		void CreateChilds(Control* _parent, MyGUI::Widget* _widget);
		void AdviceWidget(MyGUI::Widget* _widget);
		void CreateControllers();
		void ActivateControllers();
		void DeactivateControllers();

		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, std::string_view _name);
		void notifyEditSelectAccept(MyGUI::EditBox* _sender);

		MyGUI::Widget* CreateFakeWidgetT(std::string_view _typeName, MyGUI::Widget* _parent);

	private:
		VectorControl mChilds;
		Control* mParent{nullptr};
		using VectorController = std::vector<IControlController*>;
		VectorController mControllers;
	};

}

#endif

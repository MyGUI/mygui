/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyFieldManager.h"
#include "PropertyFieldComboBox.h"
#include "PropertyFieldEditBox.h"
#include "PropertyFieldSkin.h"
#include "PropertyFieldType.h"
#include "PropertyFieldFont.h"
#include "PropertyFieldFileName.h"
#include "PropertyFieldNumeric.h"
#include "PropertyFieldAlpha.h"
#include "PropertyFieldPosition.h"
#include "PropertyFieldColour.h"

template <> tools::PropertyFieldManager* MyGUI::Singleton<tools::PropertyFieldManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::PropertyFieldManager>::mClassTypeName = "PropertyFieldManager";

namespace tools
{

	template <typename Type>
	class GenericFactory
	{
	public:
		typedef MyGUI::delegates::CDelegate2<IPropertyField*&, MyGUI::Widget*> Delegate;
		static typename Delegate::IDelegate* getFactory()
		{
			return MyGUI::newDelegate(createFromFactory);
		}

	private:
		static void createFromFactory(IPropertyField*& _instance, MyGUI::Widget* _parent)
		{
			_instance = new Type(_parent);
		}
	};

	PropertyFieldManager::PropertyFieldManager()
	{
	}

	PropertyFieldManager::~PropertyFieldManager()
	{
	}

	void PropertyFieldManager::initialise()
	{
		mFactories["Name"] = GenericFactory<PropertyFieldEditBox>::getFactory();
		mFactories["Type"] = GenericFactory<PropertyFieldType>::getFactory();
		mFactories["Skin"] = GenericFactory<PropertyFieldSkin>::getFactory();
		mFactories["Font"] = GenericFactory<PropertyFieldFont>::getFactory();
		mFactories["Position"] = GenericFactory<PropertyFieldPosition>::getFactory();
		mFactories["Layer"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["String"] = GenericFactory<PropertyFieldEditBox>::getFactory();
		mFactories["Align"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["FileName"] = GenericFactory<PropertyFieldFileName>::getFactory();
		mFactories["1 int"] = GenericFactory<PropertyFieldNumeric>::getFactory();
		mFactories["2 int"] = GenericFactory<PropertyFieldNumeric>::getFactory();
		mFactories["4 int"] = GenericFactory<PropertyFieldNumeric>::getFactory();
		mFactories["1 float"] = GenericFactory<PropertyFieldNumeric>::getFactory();
		mFactories["2 float"] = GenericFactory<PropertyFieldNumeric>::getFactory();
		mFactories["Alpha"] = GenericFactory<PropertyFieldAlpha>::getFactory();
		mFactories["Colour"] = GenericFactory<PropertyFieldColour>::getFactory();
		mFactories["Bool"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["TextAlign"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["FlowDirection"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["CanvasAlign"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["MenuItemType"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["ItemResizingPolicy"] = GenericFactory<PropertyFieldComboBox>::getFactory();
		mFactories["Function"] = GenericFactory<PropertyFieldComboBox>::getFactory();
	}

	void PropertyFieldManager::shutdown()
	{
	}

	IPropertyField* PropertyFieldManager::createPropertyField(MyGUI::Widget* _window, const std::string& _type)
	{
		IPropertyField* result = nullptr;

		MapFactoryItem::iterator item = mFactories.find(_type);
		MYGUI_ASSERT(item != mFactories.end(), "Factory PropertyField '" << _type << "' not found.");

		(*item).second(result, _window);
		result->initialise(_type);

		return result;
	}

}

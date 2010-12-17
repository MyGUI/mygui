/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#include "Precompiled.h"
#include "PropertyFieldSkin.h"
#include "WidgetTypes.h"
#include "Localise.h"
#include "EditorWidgets.h"
#include "UndoManager.h"
#include "WidgetSelectorManager.h"
#include "GroupMessage.h"

namespace tools
{

	PropertyFieldSkin::PropertyFieldSkin(MyGUI::Widget* _parent) :
		PropertyFieldComboBox(_parent)
	{
	}

	PropertyFieldSkin::~PropertyFieldSkin()
	{
	}

	void PropertyFieldSkin::onFillValues()
	{
		WidgetStyle::VectorString values = WidgetTypes::getInstance().findWidgetStyle(mCurrentWidget->getTypeName())->skin;

		// добавляем скины и шаблоны
		MyGUI::ResourceManager::EnumeratorPtr resource = MyGUI::ResourceManager::getInstance().getEnumerator();
		while (resource.next())
		{
			MyGUI::ResourceSkin* resourceSkin = resource.current().second->castType<MyGUI::ResourceSkin>(false);
			if (resourceSkin != nullptr)
			{
				if (!MyGUI::utility::startWith(resourceSkin->getResourceName(), "LE_"))
					values.push_back(replaceTags("ColourDefault") + resourceSkin->getResourceName());
			}

			MyGUI::ResourceLayout* resourceLayout = resource.current().second->castType<MyGUI::ResourceLayout>(false);
			if (resourceLayout != nullptr)
			{
				if (!MyGUI::utility::endWith(resourceLayout->getResourceName(), ".layout"))
				{
					if (!MyGUI::utility::startWith(resourceLayout->getResourceName(), "LE_"))
						values.push_back(replaceTags("ColourDefault") + resourceLayout->getResourceName());
				}
			}
		}

		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

	void PropertyFieldSkin::onAction(const std::string& _value)
	{
		WidgetContainer* widgetContainer = EditorWidgets::getInstance().find(mCurrentWidget);

		widgetContainer->skin = _value;
		if (isSkinExist(widgetContainer->skin) || widgetContainer->skin.empty())
		{
			WidgetSelectorManager::getInstance().saveSelectedWidget();

			MyGUI::xml::Document* savedDoc = EditorWidgets::getInstance().savexmlDocument();
			EditorWidgets::getInstance().clear();
			EditorWidgets::getInstance().loadxmlDocument(savedDoc);
			delete savedDoc;

			WidgetSelectorManager::getInstance().restoreSelectedWidget();
		}
		else
		{
			std::string mess = MyGUI::utility::toString("Skin '", widgetContainer->skin, "' not found. This value will be saved.");
			GroupMessage::getInstance().addMessage(mess, MyGUI::LogLevel::Error);
		}

		UndoManager::getInstance().addValue(PR_PROPERTIES);
	}

	void PropertyFieldSkin::onToolTip(const MyGUI::ToolTipInfo& _info)
	{
		if (_info.type == MyGUI::ToolTipInfo::Show)
		{
			SkinInfo data = getCellData(_info.index);
			EditorToolTip::getInstancePtr()->show(data);
			EditorToolTip::getInstancePtr()->move(_info.point);
		}
		else if (_info.type == MyGUI::ToolTipInfo::Hide)
		{
			EditorToolTip::getInstancePtr()->hide();
		}
		else if (_info.type == MyGUI::ToolTipInfo::Move)
		{
			EditorToolTip::getInstancePtr()->move(_info.point);
		}
	}

	SkinInfo PropertyFieldSkin::getCellData(size_t _index)
	{
		if (_index != MyGUI::ITEM_NONE)
		{
			MyGUI::UString name = mField->getItemNameAt(_index);
			return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "", "");
		}
		else
		{
			MyGUI::UString name = mField->getCaption();
			return SkinInfo(MyGUI::TextIterator::getOnlyText(name), "", "");
		}
	}

	bool PropertyFieldSkin::isSkinExist(const std::string& _skinName)
	{
		return _skinName == "Default" ||
			MyGUI::SkinManager::getInstance().isExist(_skinName) ||
			(MyGUI::LayoutManager::getInstance().isExist(_skinName) && checkTemplate(_skinName));
	}

	bool PropertyFieldSkin::checkTemplate(const std::string& _skinName)
	{
		MyGUI::ResourceLayout* templateInfo = MyGUI::LayoutManager::getInstance().getByName(_skinName, false);
		if (templateInfo != nullptr)
		{
			const MyGUI::VectorWidgetInfo& data = templateInfo->getLayoutData();
			for (MyGUI::VectorWidgetInfo::const_iterator container = data.begin(); container != data.end(); ++container)
			{
				if (container->name == "Root")
					return true;
			}
		}

		return false;
	}

} // namespace tools

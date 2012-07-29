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

namespace tools
{

	PropertyFieldSkin::PropertyFieldSkin(MyGUI::Widget* _parent) :
		PropertyFieldComboBox(_parent)
	{
		mField->eventToolTip += newDelegate (this, &PropertyFieldSkin::notifyToolTip);
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

		mField->removeAllItems();
		for (WidgetStyle::VectorString::iterator iter = values.begin(); iter != values.end(); ++iter)
			mField->addItem(*iter);
		mField->beginToItemFirst();
	}

	void PropertyFieldSkin::notifyToolTip(MyGUI::Widget* _sender, const MyGUI::ToolTipInfo& _info)
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

}

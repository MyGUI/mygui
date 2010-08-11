/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinItem.h"

namespace tools
{

	SkinItem::SkinItem() :
		mPropertySet(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Texture", "Texture");
		mPropertySet->createChild("Coord", "IntCoord", "0 0 32 32");

		StateItem* skin = nullptr;
		skin = mStates.createChild();
		skin->setName("Disabled");
		skin = mStates.createChild();
		skin->setName("Normal");
		skin = mStates.createChild();
		skin->setName("Over");
		skin = mStates.createChild();
		skin->setName("Pressed");
		skin = mStates.createChild();
		skin->setName("Selected Disabled");
		skin = mStates.createChild();
		skin->setName("Selected Normal");
		skin = mStates.createChild();
		skin->setName("Selected Over");
		skin = mStates.createChild();
		skin->setName("Selected Pressed");

		SeparatorItem* separator = nullptr;
		separator = mSeparators.createChild();
		separator->setName("Top");
		separator->setCorner(MyGUI::Align::Top);
		separator = mSeparators.createChild();
		separator->setName("Bottom");
		separator->setCorner(MyGUI::Align::Bottom);
		separator = mSeparators.createChild();
		separator->setName("Left");
		separator->setCorner(MyGUI::Align::Left);
		separator = mSeparators.createChild();
		separator->setName("Right");
		separator->setCorner(MyGUI::Align::Right);

		RegionItem* region = nullptr;

		region = mRegions.createChild();
		region->setName("Left Top");
		region->setSeparator(MyGUI::Align::Left | MyGUI::Align::Top);
		region->getPropertySet()->setPropertyValue("Align", "Left Top", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Top");
		region->setSeparator(MyGUI::Align::Top);
		region->getPropertySet()->setPropertyValue("Align", "HStretch Top", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Right Top");
		region->setSeparator(MyGUI::Align::Right | MyGUI::Align::Top);
		region->getPropertySet()->setPropertyValue("Align", "Right Top", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Right");
		region->setSeparator(MyGUI::Align::Right);
		region->getPropertySet()->setPropertyValue("Align", "Right VStretch", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Right Bottom");
		region->setSeparator(MyGUI::Align::Right | MyGUI::Align::Bottom);
		region->getPropertySet()->setPropertyValue("Align", "Right Bottom", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Bottom");
		region->setSeparator(MyGUI::Align::Bottom);
		region->getPropertySet()->setPropertyValue("Align", "HStretch Bottom", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Left Bottom");
		region->setSeparator(MyGUI::Align::Left | MyGUI::Align::Bottom);
		region->getPropertySet()->setPropertyValue("Align", "Left Bottom", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Left");
		region->setSeparator(MyGUI::Align::Left);
		region->getPropertySet()->setPropertyValue("Align", "Left VStretch", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Center");
		region->setSeparator(MyGUI::Align::Center);
		region->getPropertySet()->setPropertyValue("Align", "Stretch", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Text");
		region->setSeparator(MyGUI::Align::Center);
		region->getPropertySet()->setPropertyValue("Align", "Stretch", "");
		region->getPropertySet()->setPropertyValue("RegionType", "EditText", "");
		region->getPropertySet()->setPropertyValue("Visible", "False", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
	}

	SkinItem::~SkinItem()
	{
		delete mPropertySet;
	}

	const MyGUI::UString& SkinItem::getName()
	{
		return mName;
	}

	void SkinItem::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	PropertySet* SkinItem::getPropertySet()
	{
		return mPropertySet;
	}

} // namespace tools

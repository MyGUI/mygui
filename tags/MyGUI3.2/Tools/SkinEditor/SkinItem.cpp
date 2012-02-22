/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "SkinItem.h"

namespace tools
{

	SkinItem::SkinItem() :
		mPropertySet(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Texture", "Texture");
		mPropertySet->createChild("Coord", "IntCoord", "0 0 32 32");

		StateItem* state = nullptr;

		state = mStates.createChild();
		state->setName("Disabled");
		state->getPropertySet()->setPropertyValue("Visible", "False", "");

		state = mStates.createChild();
		state->setName("Normal");
		state->getPropertySet()->setPropertyValue("Visible", "True", "");

		state = mStates.createChild();
		state->setName("Over");
		state->getPropertySet()->setPropertyValue("Visible", "False", "");

		state = mStates.createChild();
		state->setName("Pressed");
		state->getPropertySet()->setPropertyValue("Visible", "False", "");

		state = mStates.createChild();
		state->setName("Selected Disabled");
		state->getPropertySet()->setPropertyValue("Visible", "False", "");

		state = mStates.createChild();
		state->setName("Selected Normal");
		state->getPropertySet()->setPropertyValue("Visible", "False", "");

		state = mStates.createChild();
		state->setName("Selected Over");
		state->getPropertySet()->setPropertyValue("Visible", "False", "");

		state = mStates.createChild();
		state->setName("Selected Pressed");
		state->getPropertySet()->setPropertyValue("Visible", "False", "");


		SeparatorItem* separator = nullptr;

		separator = mSeparators.createChild();
		separator->setName("Top");
		separator->setCorner(MyGUI::Align::Top);
		separator->getPropertySet()->setPropertyValue("Offset", "1", "");

		separator = mSeparators.createChild();
		separator->setName("Bottom");
		separator->setCorner(MyGUI::Align::Bottom);
		separator->getPropertySet()->setPropertyValue("Offset", "1", "");

		separator = mSeparators.createChild();
		separator->setName("Left");
		separator->setCorner(MyGUI::Align::Left);
		separator->getPropertySet()->setPropertyValue("Offset", "1", "");

		separator = mSeparators.createChild();
		separator->setName("Right");
		separator->setCorner(MyGUI::Align::Right);
		separator->getPropertySet()->setPropertyValue("Offset", "1", "");


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
		region->getPropertySet()->setPropertyValue("RegionType", "SimpleText", "");
		region->getPropertySet()->setPropertyValue("Visible", "False", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
	}

	SkinItem::~SkinItem()
	{
		delete mPropertySet;
	}

	const MyGUI::UString& SkinItem::getName() const
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

	ItemHolder<StateItem>& SkinItem::getStates()
	{
		return mStates;
	}
	ItemHolder<SeparatorItem>& SkinItem::getSeparators()
	{
		return mSeparators;
	}
	ItemHolder<RegionItem>& SkinItem::getRegions()
	{
		return mRegions;
	}

	void SkinItem::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->addAttribute("name", mName);

		ItemHolder<StateItem>::EnumeratorItem stateItems = mStates.getChildsEnumerator();
		while (stateItems.next())
		{
			MyGUI::xml::Element* node = _node->createChild("StateItem");
			stateItems->serialization(node, _version);
		}

		ItemHolder<SeparatorItem>::EnumeratorItem separatorItems = mSeparators.getChildsEnumerator();
		while (separatorItems.next())
		{
			MyGUI::xml::Element* node = _node->createChild("SeparatorItem");
			separatorItems->serialization(node, _version);
		}

		ItemHolder<RegionItem>::EnumeratorItem regionItems = mRegions.getChildsEnumerator();
		while (regionItems.next())
		{
			MyGUI::xml::Element* node = _node->createChild("RegionItem");
			regionItems->serialization(node, _version);
		}

		MyGUI::xml::Element* node = _node->createChild("PropertySet");
		mPropertySet->serialization(node, _version);
	}

	/*void SkinItem::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		mName = _node->findAttribute("name");

		mStates.destroyAllChilds();
		mSeparators.destroyAllChilds();
		mRegions.destroyAllChilds();

		MyGUI::xml::ElementEnumerator nodes = _node->getElementEnumerator();
		while (nodes.next())
		{
			MyGUI::xml::Element* node = nodes.current();

			if (node->getName() == "PropertySet")
			{
				mPropertySet->deserialization(node, _version);
			}
			else if (node->getName() == "StateItem")
			{
				StateItem* item = mStates.createChild();
				item->deserialization(node, _version);
			}
			else if (node->getName() == "SeparatorItem")
			{
				SeparatorItem* item = mSeparators.createChild();
				item->deserialization(node, _version);
			}
			else if (node->getName() == "RegionItem")
			{
				RegionItem* item = mRegions.createChild();
				item->deserialization(node, _version);
			}
		}
	}*/

	void SkinItem::parseProperty(MyGUI::xml::Element* _node)
	{
		Property* proper = mPropertySet->getChild("Texture");
		if (proper != nullptr)
			proper->setValue(_node->findAttribute("texture"), "");

		proper = mPropertySet->getChild("Coord");
		if (proper != nullptr)
			proper->setValue(getNormalStateOffset(_node).print() + " " + _node->findAttribute("size"), "");
	}

	void SkinItem::parseRegions(MyGUI::xml::Element* _node)
	{
		mRegions.destroyAllChilds();

		RegionItem* regionLeftTop = mRegions.createChild();
		regionLeftTop->setName("Left Top");
		regionLeftTop->setSeparator(MyGUI::Align::Left | MyGUI::Align::Top);
		regionLeftTop->getPropertySet()->setPropertyValue("Align", "Left Top", "");
		regionLeftTop->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionLeftTop->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionLeftTop->getPropertySet()->setPropertyReadOnly("Position", true);
		regionLeftTop->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionTop = mRegions.createChild();
		regionTop->setName("Top");
		regionTop->setSeparator(MyGUI::Align::Top);
		regionTop->getPropertySet()->setPropertyValue("Align", "HStretch Top", "");
		regionTop->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionTop->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionTop->getPropertySet()->setPropertyReadOnly("Position", true);
		regionTop->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionRightTop = mRegions.createChild();
		regionRightTop->setName("Right Top");
		regionRightTop->setSeparator(MyGUI::Align::Right | MyGUI::Align::Top);
		regionRightTop->getPropertySet()->setPropertyValue("Align", "Right Top", "");
		regionRightTop->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionRightTop->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionRightTop->getPropertySet()->setPropertyReadOnly("Position", true);
		regionRightTop->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionRight = mRegions.createChild();
		regionRight->setName("Right");
		regionRight->setSeparator(MyGUI::Align::Right);
		regionRight->getPropertySet()->setPropertyValue("Align", "Right VStretch", "");
		regionRight->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionRight->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionRight->getPropertySet()->setPropertyReadOnly("Position", true);
		regionRight->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionRightBottom = mRegions.createChild();
		regionRightBottom->setName("Right Bottom");
		regionRightBottom->setSeparator(MyGUI::Align::Right | MyGUI::Align::Bottom);
		regionRightBottom->getPropertySet()->setPropertyValue("Align", "Right Bottom", "");
		regionRightBottom->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionRightBottom->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionRightBottom->getPropertySet()->setPropertyReadOnly("Position", true);
		regionRightBottom->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionBottom = mRegions.createChild();
		regionBottom->setName("Bottom");
		regionBottom->setSeparator(MyGUI::Align::Bottom);
		regionBottom->getPropertySet()->setPropertyValue("Align", "HStretch Bottom", "");
		regionBottom->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionBottom->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionBottom->getPropertySet()->setPropertyReadOnly("Position", true);
		regionBottom->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionLeftBottom = mRegions.createChild();
		regionLeftBottom->setName("Left Bottom");
		regionLeftBottom->setSeparator(MyGUI::Align::Left | MyGUI::Align::Bottom);
		regionLeftBottom->getPropertySet()->setPropertyValue("Align", "Left Bottom", "");
		regionLeftBottom->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionLeftBottom->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionLeftBottom->getPropertySet()->setPropertyReadOnly("Position", true);
		regionLeftBottom->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionLeft = mRegions.createChild();
		regionLeft->setName("Left");
		regionLeft->setSeparator(MyGUI::Align::Left);
		regionLeft->getPropertySet()->setPropertyValue("Align", "Left VStretch", "");
		regionLeft->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionLeft->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionLeft->getPropertySet()->setPropertyReadOnly("Position", true);
		regionLeft->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionCenter = mRegions.createChild();
		regionCenter->setName("Center");
		regionCenter->setSeparator(MyGUI::Align::Center);
		regionCenter->getPropertySet()->setPropertyValue("Align", "Stretch", "");
		regionCenter->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionCenter->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionCenter->getPropertySet()->setPropertyReadOnly("Position", true);
		regionCenter->getPropertySet()->setPropertyReadOnly("Align", true);

		RegionItem* regionText = mRegions.createChild();
		regionText->setName("Text");
		regionText->setSeparator(MyGUI::Align::Center);
		regionText->getPropertySet()->setPropertyValue("Align", "Stretch", "");
		regionText->getPropertySet()->setPropertyReadOnly("Enabled", true);
		regionText->getPropertySet()->setPropertyValue("Visible", "False", "");
		regionText->getPropertySet()->setPropertyValue("RegionType", "SimpleText", "");

		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("BasisSkin"))
		{
			std::string align = node->findAttribute("align");
			std::string type = node->findAttribute("type");
			std::string offset = node->findAttribute("offset");

			if (type == "SubSkin" || type == "TileRect")
			{
				if (type == "TileRect")
				{
					MyGUI::xml::ElementEnumerator nodeState = node->getElementEnumerator();
					while (nodeState.next("State"))
					{
						if (nodeState->findAttribute("name") == "normal")
						{
							bool tileH = false;
							bool tileV = false;

							MyGUI::xml::ElementEnumerator nodeProperty = nodeState->getElementEnumerator();
							while (nodeProperty.next("Property"))
							{
								if (nodeProperty->findAttribute("key") == "TileH")
									tileH = nodeProperty->findAttribute("value") == "true";
								else if (nodeProperty->findAttribute("key") == "TileV")
									tileV = nodeProperty->findAttribute("value") == "true";
							}

							if (tileV && !tileH)
								type = "TileRect Ver";
							else if (!tileV && tileH)
								type = "TileRect Hor";

							break;
						}
					}
				}

				if (align == "Left Top" || align == "Default")
				{
					regionLeftTop->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionLeftTop->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionLeftTop->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "HStretch Top")
				{
					regionTop->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionTop->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionTop->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "Right Top")
				{
					regionRightTop->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionRightTop->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionRightTop->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "Right VStretch")
				{
					regionRight->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionRight->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionRight->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "Right Bottom")
				{
					regionRightBottom->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionRightBottom->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionRightBottom->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "HStretch Bottom")
				{
					regionBottom->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionBottom->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionBottom->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "Left Bottom")
				{
					regionLeftBottom->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionLeftBottom->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionLeftBottom->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "Left VStretch")
				{
					regionLeft->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionLeft->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionLeft->getPropertySet()->setPropertyValue("Position", offset, "");
				}
				else if (align == "Stretch" || align == "HStretch VStretch")
				{
					regionCenter->getPropertySet()->setPropertyValue("Visible", "True", "");
					regionCenter->getPropertySet()->setPropertyValue("RegionType", type, "");
					regionCenter->getPropertySet()->setPropertyValue("Position", offset, "");
				}
			}
			else if (type == "SimpleText" || type == "EditText")
			{
				regionText->getPropertySet()->setPropertyValue("Visible", "True", "");
				regionText->getPropertySet()->setPropertyValue("RegionType", type, "");
				regionText->getPropertySet()->setPropertyValue("Position", offset, "");
				regionText->getPropertySet()->setPropertyValue("Align", align, "");
			}
		}
	}

	void SkinItem::parseSeparators(MyGUI::xml::Element* _node)
	{
		int left = 0;
		int top = 0;
		int right = 0;
		int bottom = 0;

		ItemHolder<RegionItem>::EnumeratorItem regionItem = mRegions.getChildsEnumerator();
		while (regionItem.next())
		{
			if (regionItem->getPropertySet()->getPropertyValue("Visible") == "False")
				continue;
			if (regionItem->getPropertySet()->getPropertyValue("Enabled") == "False")
				continue;

			if (regionItem->getName() == "Left Top"
				|| regionItem->getName() == "Left"
				|| regionItem->getName() == "Left Bottom")
			{
				left = MyGUI::IntCoord::parse(regionItem->getPropertySet()->getPropertyValue("Position")).width;
			}
			else if (regionItem->getName() == "Right Top"
				|| regionItem->getName() == "Right"
				|| regionItem->getName() == "Right Bottom")
			{
				right = MyGUI::IntCoord::parse(regionItem->getPropertySet()->getPropertyValue("Position")).width;
			}
			if (regionItem->getName() == "Left Top"
				|| regionItem->getName() == "Top"
				|| regionItem->getName() == "Right Top")
			{
				top = MyGUI::IntCoord::parse(regionItem->getPropertySet()->getPropertyValue("Position")).height;
			}
			else if (regionItem->getName() == "Left Bottom"
				|| regionItem->getName() == "Bottom"
				|| regionItem->getName() == "Right Bottom")
			{
				bottom = MyGUI::IntCoord::parse(regionItem->getPropertySet()->getPropertyValue("Position")).height;
			}
		}

		mSeparators.destroyAllChilds();

		SeparatorItem* separatorTop = mSeparators.createChild();
		separatorTop->setName("Top");
		separatorTop->setCorner(MyGUI::Align::Top);
		if (top != 0)
		{
			separatorTop->getPropertySet()->setPropertyValue("Offset", MyGUI::utility::toString(top), "");
			separatorTop->getPropertySet()->setPropertyValue("Visible", "True", "");
		}

		SeparatorItem* separatorBottom = mSeparators.createChild();
		separatorBottom->setName("Bottom");
		separatorBottom->setCorner(MyGUI::Align::Bottom);
		if (bottom != 0)
		{
			separatorBottom->getPropertySet()->setPropertyValue("Offset", MyGUI::utility::toString(bottom), "");
			separatorBottom->getPropertySet()->setPropertyValue("Visible", "True", "");
		}

		SeparatorItem* separatorLeft = mSeparators.createChild();
		separatorLeft->setName("Left");
		separatorLeft->setCorner(MyGUI::Align::Left);
		if (left != 0)
		{
			separatorLeft->getPropertySet()->setPropertyValue("Offset", MyGUI::utility::toString(left), "");
			separatorLeft->getPropertySet()->setPropertyValue("Visible", "True", "");
		}

		SeparatorItem* separatorRight = mSeparators.createChild();
		separatorRight->setName("Right");
		separatorRight->setCorner(MyGUI::Align::Right);
		if (right != 0)
		{
			separatorRight->getPropertySet()->setPropertyValue("Offset", MyGUI::utility::toString(right), "");
			separatorRight->getPropertySet()->setPropertyValue("Visible", "True", "");
		}
	}

	void SkinItem::parseStates(MyGUI::xml::Element* _node)
	{
		mStates.destroyAllChilds();

		StateItem* stateDisabled = mStates.createChild();
		stateDisabled->setName("Disabled");
		stateDisabled->getPropertySet()->setPropertyValue("Visible", "False", "");

		StateItem* stateNormal = mStates.createChild();
		stateNormal->setName("Normal");
		stateNormal->getPropertySet()->setPropertyValue("Visible", "True", "");

		StateItem* stateOver = mStates.createChild();
		stateOver->setName("Over");
		stateOver->getPropertySet()->setPropertyValue("Visible", "False", "");

		StateItem* statePressed = mStates.createChild();
		statePressed->setName("Pressed");
		statePressed->getPropertySet()->setPropertyValue("Visible", "False", "");

		StateItem* stateSelectedDisabled = mStates.createChild();
		stateSelectedDisabled->setName("Selected Disabled");
		stateSelectedDisabled->getPropertySet()->setPropertyValue("Visible", "False", "");

		StateItem* stateSelectedNormal = mStates.createChild();
		stateSelectedNormal->setName("Selected Normal");
		stateSelectedNormal->getPropertySet()->setPropertyValue("Visible", "False", "");

		StateItem* stateSelectedOver = mStates.createChild();
		stateSelectedOver->setName("Selected Over");
		stateSelectedOver->getPropertySet()->setPropertyValue("Visible", "False", "");

		StateItem* stateSelectedPressed = mStates.createChild();
		stateSelectedPressed->setName("Selected Pressed");
		stateSelectedPressed->getPropertySet()->setPropertyValue("Visible", "False", "");

		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("BasisSkin"))
		{
			std::string type = node->findAttribute("type");
			MyGUI::IntPoint skinOffset = MyGUI::IntCoord::parse(node->findAttribute("offset")).point();

			MyGUI::xml::ElementEnumerator nodeState = node->getElementEnumerator();
			while (nodeState.next("State"))
			{
				std::string name = nodeState->findAttribute("name");
				std::string position = (MyGUI::IntCoord::parse(nodeState->findAttribute("offset")).point() - skinOffset).print();
				std::string shift = MyGUI::utility::toString(MyGUI::utility::parseValue<int>(nodeState->findAttribute("shift")));
				MyGUI::Colour colourValue = MyGUI::Colour::parse(nodeState->findAttribute("colour"));
				std::string colour = MyGUI::utility::toString(colourValue.red, " ", colourValue.green, " ", colourValue.blue);

				if (type == "SubSkin" || type == "TileRect")
				{
					if (name == "disabled")
					{
						stateDisabled->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateDisabled->getPropertySet()->setPropertyValue("Position", position, "");
					}
					else if (name == "normal")
					{
						stateNormal->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateNormal->getPropertySet()->setPropertyValue("Position", position, "");
					}
					else if (name == "highlighted")
					{
						stateOver->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateOver->getPropertySet()->setPropertyValue("Position", position, "");
					}
					else if (name == "pushed")
					{
						statePressed->getPropertySet()->setPropertyValue("Visible", "True", "");
						statePressed->getPropertySet()->setPropertyValue("Position", position, "");
					}
					else if (name == "disabled_checked")
					{
						stateSelectedDisabled->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedDisabled->getPropertySet()->setPropertyValue("Position", position, "");
					}
					else if (name == "normal_checked")
					{
						stateSelectedNormal->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedNormal->getPropertySet()->setPropertyValue("Position", position, "");
					}
					else if (name == "highlighted_checked")
					{
						stateSelectedOver->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedOver->getPropertySet()->setPropertyValue("Position", position, "");
					}
					else if (name == "pushed_checked")
					{
						stateSelectedPressed->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedPressed->getPropertySet()->setPropertyValue("Position", position, "");
					}
				}
				else if (type == "SimpleText" || type == "EditText")
				{
					if (name == "disabled")
					{
						stateDisabled->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateDisabled->getPropertySet()->setPropertyValue("TextColour", colour, "");
						stateDisabled->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
					else if (name == "normal")
					{
						stateNormal->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateNormal->getPropertySet()->setPropertyValue("TextColour", colour, "");
						stateNormal->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
					else if (name == "highlighted")
					{
						stateOver->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateOver->getPropertySet()->setPropertyValue("TextColour", colour, "");
						stateOver->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
					else if (name == "pushed")
					{
						statePressed->getPropertySet()->setPropertyValue("Visible", "True", "");
						statePressed->getPropertySet()->setPropertyValue("TextColour", colour, "");
						statePressed->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
					else if (name == "disabled_checked")
					{
						stateSelectedDisabled->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedDisabled->getPropertySet()->setPropertyValue("TextColour", colour, "");
						stateSelectedDisabled->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
					else if (name == "normal_checked")
					{
						stateSelectedNormal->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedNormal->getPropertySet()->setPropertyValue("TextColour", colour, "");
						stateSelectedNormal->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
					else if (name == "highlighted_checked")
					{
						stateSelectedOver->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedOver->getPropertySet()->setPropertyValue("TextColour", colour, "");
						stateSelectedOver->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
					else if (name == "pushed_checked")
					{
						stateSelectedPressed->getPropertySet()->setPropertyValue("Visible", "True", "");
						stateSelectedPressed->getPropertySet()->setPropertyValue("TextColour", colour, "");
						stateSelectedPressed->getPropertySet()->setPropertyValue("TextShift", shift, "");
					}
				}
			}
		}
	}

	void SkinItem::deserialization2(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		mName = _node->findAttribute("name");

		parseProperty(_node);
		parseRegions(_node);
		parseSeparators(_node);
		parseStates(_node);
	}

	MyGUI::IntPoint SkinItem::getNormalStateOffset(MyGUI::xml::Element* _node)
	{
		MyGUI::IntPoint result;
		bool found = false;

		MyGUI::xml::ElementEnumerator node = _node->getElementEnumerator();
		while (node.next("BasisSkin"))
		{
			MyGUI::xml::ElementEnumerator nodeState = node->getElementEnumerator();
			while (nodeState.next("State"))
			{
				if (nodeState->findAttribute("name") == "normal")
				{
					std::string offset = nodeState->findAttribute("offset");
					if (!offset.empty())
					{
						MyGUI::IntPoint position = MyGUI::IntCoord::parse(offset).point();
						if (found)
						{
							result.set(std::min(result.left, position.left), std::min(result.top, position.top));
						}
						else
						{
							result = position;
							found = true;
						}
					}
				}
			}
		}

		return result;
	}

	/*void SkinItem::serialization2(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
	}*/

} // namespace tools

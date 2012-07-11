/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "ExportManager.h"

template <> tools::ExportManager* MyGUI::Singleton<tools::ExportManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::ExportManager>::mClassTypeName("ExportManager");

namespace tools
{

	ExportManager::ExportManager()
	{
	}

	ExportManager::~ExportManager()
	{
	}

	void ExportManager::initialise()
	{
	}

	void ExportManager::shutdown()
	{
	}

	void ExportManager::convertSkin(MyGUI::xml::Element* _from, MyGUI::xml::Element* _to)
	{
		_to->addAttribute("type", "ResourceSkin");
		_to->addAttribute("name", _from->findAttribute("name"));

		MyGUI::xml::ElementEnumerator nodes = _from->getElementEnumerator();
		while (nodes.next())
		{
			MyGUI::UString coordValue;
			MyGUI::UString textureValue;

			MyGUI::xml::Element* node = nodes.current();
			if (node->getName() == "PropertySet")
			{
				MyGUI::xml::ElementEnumerator propertyNodes = node->getElementEnumerator();
				while (propertyNodes.next("Property"))
				{
					MyGUI::xml::Element* propertyNode = propertyNodes.current();

					std::string name;
					if (propertyNode->findAttribute("name", name))
					{
						if (name == "Coord")
						{
							MyGUI::IntCoord coord = MyGUI::IntCoord::parse(propertyNode->getContent());
							coordValue = coord.size().print();
						}
						else if (name == "Texture")
						{
							textureValue = propertyNode->getContent();
						}
					}
				}

				_to->addAttribute("size", coordValue);
				_to->addAttribute("texture", textureValue);

				break;
			}
			else if (node->getName() == "RegionItem")
			{
				bool regionVisible = true;

				MyGUI::xml::ElementEnumerator regionNodes = node->getElementEnumerator();
				while (regionNodes.next("PropertySet"))
				{
					MyGUI::xml::ElementEnumerator propertyNodes = regionNodes->getElementEnumerator();
					while (propertyNodes.next("Property"))
					{
						MyGUI::xml::Element* propertyNode = propertyNodes.current();

						std::string name;
						if (propertyNode->findAttribute("name", name))
						{
							if (name == "Visible")
							{
								if (propertyNode->getContent() != "True")
									regionVisible = false;
							}
							else if (name == "Enabled")
							{
								if (propertyNode->getContent() != "True")
									regionVisible = false;
							}
						}
					}
					break;
				}

				if (!regionVisible)
					continue;

				MyGUI::xml::Element* region = _to->createChild("BasisSkin");
				MyGUI::IntCoord regionCoord;

				MyGUI::UString regionTypeValue;
				MyGUI::UString regionOffsetValue;
				MyGUI::UString alignValue;

				regionNodes = node->getElementEnumerator();
				while (regionNodes.next("PropertySet"))
				{
					MyGUI::xml::ElementEnumerator regionPropertyNode = regionNodes->getElementEnumerator();
					while (regionPropertyNode.next("Property"))
					{
						std::string name;
						if (regionPropertyNode->findAttribute("name", name))
						{
							if (name == "RegionType")
							{
								regionTypeValue = regionPropertyNode->getContent();
							}
							else if (name == "Position")
							{
								regionCoord = MyGUI::IntCoord::parse(regionPropertyNode->getContent());
								regionOffsetValue = regionCoord.print();
							}
							else if (name == "Align")
							{
								alignValue = regionPropertyNode->getContent();
							}
						}
					}

					bool tileHor = true;
					bool tileVer = true;

					if (regionTypeValue == "TileRect Hor")
					{
						regionTypeValue = "TileRect";
						tileVer = false;
					}
					else if (regionTypeValue == "TileRect Ver")
					{
						regionTypeValue = "TileRect";
						tileHor = false;
					}

					region->addAttribute("type", regionTypeValue);
					region->addAttribute("offset", regionOffsetValue);
					region->addAttribute("align", alignValue);

					MyGUI::xml::ElementEnumerator stateNodes = _from->getElementEnumerator();
					while (stateNodes.next("StateItem"))
					{
						bool stateVisible = true;

						MyGUI::xml::ElementEnumerator propertySetNodes = stateNodes->getElementEnumerator();
						while (propertySetNodes.next("PropertySet"))
						{
							MyGUI::xml::ElementEnumerator statePropertyNode = propertySetNodes->getElementEnumerator();
							while (statePropertyNode.next("Property"))
							{
								std::string name;
								if (statePropertyNode->findAttribute("name", name))
								{
									if (name == "Visible")
									{
										if (statePropertyNode->getContent() != "True")
											stateVisible = false;
									}
								}
							}
							break;
						}

						if (!stateVisible)
							continue;

						MyGUI::xml::Element* state = region->createChild("State");

						MyGUI::UString stateOffsetValue;
						MyGUI::UString textColourValue;
						MyGUI::UString textShiftValue;

						propertySetNodes = stateNodes->getElementEnumerator();
						while (propertySetNodes.next("PropertySet"))
						{
							MyGUI::xml::ElementEnumerator propertyNodes = propertySetNodes->getElementEnumerator();
							while (propertyNodes.next("Property"))
							{
								MyGUI::xml::Element* propertyNode = propertyNodes.current();

								std::string name;
								if (propertyNode->findAttribute("name", name))
								{
									if (name == "Position")
									{
										MyGUI::IntCoord coord = regionCoord + MyGUI::IntPoint::parse(propertyNode->getContent());
										stateOffsetValue = coord.print();
									}
									else if (name == "TextColour")
									{
										textColourValue = propertyNode->getContent();
									}
									else if (name == "TextShift")
									{
										textShiftValue = propertyNode->getContent();
									}
								}
							}
							break;
						}

						MyGUI::UString stateNameValue = stateNodes->findAttribute("name");
						state->addAttribute("name", convertStateName(stateNameValue));

						if (regionTypeValue == "SimpleText" || regionTypeValue == "EditText")
						{
							state->addAttribute("colour", textColourValue);
							state->addAttribute("shift", textShiftValue);
						}
						else
						{
							state->addAttribute("offset", stateOffsetValue);

							if (stateNameValue == "Normal")
							{
								if (regionTypeValue == "TileRect")
								{
									MyGUI::xml::Element* prop = state->createChild("Property");
									prop->addAttribute("key", "TileSize");
									prop->addAttribute("value", regionCoord.size().print());
									prop = state->createChild("Property");
									prop->addAttribute("key", "TileH");
									prop->addAttribute("value", MyGUI::utility::toString(tileHor));
									prop = state->createChild("Property");
									prop->addAttribute("key", "TileV");
									prop->addAttribute("value", MyGUI::utility::toString(tileVer));
								}
							}
						}
					}
					break;
				}
			}
		}
	}

	MyGUI::UString ExportManager::convertStateName(const MyGUI::UString& _value) const
	{
		if (_value == "Disabled")
			return "disabled";
		if (_value == "Normal")
			return "normal";
		if (_value == "Over")
			return "highlighted";
		if (_value == "Pressed")
			return "pushed";

		if (_value == "Selected Disabled")
			return "disabled_checked";
		if (_value == "Selected Normal")
			return "normal_checked";
		if (_value == "Selected Over")
			return "highlighted_checked";
		if (_value == "Selected Pressed")
			return "pushed_checked";
		return _value;
	}

} // namespace tools

/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SkinDataUtility.h"
#include "DataTypeManager.h"

namespace tools
{

	const SkinDataUtility::VectorString& SkinDataUtility::getRegionNames()
	{
		static VectorString names;
		if (names.empty())
		{
			names.resize(RegionMax);

			names[RegionLeftTop] = "Left Top";
			names[RegionTop] = "Top";
			names[RegionRightTop] = "Right Top";
			names[RegionLeft] = "Left";
			names[RegionCenter] = "Center";
			names[RegionRight] = "Right";
			names[RegionLeftBottom] = "Left Bottom";
			names[RegionBottom] = "Bottom";
			names[RegionRightBottom] = "Right Bottom";
		}

		return names;
	}

	void SkinDataUtility::CreateSkinData(Data* _skinData)
	{
		CreateStates(_skinData);
		CreateSeparators(_skinData);
		CreateRegions(_skinData);

		MyGUI::IntSize size = getSkinSize(_skinData);
		MyGUI::IntRect separators = getSkinSeparators(_skinData);
		VectorCoord coords = getRegions(size, separators);
		FillRegions(_skinData, coords);
	}

	void SkinDataUtility::CreateStates(Data* _skinData)
	{
		Data* state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Disabled");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Normal");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Over");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Pressed");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Disabled");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Normal");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Over");
		_skinData->addChild(state);

		state = new Data();
		state->setType(DataTypeManager::getInstance().getType("State"));
		state->setPropertyValue("Name", "Selected Pressed");
		_skinData->addChild(state);
	}

	void SkinDataUtility::CreateSeparators(Data* _skinData)
	{
		Data* separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Left");
		_skinData->addChild(separator);

		separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Top");
		_skinData->addChild(separator);

		separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Right");
		_skinData->addChild(separator);

		separator = new Data();
		separator->setType(DataTypeManager::getInstance().getType("Separator"));
		separator->setPropertyValue("Name", "Bottom");
		_skinData->addChild(separator);
	}

	void SkinDataUtility::CreateRegions(Data* _skinData)
	{
		const VectorString& names = getRegionNames();

		Data* region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionLeftTop]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionTop]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionRightTop]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionLeft]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionCenter]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionRight]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionLeftBottom]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionBottom]);
		_skinData->addChild(region);

		region = new Data();
		region->setType(DataTypeManager::getInstance().getType("Region"));
		region->setPropertyValue("Name", names[RegionRightBottom]);
		_skinData->addChild(region);
	}

	SkinDataUtility::VectorCoord SkinDataUtility::getRegions(const MyGUI::IntSize& _size, const MyGUI::IntRect& _separators)
	{
		VectorCoord result;
		result.resize(RegionMax);

		result[RegionLeftTop] = MyGUI::IntCoord(0, 0, _separators.left, _separators.top);
		result[RegionTop] = MyGUI::IntCoord(_separators.left, 0, _size.width - (_separators.left + _separators.right), _separators.top);
		result[RegionRightTop] = MyGUI::IntCoord(_size.width - _separators.right, 0, _separators.right, _separators.top);

		result[RegionLeft] = MyGUI::IntCoord(0, _separators.top, _separators.left, _size.height - (_separators.top + _separators.bottom));
		result[RegionCenter] = MyGUI::IntCoord(_separators.left, _separators.top, _size.width - (_separators.left + _separators.right), _size.height - (_separators.top + _separators.bottom));
		result[RegionRight] = MyGUI::IntCoord(_size.width - _separators.right, _separators.top, _separators.right, _size.height - (_separators.top + _separators.bottom));

		result[RegionLeftBottom] = MyGUI::IntCoord(0, _size.height - _separators.bottom, _separators.left, _separators.bottom);
		result[RegionBottom] = MyGUI::IntCoord(_separators.left, _size.height - _separators.bottom, _size.width - (_separators.left + _separators.right), _separators.bottom);
		result[RegionRightBottom] = MyGUI::IntCoord(_size.width - _separators.right, _size.height - _separators.bottom, _separators.right, _separators.bottom);

		return result;
	}

	MyGUI::IntSize SkinDataUtility::getSkinSize(Data* _skinData)
	{
		return MyGUI::IntCoord::parse(_skinData->getPropertyValue("Size")).size();
	}

	MyGUI::IntRect SkinDataUtility::getSkinSeparators(Data* _skinData)
	{
		MyGUI::IntRect result;

		for (Data::VectorData::const_iterator child = _skinData->getChilds().begin(); child != _skinData->getChilds().end(); child ++)
		{
			if ((*child)->getType()->getName() != "Separator")
				continue;

			std::string name = (*child)->getPropertyValue("Name");
			int offset = (*child)->getPropertyValue<int>("Offset");

			if (name == "Left")
				result.left = offset;
			else if (name == "Top")
				result.top = offset;
			else if (name == "Right")
				result.right = offset;
			else if (name == "Bottom")
				result.bottom = offset;
		}

		return result;
	}

	void SkinDataUtility::FillRegions(Data* _skinData, const SkinDataUtility::VectorCoord& _value)
	{
		const VectorString& names = getRegionNames();

		for (Data::VectorData::const_iterator child = _skinData->getChilds().begin(); child != _skinData->getChilds().end(); child ++)
		{
			if ((*child)->getType()->getName() != "Region")
				continue;

			std::string name = (*child)->getPropertyValue("Name");

			if (name == names[RegionLeftTop])
				(*child)->setPropertyValue("Coord", _value[RegionLeftTop]);
			else if (name == names[RegionTop])
				(*child)->setPropertyValue("Coord", _value[RegionTop]);
			else if (name == names[RegionRightTop])
				(*child)->setPropertyValue("Coord", _value[RegionRightTop]);
			else if (name == names[RegionLeft])
				(*child)->setPropertyValue("Coord", _value[RegionLeft]);
			else if (name == names[RegionCenter])
				(*child)->setPropertyValue("Coord", _value[RegionCenter]);
			else if (name == names[RegionRight])
				(*child)->setPropertyValue("Coord", _value[RegionRight]);
			else if (name == names[RegionLeftBottom])
				(*child)->setPropertyValue("Coord", _value[RegionLeftBottom]);
			else if (name == names[RegionBottom])
				(*child)->setPropertyValue("Coord", _value[RegionBottom]);
			else if (name == names[RegionRightBottom])
				(*child)->setPropertyValue("Coord", _value[RegionRightBottom]);
		}
	}

}

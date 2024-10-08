/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _eeb02a46_5598_4667_b6e5_24165ec13db3_
#define _eeb02a46_5598_4667_b6e5_24165ec13db3_

#include "Data.h"

namespace tools
{

	class SkinDataUtility
	{
	public:
		enum RegionIndex
		{
			RegionLeftTop,
			RegionTop,
			RegionRightTop,
			RegionLeft,
			RegionCenter,
			RegionRight,
			RegionLeftBottom,
			RegionBottom,
			RegionRightBottom,
			RegionMax
		};

		static void CreateSkinData(DataPtr _skinData);

		using VectorCoord = std::vector<MyGUI::IntCoord>;
		static VectorCoord getRegions(const MyGUI::IntSize& _size, const MyGUI::IntRect& _separators);

		static MyGUI::IntSize getSkinSize(DataPtr _skinData);
		static MyGUI::IntRect getSeparatorsOffset(DataPtr _skinData);

		using RectVisible = MyGUI::types::TRect<bool>;
		static RectVisible getSeparatorsVisible(DataPtr _skinData);

		using VectorString = std::vector<std::string>;
		static const VectorString& getRegionNames();

		static void fillRegionCoords(DataPtr _skinData, const VectorCoord& _value);
		static void fillRegionEnable(DataPtr _skinData, const RectVisible& _value);

		static void ShowRegions(DataPtr _skinData);

	private:
		static void CreateStates(DataPtr _skinData);
		static void CreateSeparators(DataPtr _skinData);
		static void CreateRegions(DataPtr _skinData);
	};

}

#endif

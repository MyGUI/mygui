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
			RegionLeftTop, RegionTop, RegionRightTop,
			RegionLeft, RegionCenter, RegionRight,
			RegionLeftBottom, RegionBottom, RegionRightBottom,
			RegionMax
		};

		static void CreateSkinData(Data* _skinData);

		typedef std::vector<MyGUI::IntCoord> VectorCoord;
		static VectorCoord getRegions(const MyGUI::IntSize& _size, const MyGUI::IntRect& _separators);

		static MyGUI::IntSize getSkinSize(Data* _skinData);
		static MyGUI::IntRect getSeparatorsOffset(Data* _skinData);

		typedef MyGUI::types::TRect<bool> RectVisible;
		static RectVisible getSeparatorsVisible(Data* _skinData);

		typedef std::vector<std::string> VectorString;
		static const VectorString& getRegionNames();

		static void fillRegionCoords(Data* _skinData, const VectorCoord& _value);
		static void fillRegionEnable(Data* _skinData, const RectVisible& _value);

		static void ShowRegions(Data* _skinData);

	private:
		static void CreateStates(Data* _skinData);
		static void CreateSeparators(Data* _skinData);
		static void CreateRegions(Data* _skinData);
	};

}

#endif

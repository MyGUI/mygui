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
		static void CreateSkinData(Data* _skinData);

	private:
		static void CreateStates(Data* _skinData);
		static void CreateSeparators(Data* _skinData);
		static void CreateRegions(Data* _skinData);
	};

}

#endif

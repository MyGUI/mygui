/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _8737f760_eac0_484c_8c1e_c72f6f845ae9_
#define _8737f760_eac0_484c_8c1e_c72f6f845ae9_

#include <string>
#include <vector>
#include "Data.h"
#include "sigslot.h"

namespace tools
{
	class DataManager
	{
	public:
		DataManager();
		~DataManager();

		static DataManager& getInstance();
		static DataManager* getInstancePtr();

		void initialise();
		void shutdown();

		void clear();

		Data* getRoot();

		void invalidateDatas();

		sigslot::signal0<> eventChangeData;

	private:
		static DataManager* mInstance;
		Data* mRoot;
	};
}

#endif

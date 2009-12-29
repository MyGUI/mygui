/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
	@module
*/

#ifndef __MYGUI_OGRE_DATA_MANAGER_H__
#define __MYGUI_OGRE_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class DirectXDataManager :
		public DataManager
	{
		MYGUI_INSTANCE_HEADER(DirectXDataManager);

	public:
		void initialise();
		void shutdown();

		virtual IDataStream* getData(const std::string& _name);

		virtual bool isDataExist(const std::string& _name);

		virtual const VectorString& getDataListNames(const std::string& _pattern);

		virtual const std::string& getDataPath(const std::string& _name);

	/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::string name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_DATA_MANAGER_H__

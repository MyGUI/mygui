/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#ifndef MYGUI_OGRE_NEXT_DATA_MANAGER_H_
#define MYGUI_OGRE_NEXT_DATA_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class OgreNextDataManager : public DataManager
	{
	public:
		void initialise(const std::string& _group);
		void shutdown();

		static OgreNextDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static OgreNextDataManager* getInstancePtr()
		{
			return static_cast<OgreNextDataManager*>(DataManager::getInstancePtr());
		}

		const std::string& getGroup() const
		{
			return mGroup;
		}

		IDataStream* getData(const std::string& _name) const override;

		void freeData(IDataStream* _data) override;

		bool isDataExist(const std::string& _name) const override;

		VectorString getDataListNames(const std::string& _pattern) const override;

		std::string getDataPath(const std::string& _name) const override;

		/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		VectorString getDataListNames(const std::string& _pattern, bool _fullpath) const;

	private:
		std::string mGroup;
		bool mAllGroups{false};

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_DATA_MANAGER_H_

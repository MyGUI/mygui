/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_VULKAN_DATA_MANAGER_H_
#define MYGUI_VULKAN_DATA_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class VulkanDataManager : public DataManager
	{
	public:
		void initialise();
		void shutdown();

		static VulkanDataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static VulkanDataManager* getInstancePtr()
		{
			return static_cast<VulkanDataManager*>(DataManager::getInstancePtr());
		}

		/** @see DataManager::getData(const std::string& _name) */
		IDataStream* getData(const std::string& _name) const override;

		/** @see DataManager::freeData */
		void freeData(IDataStream* _data) override;

		/** @see DataManager::isDataExist(const std::string& _name) */
		bool isDataExist(const std::string& _name) const override;

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		VectorString getDataListNames(const std::string& _pattern) const override;

		/** @see DataManager::getDataPath(const std::string& _name) */
		std::string getDataPath(const std::string& _name) const override;

		/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		struct ArchiveInfo
		{
			std::wstring name;
			bool recursive{false};
		};
		using VectorArchiveInfo = std::vector<ArchiveInfo>;
		VectorArchiveInfo mPaths;

		bool mIsInitialise{false};
	};

} // namespace MyGUI

#endif // MYGUI_VULKAN_DATA_MANAGER_H_

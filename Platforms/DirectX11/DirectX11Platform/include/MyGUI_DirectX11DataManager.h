/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/
#ifndef __MYGUI_DIRECTX11_DATA_MANAGER_H__
#define __MYGUI_DIRECTX11_DATA_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	class DirectX11DataManager :
		public DataManager
	{
	public:
		DirectX11DataManager();

		void initialise();
		void shutdown();

		static DirectX11DataManager& getInstance()
		{
			return *getInstancePtr();
		}
		static DirectX11DataManager* getInstancePtr()
		{
			return static_cast<DirectX11DataManager*>(DataManager::getInstancePtr());
		}

		/** @see DataManager::getData(const std::string& _name) */
		virtual IDataStream* getData(const std::string& _name);

		/** @see DataManager::freeData */
		virtual void freeData(IDataStream* _data);

		/** @see DataManager::isDataExist(const std::string& _name) */
		virtual bool isDataExist(const std::string& _name);

		/** @see DataManager::getDataListNames(const std::string& _pattern) */
		virtual const VectorString& getDataListNames(const std::string& _pattern);

		/** @see DataManager::getDataPath(const std::string& _name) */
		virtual const std::string& getDataPath(const std::string& _name);

		/*internal:*/
		void addResourceLocation(const std::string& _name, bool _recursive);

	private:
		struct ArhivInfo
		{
			std::wstring name;
			bool recursive;
		};
		typedef std::vector<ArhivInfo> VectorArhivInfo;
		VectorArhivInfo mPaths;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_DIRECTX11_DATA_MANAGER_H__

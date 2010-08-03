/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_MANAGER_H__
#define __SKIN_MANAGER_H__

#include "BaseManager.h"
#include "SkinItem.h"

namespace tools
{

	typedef std::vector<SkinItem*> VectorSkinItem;
	typedef MyGUI::Enumerator<VectorSkinItem> EnumeratorSkinItem;

	class SkinManager :
		public MyGUI::Singleton<SkinManager>
	{
	public:
		SkinManager();
		virtual ~SkinManager();

		void initialise();
		void shutdown();

		SkinItem* createChild(const std::string& _name);

		void destroyChild(SkinItem* _item);

		void destroyChild(const std::string& _name);

		void destroyAllChilds();

		SkinItem* getChild(const std::string& _name);

		EnumeratorSkinItem getChildsEnumerator();

	private:
		VectorSkinItem mChilds;

	};

} // namespace tools

#endif // __SKIN_MANAGER_H__

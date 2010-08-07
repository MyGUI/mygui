/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_MANAGER_H__
#define __SKIN_MANAGER_H__

#include "SkinItem.h"

namespace tools
{

	typedef std::vector<SkinItem*> VectorSkinItem;
	typedef MyGUI::Enumerator<VectorSkinItem> EnumeratorSkinItem;

	typedef MyGUI::delegates::CMultiDelegate0 EventHandle_SkinChangeSelection;

	class SkinManager :
		public MyGUI::Singleton<SkinManager>
	{
	public:
		SkinManager();
		virtual ~SkinManager();

		void initialise();
		void shutdown();

		SkinItem* createChild(const MyGUI::UString& _name);

		void destroyChild(SkinItem* _item);

		void destroyChild(const MyGUI::UString& _name);

		void destroyAllChilds();

		SkinItem* getChild(const MyGUI::UString& _name);

		EnumeratorSkinItem getChildsEnumerator();

		SkinItem* getItemSelected();
		void setItemSelected(SkinItem* _value);

		EventHandle_SkinChangeSelection eventChangeSelection;

	private:
		VectorSkinItem mChilds;
		SkinItem* mItemSelected;

	};

} // namespace tools

#endif // __SKIN_MANAGER_H__

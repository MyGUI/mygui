/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __SKIN_MANAGER_H__
#define __SKIN_MANAGER_H__

#include "SkinItem.h"
#include "ItemHolder.h"

namespace tools
{

	class SkinManager :
		public MyGUI::Singleton<SkinManager>,
		public ItemHolder<SkinItem>,
		public MyGUI::ISerializable
	{
	public:
		SkinManager();
		virtual ~SkinManager();

		void initialise();
		void shutdown();

		void clear();

		virtual void serialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
		virtual void deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version);
	};

} // namespace tools

#endif // __SKIN_MANAGER_H__

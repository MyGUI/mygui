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
		public ItemHolder<SkinItem>
	{
	public:
		SkinManager();
		virtual ~SkinManager();

		void initialise();
		void shutdown();
	};

} // namespace tools

#endif // __SKIN_MANAGER_H__

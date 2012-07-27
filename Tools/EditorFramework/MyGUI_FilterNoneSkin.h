/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _999d0868_5be5_4274_bd56_4321743a134e_
#define _999d0868_5be5_4274_bd56_4321743a134e_

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class MYGUI_EXPORT_DLL FilterNone :
		public SubSkin
	{
		MYGUI_RTTI_DERIVED( FilterNone )

	public:
		FilterNone();
		virtual ~FilterNone();

		virtual void doManualRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

	/*internal:*/
		virtual void _setAlign(const IntSize& _oldsize);
	};

}

#endif

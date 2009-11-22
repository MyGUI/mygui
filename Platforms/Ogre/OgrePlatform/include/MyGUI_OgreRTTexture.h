/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_OGRE_RTTEXTURE_H__
#define __MYGUI_OGRE_RTTEXTURE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IRenderTarget.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class OgreRTTexture :
		public IRenderTarget
	{
	public:
		OgreRTTexture(Ogre::TexturePtr _texture);
		virtual ~OgreRTTexture();

		virtual void begin();
		virtual void end();

		virtual void doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count);

		virtual const RenderTargetInfo& getInfo() { return mRenderTargetInfo; }

	private:
		RenderTargetInfo mRenderTargetInfo;
		Ogre::Viewport* mViewport;
		Ogre::Viewport* mSaveViewport;
		Ogre::TexturePtr mTexture;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_RTTEXTURE_H__

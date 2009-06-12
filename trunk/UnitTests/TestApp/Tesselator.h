/*!
	@file
	@author		Albert Semenov
	@date		06/2008
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
#ifndef __MYGUI_TESSELATOR_H__
#define __MYGUI_TESSELATOR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerNodeAnimation.h"

namespace demo
{

	class Tesselator
	{
	protected:
		Tesselator();

		size_t tesselation(
			size_t _count,
			MyGUI::VectorQuadData& _data,
			MyGUI::ITexture* _texture,
			const MyGUI::RenderTargetInfo& _info,
			const MyGUI::IntCoord& _coord);

		int getCountX() { return mCountX; }
		int getCountY() { return mCountY; }
		int getCount() { return mCountX * mCountY; }

		float getLeft() { return mLeft; }
		float getTop() { return mTop; }
		float getWidth() { return mWidth; }
		float getHeight() { return mHeight; }

	private:
		int mCountX;
		int mCountY;

		float mLeft;
		float mTop;
		float mWidth;
		float mHeight;

	};

} // namespace MyGUI

#endif // __MYGUI_TESSELATOR_H__

/*!
	@file
	@author		Albert Semenov
	@date		06/2008
	@module
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

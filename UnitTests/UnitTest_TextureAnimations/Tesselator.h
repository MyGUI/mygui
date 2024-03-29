/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/

#ifndef MYGUI_TESSELATOR_H_
#define MYGUI_TESSELATOR_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_LayerNodeAnimation.h"

namespace demo
{

	class Tesselator
	{
	protected:
		size_t tesselation(
			size_t _count,
			MyGUI::VectorQuadData& _data,
			MyGUI::ITexture* _texture,
			const MyGUI::RenderTargetInfo& _info,
			const MyGUI::IntCoord& _coord);

		int getCountX() const
		{
			return mCountX;
		}
		int getCountY() const
		{
			return mCountY;
		}
		int getCount() const
		{
			return mCountX * mCountY;
		}

		float getLeft() const
		{
			return mLeft;
		}
		float getTop() const
		{
			return mTop;
		}
		float getWidth() const
		{
			return mWidth;
		}
		float getHeight() const
		{
			return mHeight;
		}

	private:
		int mCountX{64};
		int mCountY{64};

		float mLeft{0};
		float mTop{0};
		float mWidth{0};
		float mHeight{0};
	};

} // namespace MyGUI

#endif // MYGUI_TESSELATOR_H_

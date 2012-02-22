/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/

#include "Tesselator.h"

namespace demo
{

	Tesselator::Tesselator() :
		mCountX(64),
		mCountY(64),
		mLeft(0),
		mTop(0),
		mWidth(0),
		mHeight(0)
	{
	}

	size_t Tesselator::tesselation(
		size_t _count,
		MyGUI::VectorQuadData& _data,
		MyGUI::ITexture* _texture,
		const MyGUI::RenderTargetInfo& _info,
		const MyGUI::IntCoord& _coord)
	{
		mLeft = ((_info.pixScaleX * (float)(_coord.left) + _info.hOffset) * 2) - 1;
		mTop = -(((_info.pixScaleY * (float)(_coord.top) + _info.vOffset) * 2) - 1);

		mWidth = (_info.pixScaleX * (float)_coord.width * 2);
		mHeight = -(_info.pixScaleY * (float)_coord.height * 2);

		size_t count = mCountX * mCountY;
		if (_count == count) return count;

		// запрашивам нужный размер вершин
		_data.resize(count);

		float vertex_z = _info.maximumDepth;

		float texture_u = (float)_coord.width / (float)_texture->getWidth();
		float texture_v = (float)_coord.height / (float)_texture->getHeight();

		for (int rx = 0; rx < mCountX + 1; rx++)
		{
			for (int ry = 0; ry < mCountY + 1; ry++)
			{
				MyGUI::FloatPoint point((float)rx / (float)mCountX, (float)ry / (float)mCountY);

				float fx = mLeft + mWidth * point.left;
				float fy = mTop + mHeight * point.top;

				float u = texture_u * point.left;
				float v = texture_v * point.top;

				//if (_info.rttFlipY) v = 1 - v;

				MyGUI::Vertex vertex;
				vertex.set(fx, fy, vertex_z, u, v, 0xFFFFFFFF);

				if (rx < mCountX && ry < mCountY)
				{
					_data[rx + ry* mCountX].vertex[MyGUI::QuadData::CornerLT] = vertex;
				}

				if (rx > 0 && ry > 0)
				{
					_data[(rx-1) + (ry-1)*mCountX].vertex[MyGUI::QuadData::CornerRB] = vertex;
				}

				if (rx > 0 && ry < mCountY)
				{
					_data[(rx-1) + ry* mCountX].vertex[MyGUI::QuadData::CornerRT] = vertex;
				}

				if (rx < mCountX && ry > 0)
				{
					_data[rx + (ry-1)*mCountX].vertex[MyGUI::QuadData::CornerLB] = vertex;
				}
			}
		}

		return count;
	}

} // namespace MyGUI

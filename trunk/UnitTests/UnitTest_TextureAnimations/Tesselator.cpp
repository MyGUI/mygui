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

		for (int rx=0; rx<mCountX+1; rx++)
		{
			for (int ry=0; ry<mCountY+1; ry++)
			{
				MyGUI::FloatPoint point((float)rx / (float)mCountX, (float)ry / (float)mCountY);

				float fx = mLeft + mWidth * point.left;
				float fy = mTop + mHeight * point.top;

				float u = point.left * texture_u;
				float v = point.top * texture_v;

				if (_info.rttFlipY) v = 1 - v;

				MyGUI::Vertex vertex;
				vertex.set(fx, fy, vertex_z, u, v, 0xFFFFFFFF);

				if (rx < mCountX && ry < mCountY)
				{
					_data[rx + ry*mCountX].vertex[MyGUI::QuadData::CornerLT] = vertex;
				}

				if (rx > 0 && ry > 0)
				{
					_data[(rx-1) + (ry-1)*mCountX].vertex[MyGUI::QuadData::CornerRB] = vertex;
				}

				if (rx > 0 && ry < mCountY)
				{
					_data[(rx-1) + ry*mCountX].vertex[MyGUI::QuadData::CornerRT] = vertex;
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

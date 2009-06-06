/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "CustomLayerNodeAnimation.h"

namespace demo
{

	const float WINDOW_TESSELATION = 16;
	const float DRAG_SPEED = 4;

	CustomLayerNodeAnimation::CustomLayerNodeAnimation() :
		mInertiaMode(false),
		mDragStrength( 0.001f ),
		mResizeStrength( 0.0009f )
	{
	}

	size_t CustomLayerNodeAnimation::animate(
		bool _update,
		float _time,
		MyGUI::IVertexBuffer* _buffer,
		MyGUI::ITexture* _texture,
		const MyGUI::RenderTargetInfo& _info,
		const MyGUI::IntCoord& _coord
		)
	{
		// проверяем смещения виджета
		if (mOldCoord.point() != _coord.point())
		{
			const MyGUI::IntPoint& point = MyGUI::InputManager::getInstance().getMousePosition();
			mInertiaPoint = MyGUI::FloatPoint((float)(point.left - _coord.left) / (float)_coord.width , (float)(point.top - _coord.top) / (float)_coord.height);
			mInertiaMode = false;

			addInertia( MyGUI::FloatPoint( -(_coord.left-mOldCoord.left), (_coord.top-mOldCoord.top) ) );
		}
		else if (mOldCoord.size() != _coord.size())
		{
			mInertiaMode = true;

			addInertia( MyGUI::FloatPoint( -(_coord.width - mOldCoord.width), (_coord.height-mOldCoord.height) ) );
		}

		mOldCoord = _coord;

		const int count_w = WINDOW_TESSELATION;
		const int count_h = WINDOW_TESSELATION;
		const int count = count_w * count_h * MyGUI::VertexQuad::VertexCount;

		if (true/*_update*/)
		{
			// запрашивам нужный размер вершин
			_buffer->setVertextCount(count);

			MyGUI::VertexQuad* quad = (MyGUI::VertexQuad*)_buffer->lock();

			float vertex_z = _info.maximumDepth;

			float vertex_left = ((_info.pixScaleX * (float)(_coord.left) + _info.hOffset) * 2) - 1;
			//float vertex_right = vertex_left + (_info.pixScaleX * (float)_coord.width * 2);
			float vertex_top = -(((_info.pixScaleY * (float)(_coord.top) + _info.vOffset) * 2) - 1);
			//float vertex_bottom = vertex_top - (_info.pixScaleY * (float)_coord.height * 2);

			float vertex_width = (_info.pixScaleX * (float)_coord.width * 2);
			float vertex_height = -(_info.pixScaleY * (float)_coord.height * 2);

			float texture_u = (float)_coord.width / (float)_texture->getWidth();
			float texture_v = (float)_coord.height / (float)_texture->getHeight();

			buildMesh(
				MyGUI::FloatCoord(vertex_left, vertex_top, vertex_width, vertex_height), 
				vertex_z,
				quad,
				count_w,
				count_h,
				texture_u,
				texture_v
				);

			/*tesselateQuad(
				vertex_left,
				vertex_top,// - 0.2,

				vertex_right,
				vertex_top,

				vertex_left,
				vertex_bottom,// + 0.2,

				vertex_right,
				vertex_bottom,

				vertex_z,
				
				0, 0, texture_u, texture_v,

				0xFFFFFFFF,

				quad,

				count_w,
				count_h
				);*/

			/*if (_info.rttFlipY)
			{
				quad->vertex[MyGUI::VertexQuad::CornerLT].v = quad->vertex[MyGUI::VertexQuad::CornerLB].v;
				quad->vertex[MyGUI::VertexQuad::CornerLB].v = quad->vertex[MyGUI::VertexQuad::CornerRT].v;

				quad->vertex[MyGUI::VertexQuad::CornerRT].v = quad->vertex[MyGUI::VertexQuad::CornerLT].v;
				quad->vertex[MyGUI::VertexQuad::CornerRT2].v = quad->vertex[MyGUI::VertexQuad::CornerLT].v;

				quad->vertex[MyGUI::VertexQuad::CornerRB].v = quad->vertex[MyGUI::VertexQuad::CornerLB].v;
				quad->vertex[MyGUI::VertexQuad::CornerLB2].v = quad->vertex[MyGUI::VertexQuad::CornerLB].v;
			}*/

			_buffer->unlock();

		}

		onTick(_time);
		return count;
	}

	void CustomLayerNodeAnimation::tesselateQuad(
		float _x1, float _y1,
		float _x2, float _y2,
		float _x3, float _y3,
		float _x4, float _y4,
		float _z,
		float _u1, float _v1,
		float _u2, float _v2,
		unsigned int _colour,
		MyGUI::VertexQuad * _quads,
		int _tesX, int _tesY)
	{
		Ogre::Vector2 LT(_x1, _y1);
		Ogre::Vector2 RT(_x2, _y2);
		Ogre::Vector2 RB(_x4, _y4);
		Ogre::Vector2 LB(_x3, _y3);
		Ogre::Vector2 baseDYLeft = (LB - LT)/_tesY;
		Ogre::Vector2 baseDYRight = (RB - RT)/_tesY;

		Ogre::Vector2 LTUV(_x1, _y1);

		float dXUV = (_u2 - _u1)/_tesX;
		float dYUV = (_v2 - _v1)/_tesY;

		for (int i = 0; i <= _tesX; ++i)
		{
			for (int j = 0; j <= _tesY; ++j)
			{
				Ogre::Vector2 point1 = LT + baseDYLeft * j;
				Ogre::Vector2 point2 = RT + baseDYRight * j;
				Ogre::Vector2 point = point1 + (point2 - point1)*i/_tesX;

				float u = _u1 + dXUV * i;
				float v = _v1 + dYUV * j;

				if (i < _tesX && j < _tesY)
				{
					_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].x = point.x;
					_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].y = point.y;
					_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].u = u;
					_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].v = v;
				}

				if (i > 0 && j > 0)
				{
					_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].x = point.x;
					_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].y = point.y;
					_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].u = u;
					_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].v = v;
				}

				if (i > 0 && j < _tesY)
				{
					_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].x = point.x;
					_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].y = point.y;
					_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].u = u;
					_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].v = v;
				}

				if (i < _tesX && j > 0)
				{
					_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].x = point.x;
					_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].y = point.y;
					_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].u = u;
					_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].v = v;
				}
			}
		}

		for (int i = 0; i < _tesX; ++i)
		{
			for (int j = 0; j < _tesY; ++j)
			{		
				for (int k = 0; k < 6; ++k)
				{
					_quads[i + j*_tesX].vertex[k].z = _z;
					_quads[i + j*_tesX].vertex[k].colour = _colour;

					_quads[i + j*_tesX].vertex[k].z = _z;
					_quads[i + j*_tesX].vertex[k].colour = _colour;
				}

				_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT2] = _quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT];
				_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLB2] = _quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLB];
			}
		}
	}

	float squaredDistance(const MyGUI::FloatPoint& _value, const MyGUI::FloatPoint& other)
	{
		float x1 = other.left-_value.left;
		float y1 = other.top-_value.top;

		return x1*x1 + y1*y1;				
	}

	float getLength(const MyGUI::FloatPoint& _value)
	{
		return (float)sqrt( _value.left*_value.left + _value.top*_value.top );
	}

	void setLength(MyGUI::FloatPoint& _value, float _len)
	{
		float temp = getLength(_value);
		_value.left *= _len / temp;
		_value.top *= _len / temp;
	}

	void CustomLayerNodeAnimation::addInertia( const MyGUI::FloatPoint& value )
	{
		MyGUI::FloatPoint vAmount = value;

		const float clampFactor = 50.0f;

		mInertia = mInertia + vAmount;
		if ( getLength(mInertia) > clampFactor )
			setLength(mInertia, clampFactor);
	}

	void CustomLayerNodeAnimation::onTick( float vDelta )
	{
		// Clamp delta
		vDelta = std::min( 0.05f, vDelta );

		MyGUI::FloatPoint previousdrag = mDragOffset;

		mDragOffset.left = mDragOffset.left + mInertia.left * 5.0f * DRAG_SPEED * vDelta;
		mDragOffset.top = mDragOffset.top + mInertia.top * 5.0f * DRAG_SPEED * vDelta;
		
		mInertia.left += (mInertia.left * -5 * DRAG_SPEED * vDelta);
		mInertia.left += (previousdrag.left * -4.0f * DRAG_SPEED * vDelta);

		mInertia.top += (mInertia.top * -5 * DRAG_SPEED * vDelta);
		mInertia.top += (previousdrag.top * -4.0f * DRAG_SPEED * vDelta);
	}

	void CustomLayerNodeAnimation::buildMesh(const MyGUI::FloatCoord& _coord, float _z, MyGUI::VertexQuad* _quads, int _count_w, int _count_h, float _u, float _v)
	{
		for (int rx=0; rx<_count_w+1; rx++)
		{
			for (int ry=0; ry<_count_h+1; ry++)
			{
				MyGUI::FloatPoint point((float)rx / (float)_count_w, (float)ry / (float)_count_h);

				float drageffect = 0;
				if (mInertiaMode)
				{
					float drageffect1 = squaredDistance(point, MyGUI::FloatPoint(0, 0)) * mResizeStrength;
					float drageffect2 = squaredDistance(point, MyGUI::FloatPoint(1, 1)) * mResizeStrength;

					drageffect = drageffect1 < drageffect2 ? drageffect1 : drageffect2;
				}
				else
				{
					drageffect = squaredDistance(mInertiaPoint, point) * mDragStrength;
				}

				float fx = _coord.left + _coord.width * point.left;
				float fy = _coord.top + _coord.height * point.top;

				MyGUI::FloatPoint vert(fx + mDragOffset.left * drageffect, fy + mDragOffset.top * drageffect);
				mapToQuad(rx, ry, 0, _quads, vert, point.left * _u, point.top * _v, _count_w, _count_h);
			}
		}

		unsigned int _colour = 0xFFFFFFFF;
		size_t count = 0;

		for (int i = 0; i < _count_w; ++i)
		{
			for (int j = 0; j < _count_h; ++j)
			{		
				for (int k = 0; k < 6; ++k)
				{
					_quads[count].vertex[k].z = _z;
					_quads[count].vertex[k].colour = _colour;

					_quads[count].vertex[k].z = _z;
					_quads[count].vertex[k].colour = _colour;
				}

				_quads[count].vertex[MyGUI::VertexQuad::CornerRT2] = _quads[count].vertex[MyGUI::VertexQuad::CornerRT];
				_quads[count].vertex[MyGUI::VertexQuad::CornerLB2] = _quads[count].vertex[MyGUI::VertexQuad::CornerLB];

				count++;
			}
		}

	}

	void CustomLayerNodeAnimation::mapToQuad(int i, int j, int count, MyGUI::VertexQuad * _quads, const MyGUI::FloatPoint& point, float u, float v, int _tesX, int _tesY)
	{
		unsigned int _colour = 0xFFFFFFFF;

		if (i < _tesX && j < _tesY)
		{
			_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].x = point.left;
			_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].y = point.top;
			_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].u = u;
			_quads[i + j*_tesX].vertex[MyGUI::VertexQuad::CornerLT].v = v;
		}

		if (i > 0 && j > 0)
		{
			_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].x = point.left;
			_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].y = point.top;
			_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].u = u;
			_quads[(i-1) + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerRB].v = v;
		}

		if (i > 0 && j < _tesY)
		{
			_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].x = point.left;
			_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].y = point.top;
			_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].u = u;
			_quads[(i-1) + j*_tesX].vertex[MyGUI::VertexQuad::CornerRT].v = v;
		}

		if (i < _tesX && j > 0)
		{
			_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].x = point.left;
			_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].y = point.top;
			_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].u = u;
			_quads[i + (j-1)*_tesX].vertex[MyGUI::VertexQuad::CornerLB].v = v;
		}
	}

}
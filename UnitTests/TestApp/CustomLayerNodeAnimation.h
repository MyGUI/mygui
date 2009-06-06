/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#ifndef __CUSTOM_LAYER_NODE_ANIMATION_H__
#define __CUSTOM_LAYER_NODE_ANIMATION_H__

#include <MyGUI.h>
#include "MyGUI_LayerNodeAnimation.h"
#include "MyGUI_VertexData.h"

#include "Ogre.h"

namespace demo
{

	class CustomLayerNodeAnimation : public MyGUI::LayerNodeAnimation
	{
	public:
		CustomLayerNodeAnimation();

		virtual size_t animate(
			bool _update,
			float _time,
			MyGUI::IVertexBuffer* _buffer,
			MyGUI::ITexture* _texture,
			const MyGUI::RenderTargetInfo& _info,
			const MyGUI::IntCoord& _coord
			);

	private:
		void tesselateQuad(
			float _x1, float _y1,
			float _x2, float _y2,
			float _x3, float _y3,
			float _x4, float _y4,
			float _z,
			float _u1, float _v1,
			float _u2, float _v2,
			unsigned int _colour,
			MyGUI::VertexQuad* _quads,
			int _tesX, int _tesY);

	void addInertia(const MyGUI::FloatPoint& vAmount);
	void onTick(float vDelta);
	void buildMesh(const MyGUI::FloatCoord& _coord, float _z, MyGUI::VertexQuad * _quad, int _tesX, int _tesY, float _u, float _v);
	void mapToQuad(int rx, int ry, int count, MyGUI::VertexQuad * _quad, const MyGUI::FloatPoint& vert, float u, float v, int _tesX, int _tesY);

	private:
		MyGUI::IntCoord mOldCoord;

		MyGUI::FloatPoint mInertia;
		MyGUI::FloatPoint mInertiaPoint;
		MyGUI::FloatPoint mDragOffset;

		bool mInertiaMode;
		float mDragStrength;
		float mResizeStrength;
	};

}

#endif // __CUSTOM_LAYER_NODE_ANIMATION_H__

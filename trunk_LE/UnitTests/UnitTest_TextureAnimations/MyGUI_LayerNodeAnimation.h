/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/

#ifndef __MYGUI_LAYER_NODE_ANIMATION_H__
#define __MYGUI_LAYER_NODE_ANIMATION_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_RenderTargetInfo.h"
#include "MyGUI_ISerializable.h"
#include "MyGUI_VertexData.h"

namespace MyGUI
{

	struct QuadData
	{
		enum Enum
		{
			CornerLT = 0,
			CornerRT = 1,
			CornerLB = 2,
			CornerRB = 3,
			VertexCount = 4
		};

		void set(float _l, float _t, float _r, float _b, float _z, float _u1, float _v1, float _u2, float _v2, uint32 _colour)
		{
			vertex[CornerLT].set(_l, _t, _z, _u1, _v1, _colour);
			vertex[CornerRT].set(_r, _t, _z, _u2, _v1, _colour);
			vertex[CornerLB].set(_l, _b, _z, _u1, _v2, _colour);
			vertex[CornerRB].set(_r, _b, _z, _u2, _v2, _colour);
		}

		Vertex vertex[VertexCount];
	};
	typedef std::vector<QuadData> VectorQuadData;

	class ILayerNode;

	class /*MYGUI_EXPORT */LayerNodeAnimation :
		public ISerializable
	{
		MYGUI_RTTI_DERIVED( LayerNodeAnimation )

	public:
		LayerNodeAnimation() { }
		virtual ~LayerNodeAnimation() { }

		virtual void attach(ILayerNode* _node) = 0;

		// атачится айтем
		virtual void create() = 0;

		// детачится айтем
		virtual void destroy() = 0;

		// перерисовка буфера
		virtual size_t animate(
			bool _update,
			size_t _quad_count,
			VectorQuadData& _data,
			float _time,
			IVertexBuffer* _buffer,
			ITexture* _texture,
			const RenderTargetInfo& _info,
			const IntCoord& _coord,
			bool& _isAnimate) = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_NODE_ANIMATION_H__

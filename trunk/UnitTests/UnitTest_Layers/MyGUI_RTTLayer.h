/*!
	@file
	@author		Albert Semenov
	@date		06/2008
	@module
*/

#ifndef __MYGUI_RTT_LAYER_H__
#define __MYGUI_RTT_LAYER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_OverlappedLayer.h"
#include <Ogre.h>

namespace MyGUI
{

	class RTTLayer :
		public OverlappedLayer
	{
		MYGUI_RTTI_DERIVED( RTTLayer );

	public:
		RTTLayer();
		virtual ~RTTLayer();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		virtual ILayerItem* getLayerItemByPoint(int _left, int _top);

		virtual IntPoint getPosition(int _left, int _top);

		void setEntity(const std::string& _name);
		void setMaterial(const std::string& _material);

		void setTextureSize(const IntSize& _size);
		void setTextureSize(int _width, int _height) { setTextureSize(IntSize(_width, _height)); }

	private:
		void clear();
		void updateData();
		bool isIntersectMesh(IntPoint& _point, const Ogre::Ray& _ray);
		Ogre::Vector2 getCoordByTriangle(Ogre::Vector3 _position, const Ogre::Vector3& _corner0, const Ogre::Vector3& _corner1, const Ogre::Vector3& _corner2);
		Ogre::Vector2 getCoordByRel(Ogre::Vector2 _position, const Ogre::Vector2& _corner0, const Ogre::Vector2& _corner1, const Ogre::Vector2& _corner2);

	private:
		MyGUI::ITexture* mTexture;
		std::string mEntityName;
		std::string mMaterialName;
		Ogre::TextureUnitState* mTextureUnit;
		IntPoint mOldPoint;

		Ogre::Vector2* mTextureCoords;
		Ogre::Vector3* mVertices;
		unsigned long* mIndices;
		size_t mVertexCount;
		size_t mIndexCount;

		Ogre::RaySceneQuery* mRaySceneQuery;

		IntSize mTextureSize;
		float mUScale;
		float mVScale;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_H__

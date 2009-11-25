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

		// рисует леер
		virtual void renderToTarget(IRenderTarget* _target, bool _update);

		virtual ILayerItem* getLayerItemByPoint(int _left, int _top);

		virtual IntPoint getPosition(int _left, int _top);

		void setEntity(const std::string& _name, const std::string& _material);

	private:
		void clear();
		bool isIntersectMesh(IntPoint& _point, const Ogre::Ray& _ray);
		void getCoordByTriangle(IntPoint& _point, Ogre::Vector3 _position, const Ogre::Vector3& _corner0, const Ogre::Vector3& _corner1, const Ogre::Vector3& _corner2);

	private:
		MyGUI::ITexture* mTexture;
		std::string mEntityName;

		Ogre::Vector2* mTextureCoords;
		Ogre::Vector3* mVertices;
		unsigned long* mIndices;
		size_t mVertexCount;
		size_t mIndexCount;
	};

} // namespace MyGUI

#endif // __MYGUI_RTT_LAYER_H__

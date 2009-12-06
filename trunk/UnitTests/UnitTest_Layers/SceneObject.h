/*!
	@file
	@author		Albert Semenov
	@date		12/2009
	@module
*/

#ifndef __SCENE_OBJECT_H__
#define __SCENE_OBJECT_H__

#include <Ogre.h>

namespace demo
{

	class SceneObject
	{
	public:
		SceneObject();
		virtual ~SceneObject();

	public:
		void setEntity(const std::string& _value);
		void setMaterial(const std::string& _value);
		void setSceneManager(const std::string& _value);
		void setCamera(const std::string& _value);

	protected:
		void setTextureName(const std::string& _name);

		bool pickPositionInObject(int& _x, int& _y, int _view_width, int _view_height, int _texture_width, int _texture_height);

	private:
		// Code found in Wiki: www.ogre3d.org/wiki/index.php/RetrieveVertexData
		void GetMeshInformation(
			const Ogre::MeshPtr mesh,
			size_t &vertex_count,
			Ogre::Vector3* &vertices,
			size_t &index_count,
			unsigned long* &indices,
			Ogre::Vector2* &coords,
			const Ogre::Vector3 &position,
			const Ogre::Quaternion &orient,
			const Ogre::Vector3 &scale,
			const std::string& _material);
		void clear();

		bool isIntersectMesh(int& _x, int& _y, const Ogre::Ray& _ray, int _texture_width, int _texture_height);
		Ogre::Vector2 getCoordByTriangle(Ogre::Vector3 _position, const Ogre::Vector3& _corner0, const Ogre::Vector3& _corner1, const Ogre::Vector3& _corner2);
		Ogre::Vector2 getCoordByRel(Ogre::Vector2 _position, const Ogre::Vector2& _corner0, const Ogre::Vector2& _corner1, const Ogre::Vector2& _corner2);

		void updateData();

		Ogre::SceneManager* getSceneManager();
		Ogre::Camera* getCamera();

	private:
		Ogre::Vector2* mTextureCoords;
		Ogre::Vector3* mVertices;
		unsigned long* mIndices;
		size_t mVertexCount;
		size_t mIndexCount;
		float mUScale;
		float mVScale;
		Ogre::RaySceneQuery* mRaySceneQuery;
		std::string mEntityName;
		std::string mMaterialName;
		std::string mTextureName;
		std::string mSceneManager;
		std::string mCamera;
		Ogre::TextureUnitState* mTextureUnit;
	};

} // namespace demo

#endif // __SCENE_OBJECT_H__

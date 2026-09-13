/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#ifndef SCENE_OBJECT_H_
#define SCENE_OBJECT_H_

#ifdef MYGUI_OGRE_PLATFORM

	#include <OgrePrerequisites.h>

namespace demo
{

	class SceneObject
	{
	public:
		SceneObject();
		virtual ~SceneObject();

	public:
		void setEntity(std::string_view _value);
		void setMaterial(std::string_view _value);
		void setSceneManager(std::string_view _value);
		void setCamera(std::string_view _value);

	protected:
		void setTextureName(std::string_view _name);

		bool pickPositionInObject(
			int& _x,
			int& _y,
			int _view_width,
			int _view_height,
			int _texture_width,
			int _texture_height) const;

	private:
		// Code found in Wiki: www.ogre3d.org/wiki/index.php/RetrieveVertexData
		void GetMeshInformation(
			const Ogre::MeshPtr mesh,
			size_t& vertex_count,
			Ogre::Vector3*& vertices,
			size_t& index_count,
			unsigned long*& indices,
			Ogre::Vector2*& coords,
			const Ogre::Vector3& position,
			const Ogre::Quaternion& orient,
			const Ogre::Vector3& scale,
			std::string_view _material);
		void clear();

		bool isIntersectMesh(int& _x, int& _y, const Ogre::Ray& _ray, int _texture_width, int _texture_height) const;
		Ogre::Vector2 getCoordByTriangle(
			Ogre::Vector3 _position,
			const Ogre::Vector3& _corner0,
			const Ogre::Vector3& _corner1,
			const Ogre::Vector3& _corner2) const;
		Ogre::Vector2 getCoordByRel(
			Ogre::Vector2 _position,
			const Ogre::Vector2& _corner0,
			const Ogre::Vector2& _corner1,
			const Ogre::Vector2& _corner2) const;

		void updateData();

		Ogre::SceneManager* getSceneManager() const;
		Ogre::Camera* getCamera() const;

	private:
		Ogre::Vector2* mTextureCoords{nullptr};
		Ogre::Vector3* mVertices{nullptr};
		unsigned long* mIndices{nullptr};
		size_t mVertexCount{0};
		size_t mIndexCount{0};
		float mUScale{1};
		float mVScale{1};
		mutable Ogre::RaySceneQuery* mRaySceneQuery{nullptr};
		std::string mEntityName;
		std::string mMaterialName;
		std::string mTextureName;
		std::string mSceneManager;
		std::string mCamera;
		Ogre::TextureUnitState* mTextureUnit{nullptr};
	};

} // namespace demo

#endif

#endif // SCENE_OBJECT_H_

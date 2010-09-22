/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include <MyGUI.h>
#include "SceneObject.h"

#ifdef MYGUI_OGRE_PLATFORM

namespace demo
{

	SceneObject::SceneObject() :
		mTextureCoords(nullptr),
		mVertices(nullptr),
		mIndices(nullptr),
		mVertexCount(0),
		mIndexCount(0),
		mUScale(1),
		mVScale(1),
		mRaySceneQuery(nullptr),
		mTextureUnit(nullptr)
	{
	}

	SceneObject::~SceneObject()
	{
		clear();
		if (mRaySceneQuery)
		{
			getSceneManager()->destroyQuery(mRaySceneQuery);
			mRaySceneQuery = nullptr;
		}
	}

	void SceneObject::GetMeshInformation(
		const Ogre::MeshPtr mesh,
		size_t& vertex_count,
		Ogre::Vector3* &vertices,
		size_t& index_count,
		unsigned long* &indices,
		Ogre::Vector2* &coords,
		const Ogre::Vector3& position,
		const Ogre::Quaternion& orient,
		const Ogre::Vector3& scale,
		const std::string& _material)
	{
		bool added_shared = false;
		size_t current_offset = 0;
		//size_t shared_offset = 0;
		size_t next_offset = 0;
		size_t index_offset = 0;

		vertex_count = index_count = 0;

		// Calculate how many vertices and indices we're going to need
		for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh( i );
			if (submesh->getMaterialName() != _material)
				continue;

			// We only need to add the shared vertices once
			if (submesh->useSharedVertices)
			{
				if ( !added_shared )
				{
					vertex_count += mesh->sharedVertexData->vertexCount;
					added_shared = true;
				}
			}
			else
			{
				vertex_count += submesh->vertexData->vertexCount;
			}

			// Add the indices
			index_count += submesh->indexData->indexCount;
		}


		// Allocate space for the vertices and indices
		vertices = new Ogre::Vector3[vertex_count];
		indices = new unsigned long[index_count];
		coords = new Ogre::Vector2[vertex_count];

		added_shared = false;

		// Run through the submeshes again, adding the data into the arrays
		for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
			Ogre::SubMesh* submesh = mesh->getSubMesh(i);
			if (submesh->getMaterialName() != _material)
				continue;

			Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

			if ((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
			{
				if (submesh->useSharedVertices)
				{
					added_shared = true;
					//shared_offset = current_offset;
				}

				const Ogre::VertexElement* coordElem =
					vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);

				const Ogre::VertexElement* posElem =
					vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

				Ogre::HardwareVertexBufferSharedPtr vbuf =
					vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

				unsigned char* vertex =
					static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

				// There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
				//  as second argument. So make it float, to avoid trouble when Ogre::Real will
				//  be comiled/typedefed as double:
				//      Ogre::Real* pReal;
				float* pReal;

				for ( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
				{
					posElem->baseVertexPointerToElement(vertex, &pReal);

					Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

					vertices[current_offset + j] = (orient * (pt * scale)) + position;

					posElem->baseVertexPointerToElement(vertex + coordElem->getOffset() - posElem->getOffset(), &pReal);
					Ogre::Vector2 coord(pReal[0], pReal[1]);
					coords[current_offset + j] = coord;
				}

				vbuf->unlock();
				next_offset += vertex_data->vertexCount;
			}


			Ogre::IndexData* index_data = submesh->indexData;
			size_t numTris = index_data->indexCount / 3;
			Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

			bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

			unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


			//size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

			// Ogre 1.6 patch (commenting the static_cast...) - index offsets start from 0 for each submesh
			if ( use32bitindexes )
			{
				for ( size_t k = 0; k < numTris * 3; ++k)
				{
					indices[index_offset++] = pLong[k];/*+ static_cast<unsigned long>(offset)*/
				}
			}
			else
			{
				for ( size_t k = 0; k < numTris * 3; ++k)
				{
					indices[index_offset++] = static_cast<unsigned long>(pShort[k]);/*+
						static_cast<unsigned long>(offset)*/
				}
			}

			ibuf->unlock();
			current_offset = next_offset;
		}
	}

	void SceneObject::clear()
	{
		if (mTextureCoords != nullptr)
		{
			delete mTextureCoords;
			mTextureCoords = nullptr;
		}

		if (mVertices != nullptr)
		{
			delete mVertices;
			mVertices = nullptr;
			mVertexCount = 0;
		}

		if (mIndices != nullptr)
		{
			delete mIndices;
			mIndices = nullptr;
			mIndexCount = 0;
		}
	}

	bool SceneObject::isIntersectMesh(int& _x, int& _y, const Ogre::Ray& _ray, int _texture_width, int _texture_height) const
	{
		Ogre::Real closest_distance = -1.0f;
		Ogre::Vector3 closest_result;

		// test for hitting individual triangles on the mesh
		bool new_closest_found = false;
		int index_found = 0;
		for (int i = 0; i < static_cast<int>(mIndexCount); i += 3)
		{
			// check for a hit against this triangle
			std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(_ray, mVertices[mIndices[i]],
				mVertices[mIndices[i+1]], mVertices[mIndices[i+2]], true, false);

			// if it was a hit check if its the closest
			if (hit.first)
			{
				if ((closest_distance < 0.0f) ||
					(hit.second < closest_distance))
				{
					// this is the closest so far, save it off
					closest_distance = hit.second;
					index_found = i;
					new_closest_found = true;
				}
			}
		}

		if (new_closest_found)
		{
			closest_result = _ray.getPoint(closest_distance);

			// return the result
			if (closest_distance >= 0.0f)
			{
				// raycast success
				Ogre::Vector2 point = getCoordByTriangle(closest_result, mVertices[mIndices[index_found]], mVertices[mIndices[index_found+1]], mVertices[mIndices[index_found+2]]);
				Ogre::Vector2 point2 = getCoordByRel(point, mTextureCoords[mIndices[index_found]], mTextureCoords[mIndices[index_found+1]], mTextureCoords[mIndices[index_found+2]]);

				_x = (int)(point2.x * _texture_width);
				_y = (int)(point2.y * _texture_height);

				return true;
			}
		}

		// raycast failed
		return false;
	}

	Ogre::Vector2 SceneObject::getCoordByTriangle(Ogre::Vector3 _position, const Ogre::Vector3& _corner0, const Ogre::Vector3& _corner1, const Ogre::Vector3& _corner2) const
	{
		Ogre::Vector2 result; // результат

		Ogre::Vector3 dirX = _corner1 - _corner0;
		Ogre::Vector3 dirY = _corner2 - _corner0;

		_position -= _corner0; // расстояние от начала координат (от точки 0)

		Ogre::Vector3 div = (dirX.crossProduct(dirY));
		if (div.x != 0.0)
		{
			result = Ogre::Vector2((_position.crossProduct(dirY)).x, (dirX.crossProduct(_position)).x);
			result /= div.x;
		}
		else if (div.y != 0.0)
		{
			result = Ogre::Vector2((_position.crossProduct(dirY)).y, (dirX.crossProduct(_position)).y);
			result /= div.y;
		}
		else if (div.z != 0.0)
		{
			result = Ogre::Vector2((_position.crossProduct(dirY)).z, (dirX.crossProduct(_position)).z);
			result /= div.z;
		}
		else
		{
			// пипец
		}

		return result;
	}

	Ogre::Vector2 SceneObject::getCoordByRel(Ogre::Vector2 _position, const Ogre::Vector2& _corner0, const Ogre::Vector2& _corner1, const Ogre::Vector2& _corner2) const
	{
		Ogre::Vector2 result;

		result = _corner0 + _position.x * (_corner1 - _corner0) + _position.y * (_corner2 - _corner0);

		if (mUScale != 1)
		{
			float count = 1 / mUScale; // колличество тайлов
			float x = result.x * count; // пропорцией узнаем положение
			result.x  = x + 0.5f; // смещаем на половину, чтобы центр тайла был в середине
			result.x = fmod(result.x, 1); // отбрасываем до запятой получая от 0 до 1
		}

		if (mVScale != 1)
		{
			float count = 1 / mVScale; // колличество тайлов
			float y = result.y * count; // пропорцией узнаем положение
			result.y  = y + 0.5f; // смещаем на половину, чтобы центр тайла был в середине
			result.y = fmod(result.y, 1); // отбрасываем до запятой получая от 0 до 1
		}

		return result;
	}

	void SceneObject::setEntity(const std::string& _name)
	{
		mEntityName = _name;
		updateData();
	}

	void SceneObject::setMaterial(const std::string& _material)
	{
		mMaterialName = _material;
		updateData();
	}

	void SceneObject::updateData()
	{
		clear();

		Ogre::Entity* entity = getSceneManager()->getEntity(mEntityName);
		if (entity != nullptr && !mMaterialName.empty())
		{
			mVertexCount = 0;
			mIndexCount = 0;
			GetMeshInformation(entity->getMesh(), mVertexCount, mVertices, mIndexCount, mIndices, mTextureCoords, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, Ogre::Vector3::UNIT_SCALE, mMaterialName);

			Ogre::MaterialPtr material = (Ogre::MaterialPtr)Ogre::MaterialManager::getSingleton().getByName(mMaterialName);
			if (!material.isNull())
			{
				mTextureUnit = material->getTechnique(0)->getPass(0)->getTextureUnitState("gui");
				if (mTextureUnit)
				{
					mTextureUnit->setTextureName(mTextureName);
					mUScale = mTextureUnit->getTextureUScale();
					mVScale = mTextureUnit->getTextureVScale();
				}
			}
		}
	}

	void SceneObject::setTextureName(const std::string& _name)
	{
		mTextureName = _name;

		if (mTextureUnit != nullptr)
		{
			mTextureUnit->setTextureName(mTextureName);
		}
	}

	bool SceneObject::pickPositionInObject(int& _x, int& _y, int _view_width, int _view_height, int _texture_width, int _texture_height) const
	{
		if (mRaySceneQuery == nullptr)
		{
			mRaySceneQuery = getSceneManager()->createRayQuery(Ogre::Ray());
		}

		Ogre::Ray ray = getCamera()->getCameraToViewportRay(
			_x / float(_view_width),
			_y / float(_view_height));

		mRaySceneQuery->setRay(ray);
		mRaySceneQuery->setSortByDistance(true);
		Ogre::RaySceneQueryResult& result = mRaySceneQuery->execute();
		for (Ogre::RaySceneQueryResult::iterator iter = result.begin(); iter != result.end(); ++iter)
		{
			if (iter->movable != 0)
			{
				if (iter->movable->getName() == mEntityName)
				{
					if (isIntersectMesh(_x, _y, ray, _texture_width, _texture_height))
					{
						return true;
					}
					break;
				}
			}
		}

		return false;
	}

	Ogre::SceneManager* SceneObject::getSceneManager() const
	{
		return Ogre::Root::getSingleton().getSceneManager(mSceneManager);
	}

	Ogre::Camera* SceneObject::getCamera() const
	{
		return getSceneManager()->getCamera(mCamera);
	}

	void SceneObject::setSceneManager(const std::string& _value)
	{
		mSceneManager = _value;
	}

	void SceneObject::setCamera(const std::string& _value)
	{
		mCamera = _value;
	}

} // namespace demo

#endif

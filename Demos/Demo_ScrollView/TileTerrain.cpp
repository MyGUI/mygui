/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "TileTerrain.h"

namespace terrain
{

	TileTerrain::TileTerrain() :
		m_dwSizeX(0),
		m_dwSizeY(0),
		m_SceneManager(NULL),
		m_nodeTerrain(NULL),
		m_entityTerrain(NULL),
		m_entityQuadSelect(NULL),
		m_aTerrainQuadInfo(NULL),
		m_dwOldPositionCleanQuadSelect(0),
		m_dwOldPositionCleanPointSelect(0),
		m_bIsShowSelectQuad(false),
		m_bIsShowSelectPoint(false),
		m_bbsPointSet(NULL),
		m_bIsShowGrid(false)
	{
		memset((LPVOID)m_billBoardsPoint, 0, sizeof(Billboard*) * __QUADSELECT_MAX_SIZE_SELECT);
		memset((LPVOID)m_bHeightInQuadTexture, 0, sizeof(BYTE) * __TERRAIN_COUNT_TEXTURE_THIS_QUAD);
		memset((LPVOID)mHeightMapping, 2, sizeof(BYTE) * __TERRAIN_MAX_HEIGHT_Z2);

		// индекс в картинке 'tileh.png'
		// 0
		m_bHeightInQuadTexture[0][0] = 1;
		m_bHeightInQuadTexture[1][0] = 2;
		m_bHeightInQuadTexture[2][0] = 3;
		m_bHeightInQuadTexture[3][0] = 6;
		m_bHeightInQuadTexture[4][0] = 7;
		m_bHeightInQuadTexture[5][0] = 8;

		// 1
		m_bHeightInQuadTexture[0][2] = 12;
		m_bHeightInQuadTexture[1][2] = 12;
		m_bHeightInQuadTexture[2][2] = 12;
		m_bHeightInQuadTexture[3][2] = 16;
		m_bHeightInQuadTexture[4][2] = 16;
		m_bHeightInQuadTexture[5][2] = 16;

		// 2
		m_bHeightInQuadTexture[0][9] = 4;
		m_bHeightInQuadTexture[1][9] = 4;
		m_bHeightInQuadTexture[2][9] = 4;
		m_bHeightInQuadTexture[3][9] = 9;
		m_bHeightInQuadTexture[4][9] = 13;
		m_bHeightInQuadTexture[5][9] = 17;

		// 4
		m_bHeightInQuadTexture[0][36] = 27;
		m_bHeightInQuadTexture[1][36] = 27;
		m_bHeightInQuadTexture[2][36] = 27;
		m_bHeightInQuadTexture[3][36] = 27;
		m_bHeightInQuadTexture[4][36] = 27;
		m_bHeightInQuadTexture[5][36] = 27;

		// 8
		m_bHeightInQuadTexture[0][16] = 22;
		m_bHeightInQuadTexture[1][16] = 22;
		m_bHeightInQuadTexture[2][16] = 22;
		m_bHeightInQuadTexture[3][16] = 22;
		m_bHeightInQuadTexture[4][16] = 22;
		m_bHeightInQuadTexture[5][16] = 22;

		// 9
		m_bHeightInQuadTexture[0][18] = 24;
		m_bHeightInQuadTexture[1][18] = 24;
		m_bHeightInQuadTexture[2][18] = 24;
		m_bHeightInQuadTexture[3][18] = 26;
		m_bHeightInQuadTexture[4][18] = 26;
		m_bHeightInQuadTexture[5][18] = 26;

		// 6
		m_bHeightInQuadTexture[0][33] = 23;
		m_bHeightInQuadTexture[1][33] = 23;
		m_bHeightInQuadTexture[2][33] = 23;
		m_bHeightInQuadTexture[3][33] = 25;
		m_bHeightInQuadTexture[4][33] = 25;
		m_bHeightInQuadTexture[5][33] = 25;

		// 3
		m_bHeightInQuadTexture[0][8] = 11;
		m_bHeightInQuadTexture[1][8] = 18;
		m_bHeightInQuadTexture[2][8] = 31;
		m_bHeightInQuadTexture[3][8] = 32;
		m_bHeightInQuadTexture[4][8] = 37;
		m_bHeightInQuadTexture[5][8] = 38;

		// 12
		m_bHeightInQuadTexture[0][4] = 21;
		m_bHeightInQuadTexture[1][4] = 21;
		m_bHeightInQuadTexture[2][4] = 21;
		m_bHeightInQuadTexture[3][4] = 28;
		m_bHeightInQuadTexture[4][4] = 28;
		m_bHeightInQuadTexture[5][4] = 28;

		// 13
		m_bHeightInQuadTexture[0][6] = 20;
		m_bHeightInQuadTexture[1][6] = 20;
		m_bHeightInQuadTexture[2][6] = 20;
		m_bHeightInQuadTexture[3][6] = 20;
		m_bHeightInQuadTexture[4][6] = 20;
		m_bHeightInQuadTexture[5][6] = 20;

		// 14
		m_bHeightInQuadTexture[0][1] = 29;
		m_bHeightInQuadTexture[1][1] = 29;
		m_bHeightInQuadTexture[2][1] = 29;
		m_bHeightInQuadTexture[3][1] = 29;
		m_bHeightInQuadTexture[4][1] = 29;
		m_bHeightInQuadTexture[5][1] = 29;

		// 11
		m_bHeightInQuadTexture[0][24] = 10;
		m_bHeightInQuadTexture[1][24] = 30;
		m_bHeightInQuadTexture[2][24] = 34;
		m_bHeightInQuadTexture[3][24] = 10;
		m_bHeightInQuadTexture[4][24] = 36;
		m_bHeightInQuadTexture[5][24] = 36;

		// 7
		m_bHeightInQuadTexture[0][32] = 19;
		m_bHeightInQuadTexture[1][32] = 33;
		m_bHeightInQuadTexture[2][32] = 35;
		m_bHeightInQuadTexture[3][32] = 39;
		m_bHeightInQuadTexture[4][32] = 33;
		m_bHeightInQuadTexture[5][32] = 39;

		// 5
		m_bHeightInQuadTexture[0][38] = 14;
		m_bHeightInQuadTexture[1][38] = 14;
		m_bHeightInQuadTexture[2][38] = 14;
		m_bHeightInQuadTexture[3][38] = 14;
		m_bHeightInQuadTexture[4][38] = 14;
		m_bHeightInQuadTexture[5][38] = 14;

		// 10
		m_bHeightInQuadTexture[0][25] = 15;
		m_bHeightInQuadTexture[1][25] = 15;
		m_bHeightInQuadTexture[2][25] = 15;
		m_bHeightInQuadTexture[3][25] = 15;
		m_bHeightInQuadTexture[4][25] = 15;
		m_bHeightInQuadTexture[5][25] = 15;

		// 30
		m_bHeightInQuadTexture[0][37] = 29;
		m_bHeightInQuadTexture[1][37] = 29;
		m_bHeightInQuadTexture[2][37] = 29;
		m_bHeightInQuadTexture[3][37] = 29;
		m_bHeightInQuadTexture[4][37] = 29;
		m_bHeightInQuadTexture[5][37] = 29;

		// 23
		m_bHeightInQuadTexture[0][41] = 19;
		m_bHeightInQuadTexture[1][41] = 33;
		m_bHeightInQuadTexture[2][41] = 35;
		m_bHeightInQuadTexture[3][41] = 39;
		m_bHeightInQuadTexture[4][41] = 33;
		m_bHeightInQuadTexture[5][41] = 39;

		// 27
		m_bHeightInQuadTexture[0][26] = 10;
		m_bHeightInQuadTexture[1][26] = 30;
		m_bHeightInQuadTexture[2][26] = 34;
		m_bHeightInQuadTexture[3][26] = 10;
		m_bHeightInQuadTexture[4][26] = 36;
		m_bHeightInQuadTexture[5][26] = 36;

		// 29
		m_bHeightInQuadTexture[0][22] = 20;
		m_bHeightInQuadTexture[1][22] = 20;
		m_bHeightInQuadTexture[2][22] = 20;
		m_bHeightInQuadTexture[3][22] = 20;
		m_bHeightInQuadTexture[4][22] = 20;
		m_bHeightInQuadTexture[5][22] = 20;

		mHeightMapping[0] = 0;
		mHeightMapping[1] = 1;
	}

	TileTerrain::~TileTerrain()
	{
		releaseTerrain();
	}

	bool TileTerrain::initTerrain(SceneManager *mSceneMgr)
	{
		if (m_SceneManager) return false;
		m_SceneManager = mSceneMgr;

		m_bbsPointSet = m_SceneManager->createBillboardSet("Terrain_BillBoardSet");
		m_bbsPointSet->setMaterialName("MyGame/TerrainCursorPoint");

		char nameFormat[32];
		::sprintf(nameFormat, "nodeTerrain_%p", this);
		m_nodeTerrain = m_SceneManager->getRootSceneNode()->createChildSceneNode();
		return true;
	}

	void TileTerrain::releaseTerrain() // сбрасывает все, нужна будет инициализация
	{
		if (!m_SceneManager) return;
		if (m_bbsPointSet)
		{
			for (WORD i=0; i<__QUADSELECT_MAX_SIZE_SELECT; i++)
			{
				Billboard * bb = m_billBoardsPoint[i];
				if (bb) m_bbsPointSet->removeBillboard(bb);
			}
			m_SceneManager->destroyBillboardSet(m_bbsPointSet);
		}
		
		destroyTerrain();
		try
		{
			m_SceneManager->getRootSceneNode()->removeChild(m_nodeTerrain);
		}
		catch(...)
		{
		}
		m_nodeTerrain = NULL;
		m_SceneManager = NULL;
		m_dwSizeX = 0;
		m_dwSizeY = 0;
	}

	void TileTerrain::destroyTerrain()
	{
		if (m_entityTerrain)
		{
			try
			{
				m_nodeTerrain->detachObject(m_entityTerrain);
				m_nodeTerrain->detachObject(m_entityQuadSelect);
				m_SceneManager->destroyEntity(m_entityTerrain);
				m_SceneManager->destroyEntity(m_entityQuadSelect);
				m_meshTerrain->unload();
				m_meshQuadSelect->unload();

				m_entityTerrain = 0;
				m_entityQuadSelect = 0;
				m_meshTerrain.setNull();
				m_meshQuadSelect.setNull();
			}
			catch (...)
			{
			}
			m_entityTerrain = NULL;
			if (m_aTerrainQuadInfo)
			{
				delete[] m_aTerrainQuadInfo;
				m_aTerrainQuadInfo = NULL;
			}
		}
	}

	bool TileTerrain::isVertexUp(WORD wPosX, WORD wPosY) // можно ли поднять опустить землю
	{
		if (wPosX < 2) return false;
		if (wPosY < 2) return false;
		if (wPosX >= (m_dwSizeX-1)) return false;
		if (wPosY >= (m_dwSizeY-1)) return false;

		DWORD dwOffset = (DWORD)wPosY*m_dwSizeX+(DWORD)wPosX;

		int currentHeight = m_aTerrainQuadInfo[dwOffset].getHeight();
		if (currentHeight >= __TERRAIN_MAX_HEIGHT_Z2) return false;

		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;
		dwOffset --;
		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;
		dwOffset -= m_dwSizeX;
		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;
		dwOffset ++;
		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;

		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexUp(wPosX, wPosY-1)) return false;
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexUp(wPosX-1, wPosY)) return false;
		}
		dwOffset += 2;
		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexUp(wPosX+1, wPosY)) return false;
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexUp(wPosX, wPosY+1)) return false;
		}

		return true;
	}

	bool TileTerrain::isVertexDown(WORD wPosX, WORD wPosY) // можно ли опустить землю
	{
		if (wPosX < 2) return false;
		if (wPosY < 2) return false;
		if (wPosX >= (m_dwSizeX-1)) return false;
		if (wPosY >= (m_dwSizeY-1)) return false;

		DWORD dwOffset = (DWORD)wPosY*m_dwSizeX+(DWORD)wPosX;

		int currentHeight = m_aTerrainQuadInfo[dwOffset].getHeight();
		if (currentHeight == 0) return false;

		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;
		dwOffset --;
		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;
		dwOffset -= m_dwSizeX;
		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;
		dwOffset ++;
		if (m_aTerrainQuadInfo[dwOffset].bNoModify) return false;

		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexDown(wPosX, wPosY-1)) return false;
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexDown(wPosX-1, wPosY)) return false;
		}
		dwOffset += 2;
		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexDown(wPosX+1, wPosY)) return false;
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (!isVertexDown(wPosX, wPosY+1)) return false;
		}

		return true;
	}

	bool TileTerrain::TerrainUp(WORD wPosX, WORD wPosY) // непосредственно поднятие
	{
		if (!m_entityTerrain) return false;

		if (!isVertexUp(wPosX, wPosY)) return false;

		DWORD vertex_count = 0;
		SubMesh* submesh = m_meshTerrain->getSubMesh(0);
		Ogre::IndexData* index_data = submesh->indexData;
		DWORD index_count = (DWORD)index_data->indexCount;
		Ogre::VertexData* vertex_data;
		if (submesh->useSharedVertices) vertex_data = m_meshTerrain->sharedVertexData;
		else vertex_data = submesh->vertexData;
		vertex_count = (DWORD)vertex_data->vertexCount;
		const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
		float * ptrVertex = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		m_wSmoothStartX = m_wSmoothEndX = wPosX;
		m_wSmoothStartY = m_wSmoothEndY = wPosY;

		VertexUp(wPosX, wPosY, ptrVertex); // поднятие вершин рекурсия

		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		DWORD *ptrIndex = static_cast<DWORD*>(ibuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		SmoothTerrain(ptrVertex, ptrIndex);

		vbuf->unlock();
		ibuf->unlock();

		return true;
	}

	bool TileTerrain::TerrainDown(WORD wPosX, WORD wPosY) // непосредственно опускание
	{
		if (!m_entityTerrain) return false;

		if (!isVertexDown(wPosX, wPosY)) return false;

		DWORD vertex_count = 0;
		SubMesh* submesh = m_meshTerrain->getSubMesh(0);
		Ogre::IndexData* index_data = submesh->indexData;
		DWORD index_count = (DWORD)index_data->indexCount;
		Ogre::VertexData* vertex_data;
		if (submesh->useSharedVertices) vertex_data = m_meshTerrain->sharedVertexData;
		else vertex_data = submesh->vertexData;
		vertex_count = (DWORD)vertex_data->vertexCount;
		const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
		float* ptrVertex = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		m_wSmoothStartX = m_wSmoothEndX = wPosX;
		m_wSmoothStartY = m_wSmoothEndY = wPosY;

		VertexDown(wPosX, wPosY, ptrVertex); // опускание вершин рекурсия

		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		DWORD *ptrIndex = static_cast<DWORD*>(ibuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		SmoothTerrain(ptrVertex, ptrIndex);

		vbuf->unlock();
		ibuf->unlock();

		return true;
	}

	bool TileTerrain::setQuadTexture(WORD wPosX, WORD wPosY, BYTE bNumTexture)
	{
		if (wPosX >= m_dwSizeX) return false;
		if (wPosY >= m_dwSizeY) return false;
		float fPosY = (bNumTexture / 10) * __TERRAIN_SIZE_TEXTURE_Y;
		float fPosX = (bNumTexture % 10) * __TERRAIN_SIZE_TEXTURE_X;

		DWORD vertex_count = 0;
		SubMesh* submesh = m_meshTerrain->getSubMesh(0);
		Ogre::VertexData* vertex_data;
		if (submesh->useSharedVertices) vertex_data = m_meshTerrain->sharedVertexData;
		else vertex_data = submesh->vertexData;
		vertex_count = (DWORD)vertex_data->vertexCount;
		const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
		float * ptrVertex = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		DWORD dwOffset = ((DWORD)wPosY * m_dwSizeX + (DWORD)wPosX) * __TERRAIN_SIZE_QUAD_VERTEX;

		dwOffset += 6; // texture
		ptrVertex[dwOffset] = fPosX;
		ptrVertex[dwOffset+1] = fPosY;

		dwOffset += __TERRAIN_SIZE_VERTEX;
		ptrVertex[dwOffset] = fPosX+__TERRAIN_SIZE_TEXTURE_X;
		ptrVertex[dwOffset+1] = fPosY;

		dwOffset += __TERRAIN_SIZE_VERTEX;
		fPosY += __TERRAIN_SIZE_TEXTURE_Y;
		ptrVertex[dwOffset] = fPosX;
		ptrVertex[dwOffset+1] = fPosY;

		dwOffset += __TERRAIN_SIZE_VERTEX;
		ptrVertex[dwOffset] = fPosX+__TERRAIN_SIZE_TEXTURE_X;
		ptrVertex[dwOffset+1] = fPosY;

		vbuf->unlock();

		return true;
	}

	bool TileTerrain::CreateQuadSelect() // создает сетку выделения
	{
		char nameFormat[32];
		::sprintf(nameFormat, "meshQuadSelect_%p", this);

		m_meshQuadSelect = MeshManager::getSingleton().createManual(nameFormat, "General");
		SubMesh* sub = m_meshQuadSelect->createSubMesh();

		DWORD nVertices = __QUADSELECT_MAX_SIZE_SELECT * __QUADSELECT_COUNT_QUAD_VERTEX;

		DWORD vbufCount = nVertices * __QUADSELECT_SIZE_VERTEX;
		float *vertices = (float*)(new float[vbufCount]);

		const DWORD ibufCount = __QUADSELECT_MAX_SIZE_SELECT * __QUADSELECT_COUNT_QUAD_INDEX;
		DWORD *faces = (DWORD*)(new DWORD[ibufCount]);

		DWORD index = 0;
		DWORD pos = 0;
		DWORD indexPos = 0;
		BYTE bNumTexture = 0;

		float fPos = 0.0f;
		for (DWORD num=0; num<__QUADSELECT_MAX_SIZE_SELECT; num ++)
		{
			// 1 vertex
			vertices[pos++] = fPos;
			vertices[pos++] = __QUADSELECT_OFFSET_UP_HIDE;
			vertices[pos++] = 0.0f;
			vertices[pos++] = 0.0f;
			vertices[pos++] = 0.0f;

			// 2 vertex
			vertices[pos++] = fPos + __TERRAIN_SIZE_QUAD_XY;
			vertices[pos++] = __QUADSELECT_OFFSET_UP_HIDE;
			vertices[pos++] = 0.0f;
			vertices[pos++] = __QUADSELECT_OFFSET_TEXTURE_1;
			vertices[pos++] = 0.0f;

			// 3 vertex
			vertices[pos++] = fPos;
			vertices[pos++] = __QUADSELECT_OFFSET_UP_HIDE;
			vertices[pos++] = __TERRAIN_SIZE_QUAD_XY;
			vertices[pos++] = 0.0f;
			vertices[pos++] = __QUADSELECT_OFFSET_TEXTURE_1;

			// 4 vertex
			fPos += __TERRAIN_SIZE_QUAD_XY; // внимание
			vertices[pos++] = fPos;
			vertices[pos++] = __QUADSELECT_OFFSET_UP_HIDE;
			vertices[pos++] = __TERRAIN_SIZE_QUAD_XY;
			vertices[pos++] = __QUADSELECT_OFFSET_TEXTURE_1;
			vertices[pos++] = __QUADSELECT_OFFSET_TEXTURE_1;

			// index
			faces[index++] = indexPos+1;
			faces[index++] = indexPos;
			faces[index++] = indexPos+2;
			faces[index++] = indexPos+1;
			faces[index++] = indexPos+2;
			faces[index++] = indexPos+3;
			indexPos += __QUADSELECT_COUNT_QUAD_VERTEX;//???4
		}

		m_meshQuadSelect->sharedVertexData = new VertexData();
		m_meshQuadSelect->sharedVertexData->vertexCount = nVertices;

		VertexDeclaration* decl = m_meshQuadSelect->sharedVertexData->vertexDeclaration;

		decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
		decl->addElement(0, sizeof(float)*3, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

		HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(__QUADSELECT_SIZE_VERTEX*sizeof(float), m_meshQuadSelect->sharedVertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);
		vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);

		VertexBufferBinding* bind = m_meshQuadSelect->sharedVertexData->vertexBufferBinding; 
		bind->setBinding(0, vbuf);

		HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_32BIT, ibufCount, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY);

		ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);

		sub->useSharedVertices = true;
		sub->indexData->indexBuffer = ibuf;
		sub->indexData->indexCount = ibufCount;
		sub->indexData->indexStart = 0;

		m_meshQuadSelect->load();

		delete[] faces;
		delete[] vertices;

		m_entityQuadSelect = m_SceneManager->createEntity("entityQuadSelectTerrain", nameFormat);
		m_entityQuadSelect->setCastShadows(false);
		m_entityQuadSelect->setMaterialName("MyGame/TerrainCursorQuad");

		return true;
	}

	bool TileTerrain::TestTerrainRay(WORD &wQuadX, WORD &wQuadY, Ray ray, bool bSelectQuad)
	{
		float sizeX = ((float)m_dwSizeX-1)*__TERRAIN_SIZE_QUAD_XY;
		float sizeY = ((float)m_dwSizeY-1)*__TERRAIN_SIZE_QUAD_XY;
		static float _fDopuskForTest = 7.0f; // расширение треугольника для борьбы с гранями

		Real pos = 500;
		bool bLoop = true;
		float OldHeight = -1.0;
		WORD OldPosX = -1;
		WORD OldPosY = -1;
		Vector3 vec;

		while (bLoop)
		{
			vec = ray.getPoint(pos);
			if (vec.y > 2800.0f) return false; // задрали вверх
			else if (vec.y < -10.0f) return false; // не попали

			pos += 10.0f;
			if (vec.x > 0)
			{
				if (vec.z > 0)
				{
					if (vec.x < sizeX)
					{
						if (vec.z < sizeY)
						{

							float height = (vec.y / __TERRAIN_SIZE_QUAD_Z)*__TERRAIN_SIZE_QUAD_Z;
							if (height < __TERRAIN_MAX_HEIGHT_Z2)
							{
								wQuadX = (WORD)(vec.x / __TERRAIN_SIZE_QUAD_XY);
								wQuadY = (WORD)(vec.z / __TERRAIN_SIZE_QUAD_XY);
								if (wQuadX==OldPosX)
								{
									if (wQuadY==OldPosY)
									{
										if (height==OldHeight)
										{
											continue;
										}
									}
								}
								OldHeight = height;
								OldPosX = wQuadX;
								OldPosY = wQuadY;

								DWORD offset = ((DWORD)wQuadY)*m_dwSizeX+((DWORD)wQuadX);
								bool bEnter = false;
								try
								{
									if (m_aTerrainQuadInfo[offset].getRealHeight() >= height) bEnter = true;
									else if (m_aTerrainQuadInfo[offset+1].getRealHeight() >= height) bEnter = true;
									else if (m_aTerrainQuadInfo[offset+m_dwSizeX].getRealHeight() >= height) bEnter = true;
									else if (m_aTerrainQuadInfo[offset+m_dwSizeX+1].getRealHeight() >= height) bEnter = true;
								}
								catch (...)
								{
									// на всякий случай
									_OUT("Error. Out range index in array.      bool TileTerrain::TestTerrainRay(WORD &wQuadX, WORD &wQuadY, Ray ray, bool bSelectQuad)");
									return false;
								}

								if (bEnter)
								{
									bool bIsLeft = m_aTerrainQuadInfo[offset].bIsLeft;
									float startX = ((float)wQuadX) * __TERRAIN_SIZE_QUAD_XY;
									float startY = ((float)wQuadY) * __TERRAIN_SIZE_QUAD_XY;
									Vector3 vertex0(startX-_fDopuskForTest, m_aTerrainQuadInfo[offset].getRealHeight(), startY-_fDopuskForTest);
									Vector3 vertex1(startX+__TERRAIN_SIZE_QUAD_XY+_fDopuskForTest, m_aTerrainQuadInfo[offset+1].getRealHeight(), startY-_fDopuskForTest);
									Vector3 vertex2(startX-_fDopuskForTest, m_aTerrainQuadInfo[offset+m_dwSizeX].getRealHeight(), startY+__TERRAIN_SIZE_QUAD_XY+_fDopuskForTest);
									Vector3 vertex3(startX+__TERRAIN_SIZE_QUAD_XY+_fDopuskForTest, m_aTerrainQuadInfo[offset+m_dwSizeX+1].getRealHeight(), startY+__TERRAIN_SIZE_QUAD_XY+_fDopuskForTest);
									Vector3 Point;
									bool bIsFind = false;
									if (bIsLeft)
									{
										std::pair<bool, Real> planeRes = Math::intersects(ray, vertex0, vertex1, vertex3, false, true);
										// пересечение с треугольником
										if (planeRes.first)
										{
											if (!bSelectQuad) Point = ray.getPoint(planeRes.second);
											bIsFind = true;
										}
										else
										{
											std::pair<bool, Real> planeRes = Math::intersects(ray, vertex3, vertex0, vertex2, true, false);
											// пересечение с треугольником
											if (planeRes.first)
											{
												if (!bSelectQuad) Point = ray.getPoint(planeRes.second);
												bIsFind = true;
											}
										}
									}
									else
									{
										std::pair<bool, Real> planeRes = Math::intersects(ray, vertex0, vertex1, vertex2, false, true);
										// пересечение с треугольником
										if (planeRes.first)
										{
											if (!bSelectQuad) Point = ray.getPoint(planeRes.second);
											bIsFind = true;
										}
										else
										{
											std::pair<bool, Real> planeRes = Math::intersects(ray, vertex3, vertex1, vertex2, true, false);
											// пересечение с треугольником
											if (planeRes.first)
											{
												if (!bSelectQuad) Point = ray.getPoint(planeRes.second);
												bIsFind = true;
											}
										}
									}

									if (bIsFind)
									{
										// выделять общую вешину
										if (!bSelectQuad)
										{
											if ((startX + __TERRAIN_SIZE_QUAD_HALF_XY) < Point.x)
											{
												wQuadX ++;
											}
											if ((startY + __TERRAIN_SIZE_QUAD_HALF_XY) < Point.z)
											{
												wQuadY ++;
											}
										}
										return true;
									}
								}

							}

						}
					}
				}
			 }

		};

		return false;
	}

	bool TileTerrain::showQuadSelect(WORD wStartX, WORD wStartY, WORD &wEndX, WORD &wEndY, bool bIsWhite) // показывает сетку выделения
	{
		if (wStartX == 0) return false;
		if (wStartY == 0) return false;
		if (wStartX > (m_dwSizeX-2)) return false;
		if (wStartY > (m_dwSizeY-2)) return false;

		if (!m_bIsShowSelectQuad)
		{
			m_nodeTerrain->attachObject(m_entityQuadSelect);
			m_bIsShowSelectQuad = true;
		}

		if (wEndX == 0) wEndX = 1;
		else if (wEndX > (m_dwSizeX-2)) wEndX = m_dwSizeX-2;
		if (wEndY == 0) wEndY = 1;
		else if (wEndY > (m_dwSizeY-2)) wEndY = m_dwSizeY-2;

		DWORD dwFaceOffset;
		bool bIsXInvert = false;
		bool bIsYInvert = false;

		if (wStartX > wEndX)
		{
			dwFaceOffset = wStartX;
			wStartX = wEndX;
			wEndX = (WORD)dwFaceOffset;
			bIsXInvert = true;
		}

		if (wStartY > wEndY)
		{
			dwFaceOffset = wStartY;
			wStartY = wEndY;
			wEndY = (WORD)dwFaceOffset;
			bIsYInvert = true;
		}

		WORD wSizeX = wEndX - wStartX + 1; // после проверки не использовать
		WORD wSizeY = wEndY - wStartY + 1; // после проверки не использовать

		if ((wSizeX*wSizeY)>__QUADSELECT_MAX_SIZE_SELECT)
		{
			if (wSizeX == 1)
			{
				if (bIsYInvert) wStartY = wEndY - __QUADSELECT_MAX_SIZE_SELECT+1;
				else wEndY = __QUADSELECT_MAX_SIZE_SELECT+wStartY-1;
			} else if (wSizeY == 1)
			{
				if (bIsXInvert) wStartX = wEndX - __QUADSELECT_MAX_SIZE_SELECT+1;
				else wEndX = __QUADSELECT_MAX_SIZE_SELECT+wStartX-1;
			}
			else
			{
				bool bIsLoop = true;
				do
				{
					if (wSizeX > 1)
					{
						wSizeX --;
						if ((wSizeX*wSizeY) <= __QUADSELECT_MAX_SIZE_SELECT) bIsLoop = false;
					} 
					if (bIsLoop)
					{
						if (wSizeY > 1)
						{
							wSizeY --;
							if ((wSizeX*wSizeY) <= __QUADSELECT_MAX_SIZE_SELECT) bIsLoop = false;
						}
					}
				} while (bIsLoop);

				if (bIsXInvert) wStartX = wEndX - wSizeX + 1;
				else wEndX = wStartX + wSizeX - 1;

				if (bIsYInvert) wStartY = wEndY - wSizeY + 1;
				else wEndY = wStartY + wSizeY - 1;

			}
		}

		SubMesh* submesh = m_meshQuadSelect->getSubMesh(0);
		Ogre::IndexData* index_data = submesh->indexData;
		DWORD index_count = (DWORD)index_data->indexCount;
		Ogre::VertexData* vertex_data;
		if (submesh->useSharedVertices) vertex_data = m_meshQuadSelect->sharedVertexData;
		else vertex_data = submesh->vertexData;
		DWORD vertex_count = (DWORD)vertex_data->vertexCount;
		const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
		float * ptrVertex = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		DWORD *ptrIndex = static_cast<DWORD*>(ibuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		dwFaceOffset = 0;
		DWORD dwFaceNum = 0; // смещение к первой вершине текущего квадрата
		DWORD dwOffsetVertex = 0; // смешение в буфере

		float fColor1, fColor2;
		if (bIsWhite)
		{
			fColor1 = 0.0f;
			fColor2 = __QUADSELECT_OFFSET_TEXTURE_2;
		}
		else
		{
			fColor1 = 0.5f;
			fColor2 = __QUADSELECT_OFFSET_TEXTURE_3;
		}

		for (WORD Y=wStartY; Y<=wEndY; Y++)
		{
			DWORD dwOffsetMassive = ((DWORD)Y*m_dwSizeX)+(DWORD)wStartX;

			for (WORD X=wStartX; X<=wEndX; X++)
			{
				DWORD dwOffset = dwOffsetVertex;
				float fX = X * __TERRAIN_SIZE_QUAD_XY;
				float fY = Y * __TERRAIN_SIZE_QUAD_XY;
				// vertex 1
				ptrVertex[dwOffset++] = fX;
				ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive].getRealHeight() + __QUADSELECT_OFFSET_UP;
				ptrVertex[dwOffset++] = fY;
				ptrVertex[dwOffset] = fColor1;
				dwOffset += 2;
				// vertex 2
				ptrVertex[dwOffset++] = fX + __TERRAIN_SIZE_QUAD_XY;
				ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive+1].getRealHeight() + __QUADSELECT_OFFSET_UP;
				ptrVertex[dwOffset++] = fY;
				ptrVertex[dwOffset] = fColor2;
				dwOffset += 2;
				// vertex 3
				ptrVertex[dwOffset++] = fX;
				ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive+m_dwSizeX].getRealHeight() + __QUADSELECT_OFFSET_UP;
				fY += __TERRAIN_SIZE_QUAD_XY; // внимание
				ptrVertex[dwOffset++] = fY;
				ptrVertex[dwOffset] = fColor1;
				dwOffset += 2;
				// vertex 4
				ptrVertex[dwOffset++] = fX + __TERRAIN_SIZE_QUAD_XY;
				ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive+m_dwSizeX+1].getRealHeight() + __QUADSELECT_OFFSET_UP;
				ptrVertex[dwOffset++] = fY;
				ptrVertex[dwOffset] = fColor2;

				//__________________________________________
				if (m_aTerrainQuadInfo[dwOffsetMassive].bIsLeft)
				{
					ptrIndex[dwFaceOffset+2] = dwFaceNum+3;
					ptrIndex[dwFaceOffset+3] = dwFaceNum;
				}
				else
				{
					ptrIndex[dwFaceOffset+2] = dwFaceNum+2;
					ptrIndex[dwFaceOffset+3] = dwFaceNum+1;
				}
				dwOffsetMassive ++;
				dwOffsetVertex += __QUADSELECT_SIZE_QUAD_VERTEX;
				dwFaceOffset += __QUADSELECT_COUNT_QUAD_INDEX;
				dwFaceNum += __QUADSELECT_COUNT_QUAD_VERTEX;

			}
		}

		dwFaceNum = dwOffsetVertex + 1; // смещение к Y
		// очищаем буфер от видимых вершин
		while (dwFaceNum < m_dwOldPositionCleanQuadSelect)
		{
			ptrVertex[dwFaceNum] = __QUADSELECT_OFFSET_UP_HIDE;
			dwFaceNum += __QUADSELECT_SIZE_VERTEX;
			ptrVertex[dwFaceNum] = __QUADSELECT_OFFSET_UP_HIDE;
			dwFaceNum += __QUADSELECT_SIZE_VERTEX;
			ptrVertex[dwFaceNum] = __QUADSELECT_OFFSET_UP_HIDE;
			dwFaceNum += __QUADSELECT_SIZE_VERTEX;
			ptrVertex[dwFaceNum] = __QUADSELECT_OFFSET_UP_HIDE;
			dwFaceNum += __QUADSELECT_SIZE_VERTEX;
		};

		m_dwOldPositionCleanQuadSelect = dwOffsetVertex; // запоминаем место до куда очистили

		ibuf->unlock();
		vbuf->unlock();

		if (bIsXInvert) wEndX = wStartX;
		if (bIsYInvert) wEndY = wStartY;

		return true;
	}

	bool TileTerrain::showQuadSelect(WORD wStartX, WORD wStartY, bool bIsWhite) // показывает сетку выделения
	{
		if (wStartX == 0) return false;
		if (wStartY == 0) return false;

		if (wStartX > (m_dwSizeX-2)) return false;
		if (wStartY > (m_dwSizeY-2)) return false;

		if (!m_bIsShowSelectQuad)
		{
			m_nodeTerrain->attachObject(m_entityQuadSelect);
			m_bIsShowSelectQuad = true;
		}

		SubMesh* submesh = m_meshQuadSelect->getSubMesh(0);
		Ogre::IndexData* index_data = submesh->indexData;
		DWORD index_count = (DWORD)index_data->indexCount;
		Ogre::VertexData* vertex_data;
		if (submesh->useSharedVertices) vertex_data = m_meshQuadSelect->sharedVertexData;
		else vertex_data = submesh->vertexData;
		DWORD vertex_count = (DWORD)vertex_data->vertexCount;
		const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
		float * ptrVertex = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		DWORD *ptrIndex = static_cast<DWORD*>(ibuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		float fColor1, fColor2;
		if (bIsWhite)
		{
			fColor1 = 0.0f;
			fColor2 = __QUADSELECT_OFFSET_TEXTURE_2;
		}
		else
		{
			fColor1 = 0.5f;
			fColor2 = __QUADSELECT_OFFSET_TEXTURE_3;
		}

		DWORD dwOffsetMassive = ((DWORD)wStartY*m_dwSizeX)+(DWORD)wStartX;

		DWORD dwOffset = 0;
		float fX = wStartX * __TERRAIN_SIZE_QUAD_XY;
		float fY = wStartY * __TERRAIN_SIZE_QUAD_XY;
		// vertex 1
		ptrVertex[dwOffset++] = fX;
		ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive].getRealHeight() + __QUADSELECT_OFFSET_UP;
		ptrVertex[dwOffset++] = fY;
		ptrVertex[dwOffset] = fColor1;
		dwOffset += 2;
		// vertex 2
		ptrVertex[dwOffset++] = fX + __TERRAIN_SIZE_QUAD_XY;
		ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive+1].getRealHeight() + __QUADSELECT_OFFSET_UP;
		ptrVertex[dwOffset++] = fY;
		ptrVertex[dwOffset] = fColor2;
		dwOffset += 2;
		// vertex 3
		ptrVertex[dwOffset++] = fX;
		ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive+m_dwSizeX].getRealHeight() + __QUADSELECT_OFFSET_UP;
		fY += __TERRAIN_SIZE_QUAD_XY; // внимание
		ptrVertex[dwOffset++] = fY;
		ptrVertex[dwOffset] = fColor1;
		dwOffset += 2;
		// vertex 4
		ptrVertex[dwOffset++] = fX + __TERRAIN_SIZE_QUAD_XY;
		ptrVertex[dwOffset++] = m_aTerrainQuadInfo[dwOffsetMassive+m_dwSizeX+1].getRealHeight() + __QUADSELECT_OFFSET_UP;
		ptrVertex[dwOffset++] = fY;
		ptrVertex[dwOffset] = fColor2;
		dwOffset += 2;

		//__________________________________________
		if (m_aTerrainQuadInfo[dwOffsetMassive].bIsLeft)
		{
			ptrIndex[2] = 3;
			ptrIndex[3] = 0;
		}
		else
		{
			ptrIndex[2] = 2;
			ptrIndex[3] = 1;
		}

		vertex_count = dwOffset + 1; // смещение к Y
		// очищаем буфер от видимых вершин
		while (vertex_count < m_dwOldPositionCleanQuadSelect)
		{
			ptrVertex[vertex_count] = __QUADSELECT_OFFSET_UP_HIDE;
			vertex_count += __QUADSELECT_SIZE_VERTEX;
			ptrVertex[vertex_count] = __QUADSELECT_OFFSET_UP_HIDE;
			vertex_count += __QUADSELECT_SIZE_VERTEX;
			ptrVertex[vertex_count] = __QUADSELECT_OFFSET_UP_HIDE;
			vertex_count += __QUADSELECT_SIZE_VERTEX;
			ptrVertex[vertex_count] = __QUADSELECT_OFFSET_UP_HIDE;
			vertex_count += __QUADSELECT_SIZE_VERTEX;
		};
		m_dwOldPositionCleanQuadSelect = dwOffset; // запоминаем место до куда очистили

		ibuf->unlock();
		vbuf->unlock();

		return true;
	}

	void TileTerrain::hideQuadSelect() // скрывает сетку выделения
	{
		if (m_bIsShowSelectQuad)
		{
			m_nodeTerrain->detachObject(m_entityQuadSelect);
			m_bIsShowSelectQuad = false;
		}
		if (m_bIsShowSelectPoint)
		{
			m_nodeTerrain->detachObject(m_bbsPointSet);
			m_bIsShowSelectPoint = false;
		}
	}

	bool TileTerrain::showPointSelect(WORD wStartX, WORD wStartY, bool bIsWhite) // показывает сетку выделения
	{
		if (wStartX == 0) return false;
		if (wStartY == 0) return false;
		if (wStartX > (m_dwSizeX-1)) return false;
		if (wStartY > (m_dwSizeY-1)) return false;

		if (!m_bIsShowSelectPoint)
		{
			m_nodeTerrain->attachObject(m_bbsPointSet);
			m_bIsShowSelectPoint = true;
		}

		DWORD dwOffsetMassive = ((DWORD)wStartY*m_dwSizeX)+(DWORD)wStartX;

		Billboard *bb = m_billBoardsPoint[0];
		// создаем билборд
		if (!bb)
		{
			bb = m_bbsPointSet->createBillboard((float)wStartX * __TERRAIN_SIZE_QUAD_XY, m_aTerrainQuadInfo[dwOffsetMassive].getRealHeight() + __POINTSELECT_OFFSET_UP, (float)wStartY * __TERRAIN_SIZE_QUAD_XY);
			bb->setDimensions(__POINTSELECT_SIZE_XY, __POINTSELECT_SIZE_XY);
			m_billBoardsPoint[0] = bb;
		}
		else
		{
			bb->setPosition((float)wStartX * __TERRAIN_SIZE_QUAD_XY, m_aTerrainQuadInfo[dwOffsetMassive].getRealHeight() + __POINTSELECT_OFFSET_UP, (float)wStartY * __TERRAIN_SIZE_QUAD_XY);
		}
		ColourValue colour;
		if (bIsWhite) bb->setColour(colour.White);
		else bb->setColour(colour.Red);


		dwOffsetMassive = 1;
		// очищаем буфер от видимых вершин
		while (dwOffsetMassive < m_dwOldPositionCleanPointSelect)
		{
			Billboard *bb = m_billBoardsPoint[dwOffsetMassive];
			if (bb)
			{
				m_bbsPointSet->removeBillboard(bb);
				m_billBoardsPoint[dwOffsetMassive] = NULL;
			}
			dwOffsetMassive ++;
		};

		m_dwOldPositionCleanPointSelect = 1; // запоминаем место до куда очистили

		return true;
	}

	bool TileTerrain::showPointSelect(WORD wStartX, WORD wStartY, WORD &wEndX, WORD &wEndY, bool bIsWhite) // показывает сетку выделения
	{
		if (wStartX == 0) return false;
		if (wStartY == 0) return false;
		if (wStartX > (m_dwSizeX-1)) return false;
		if (wStartY > (m_dwSizeY-1)) return false;

		if (!m_bIsShowSelectPoint)
		{
			m_nodeTerrain->attachObject(m_bbsPointSet);
			m_bIsShowSelectPoint = true;
		}

		if (wEndX == 0) wEndX = 1;
		else if (wEndX > (m_dwSizeX-1)) wEndX = m_dwSizeX-1;
		if (wEndY == 0) wEndY = 1;
		else if (wEndY > (m_dwSizeY-1)) wEndY = m_dwSizeY-1;

		DWORD dwFaceOffset;
		bool bIsXInvert = false;
		bool bIsYInvert = false;

		if (wStartX > wEndX)
		{
			dwFaceOffset = wStartX;
			wStartX = wEndX;
			wEndX = (WORD)dwFaceOffset;
			bIsXInvert = true;
		}

		if (wStartY > wEndY)
		{
			dwFaceOffset = wStartY;
			wStartY = wEndY;
			wEndY = (WORD)dwFaceOffset;
			bIsYInvert = true;
		}

		WORD wSizeX = wEndX - wStartX + 1; // после проверки не использовать
		WORD wSizeY = wEndY - wStartY + 1; // после проверки не использовать

		if ((wSizeX*wSizeY)>__QUADSELECT_MAX_SIZE_SELECT)
		{
			if (wSizeX == 1)
			{
				if (bIsYInvert) wStartY = wEndY - __QUADSELECT_MAX_SIZE_SELECT+1;
				else wEndY = __QUADSELECT_MAX_SIZE_SELECT+wStartY-1;
			}
			else if (wSizeY == 1)
			{
				if (bIsXInvert) wStartX = wEndX - __QUADSELECT_MAX_SIZE_SELECT+1;
				else wEndX = __QUADSELECT_MAX_SIZE_SELECT+wStartX-1;
			}
			else
			{
				bool bIsLoop = true;
				do
				{
					if (wSizeX > 1)
					{
						wSizeX --;
						if ((wSizeX*wSizeY) <= __QUADSELECT_MAX_SIZE_SELECT) bIsLoop = false;
					} 
					if (bIsLoop)
					{
						if (wSizeY > 1)
						{
							wSizeY --;
							if ((wSizeX*wSizeY) <= __QUADSELECT_MAX_SIZE_SELECT) bIsLoop = false;
						}
					}
				} while (bIsLoop);

				if (bIsXInvert) wStartX = wEndX - wSizeX + 1;
				else wEndX = wStartX + wSizeX - 1;

				if (bIsYInvert) wStartY = wEndY - wSizeY + 1;
				else wEndY = wStartY + wSizeY - 1;

			}
		}

		dwFaceOffset = 0; // смешение в массиве билбордов
		DWORD dwOffsetMassive = 0;

		ColourValue colour;
		if ((wStartX != wEndX) || (wStartY != wEndY)) colour = colour.Green;
		else if (bIsWhite) colour = colour.White;
		else colour = colour.Red;

		for (WORD Y=wStartY; Y<=wEndY; Y++)
		{
			dwOffsetMassive = (((DWORD)Y)*m_dwSizeX)+((DWORD)wStartX);

			for (WORD X=wStartX; X<=wEndX; X++)
			{
				Billboard *bb = m_billBoardsPoint[dwFaceOffset];
				// создаем билборд
				if (!bb)
				{
					bb = m_bbsPointSet->createBillboard(((float)X) * __TERRAIN_SIZE_QUAD_XY, m_aTerrainQuadInfo[dwOffsetMassive].getRealHeight() + __POINTSELECT_OFFSET_UP, ((float)Y) * __TERRAIN_SIZE_QUAD_XY);
					bb->setDimensions(__POINTSELECT_SIZE_XY, __POINTSELECT_SIZE_XY);
					m_billBoardsPoint[dwFaceOffset] = bb;
				}
				else
				{
					bb->setPosition(((float)X) * __TERRAIN_SIZE_QUAD_XY, m_aTerrainQuadInfo[dwOffsetMassive].getRealHeight() + __POINTSELECT_OFFSET_UP, ((float)Y) * __TERRAIN_SIZE_QUAD_XY);
				}

				bb->setColour(colour);

				dwOffsetMassive ++;
				dwFaceOffset ++;

			}
		}

		dwOffsetMassive = dwFaceOffset;
		// очищаем буфер от видимых вершин
		while (dwOffsetMassive < m_dwOldPositionCleanPointSelect)
		{
			Billboard *bb = m_billBoardsPoint[dwOffsetMassive];
			if (bb)
			{
				m_bbsPointSet->removeBillboard(bb);
				m_billBoardsPoint[dwOffsetMassive] = NULL;
			}
			dwOffsetMassive ++;
		};

		m_dwOldPositionCleanPointSelect = dwFaceOffset; // запоминаем место до куда очистили

		if (bIsXInvert) wEndX = wStartX;
		if (bIsYInvert) wEndY = wStartY;

		return true;
	}

	void  TileTerrain::VertexUp(WORD wPosX, WORD wPosY, float * ptrVertex)
	{
		DWORD dwOffset = (DWORD)wPosY*m_dwSizeX+(DWORD)wPosX;

		int currentHeight = m_aTerrainQuadInfo[dwOffset].getHeight();

		dwOffset -= m_dwSizeX;
		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothStartY > (wPosY-1)) m_wSmoothStartY = wPosY-1;
			VertexUp(wPosX, wPosY-1, ptrVertex);
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothStartX > (wPosX-1)) m_wSmoothStartX = wPosX-1;
			VertexUp(wPosX-1, wPosY, ptrVertex);
		}
		dwOffset += 2;
		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothEndX < (wPosX+1)) m_wSmoothEndX = wPosX+1;
			VertexUp(wPosX+1, wPosY, ptrVertex);
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight > m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothEndY < (wPosY+1)) m_wSmoothEndY = wPosY+1;
			VertexUp(wPosX, wPosY+1, ptrVertex);
		}

		dwOffset = (DWORD)wPosY*m_dwSizeX+(DWORD)wPosX;
		m_aTerrainQuadInfo[dwOffset].setHeight(currentHeight + 1);
		float fCurrentHeight = m_aTerrainQuadInfo[dwOffset].getRealHeight();

		dwOffset = (((DWORD)wPosY*m_dwSizeX)+(DWORD)wPosX) * __TERRAIN_SIZE_QUAD_VERTEX;

		dwOffset ++; // Y
		ptrVertex[dwOffset] = fCurrentHeight;
		dwOffset -= __TERRAIN_OFFSET_VERTEX1;
		ptrVertex[dwOffset] = fCurrentHeight;
		dwOffset -= __TERRAIN_OFFSET_VERTEX2;
		ptrVertex[dwOffset] = fCurrentHeight;
		dwOffset += __TERRAIN_OFFSET_VERTEX1;
		ptrVertex[dwOffset] = fCurrentHeight;

	}

	void  TileTerrain::VertexDown(WORD wPosX, WORD wPosY, float * ptrVertex)
	{
		DWORD dwOffset = (DWORD)wPosY*m_dwSizeX+(DWORD)wPosX;

		int currentHeight = m_aTerrainQuadInfo[dwOffset].getHeight();

		dwOffset -= m_dwSizeX;
		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothStartY > (wPosY-1)) m_wSmoothStartY = wPosY-1;
			VertexDown(wPosX, wPosY-1, ptrVertex);
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothStartX > (wPosX-1)) m_wSmoothStartX = wPosX-1;
			VertexDown(wPosX-1, wPosY, ptrVertex);
		}
		dwOffset += 2;
		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothEndX < (wPosX+1)) m_wSmoothEndX = wPosX+1;
			VertexDown(wPosX+1, wPosY, ptrVertex);
		}
		dwOffset += (m_dwSizeX - 1);
		if (currentHeight < m_aTerrainQuadInfo[dwOffset].getHeight())
		{
			if (m_wSmoothEndY < (wPosY+1)) m_wSmoothEndY = wPosY+1;
			VertexDown(wPosX, wPosY+1, ptrVertex);
		}

		dwOffset = (DWORD)wPosY*m_dwSizeX+(DWORD)wPosX;
		m_aTerrainQuadInfo[dwOffset].setHeight(currentHeight - 1);

		float fCurrentHeight = m_aTerrainQuadInfo[dwOffset].getRealHeight();

		dwOffset = (((DWORD)wPosY*m_dwSizeX)+(DWORD)wPosX) * __TERRAIN_SIZE_QUAD_VERTEX;

		dwOffset ++; // Y
		ptrVertex[dwOffset] = fCurrentHeight;
		dwOffset -= __TERRAIN_OFFSET_VERTEX1;
		ptrVertex[dwOffset] = fCurrentHeight;
		dwOffset -= __TERRAIN_OFFSET_VERTEX2;
		ptrVertex[dwOffset] = fCurrentHeight;
		dwOffset += __TERRAIN_OFFSET_VERTEX1;
		ptrVertex[dwOffset] = fCurrentHeight;

	}

	bool TileTerrain::TerrainEqviv(WORD wStartX, WORD wStartY, WORD wEndX, WORD wEndY) // непосредственно выравнивание
	{
		if (!m_entityTerrain) return false;

		if (wStartX == 0) return false;
		if (wStartY == 0) return false;
		if (wStartX > (m_dwSizeX-1)) return false;
		if (wStartY > (m_dwSizeY-1)) return false;

		if (wEndX == 0) wEndX = 1;
		else if (wEndX > (m_dwSizeX-1)) wEndX = m_dwSizeX-1;
		if (wEndY == 0) wEndY = 1;
		else if (wEndY > (m_dwSizeY-1)) wEndY = m_dwSizeY-1;

		int height = m_aTerrainQuadInfo[(DWORD)wStartY*m_dwSizeX+(DWORD)wStartX].getHeight();
		DWORD dwOffset;

		if (wStartX > wEndX)
		{
			dwOffset = wStartX;
			wStartX = wEndX;
			wEndX = (WORD)dwOffset;
		}

		if (wStartY > wEndY)
		{
			dwOffset = wStartY;
			wStartY = wEndY;
			wEndY = (WORD)dwOffset;
		}


		DWORD vertex_count = 0;
		SubMesh* submesh = m_meshTerrain->getSubMesh(0);
		Ogre::IndexData* index_data = submesh->indexData;
		DWORD index_count = (DWORD)index_data->indexCount;
		Ogre::VertexData* vertex_data;
		if (submesh->useSharedVertices) vertex_data = m_meshTerrain->sharedVertexData;
		else vertex_data = submesh->vertexData;
		vertex_count = (DWORD)vertex_data->vertexCount;
		const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
		float* ptrVertex = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		m_wSmoothStartX = wStartX;
		m_wSmoothStartY = wStartY;
		m_wSmoothEndX = wEndX;
		m_wSmoothEndY = wEndY;

		for (WORD Y=wStartY; Y<=wEndY; Y++)
		{
			for (WORD X=wStartX; X<=wEndX; X++)
			{
				dwOffset = (DWORD)Y*m_dwSizeX+(DWORD)X;
				bool bIsLoop = true;

				do
				{
					int currentHeight = m_aTerrainQuadInfo[dwOffset].getHeight();

					if (height > currentHeight)
					{
						if (!isVertexUp(X, Y)) bIsLoop = false;
						else VertexUp(X, Y, ptrVertex); // поднятие вершин рекурсия
					}
					else if (height < currentHeight)
					{
						if (!isVertexDown(X, Y)) bIsLoop = false;
						else VertexDown(X, Y, ptrVertex); // рекурсия
					}
					else
					{
						bIsLoop = false;
					}
				}
				while (bIsLoop);

			}
		}


		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		DWORD *ptrIndex = static_cast<DWORD*>(ibuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		SmoothTerrain(ptrVertex, ptrIndex);

		vbuf->unlock();
		ibuf->unlock();

		return true;
	}

	bool TileTerrain::createTerrain(WORD wSizeX, WORD wSizeY)
	{
		if (!m_SceneManager) return false;
		if (m_entityTerrain) destroyTerrain();

		if (wSizeX < 2) return false;
		if (wSizeY < 2) return false;

		m_dwSizeX = wSizeX;
		m_dwSizeY = wSizeY;
		__TERRAIN_OFFSET_VERTEX2 = (__TERRAIN_SIZE_QUAD_VERTEX*m_dwSizeX)-(__TERRAIN_SIZE_VERTEX*2);

		if (m_aTerrainQuadInfo) delete[] m_aTerrainQuadInfo;
		DWORD tmp = m_dwSizeX*m_dwSizeY;
		m_aTerrainQuadInfo = new TerrainTileInfo[tmp];
		memset((void*)m_aTerrainQuadInfo, 0, sizeof(TerrainTileInfo)*tmp);

		char nameFormat[32];
		::sprintf(nameFormat, "meshTerrain_%p", this);

		m_meshTerrain = MeshManager::getSingleton().createManual(nameFormat, "General");
		SubMesh* sub = m_meshTerrain->createSubMesh();

		DWORD nVertices = m_dwSizeX * m_dwSizeY * __TERRAIN_COUNT_QUAD_VERTEX;

		DWORD vbufCount = nVertices * __TERRAIN_SIZE_VERTEX;
		float *vertices = (float*)(new float[vbufCount]);

		const DWORD ibufCount = m_dwSizeX * m_dwSizeY * __TERRAIN_COUNT_QUAD_INDEX;
		DWORD *faces = (DWORD*)(new DWORD[ibufCount]);

		DWORD index = 0;
		DWORD pos = 0;
		DWORD indexPos = 0;
		BYTE bNumTexture = 0;
		for (DWORD Y=0; Y<m_dwSizeY; Y ++)
		{
			for (DWORD X=0; X<m_dwSizeX; X ++)
			{

	#ifdef __SMOOTH_RANDOM
				bNumTexture = 1;//(BYTE)(::rand()%5);
	#else
				bNumTexture = 0;
	#endif

				float tmpX = X * __TERRAIN_SIZE_QUAD_XY;
				float tmpY = Y * __TERRAIN_SIZE_QUAD_XY;
				// 1 vertex
				vertices[pos++] = tmpX;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((X==0) || (Y == 0))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}

				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				// 2 vertex
				vertices[pos++] = tmpX + __TERRAIN_SIZE_QUAD_XY;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((Y == 0) || (X == (m_dwSizeX-1)))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}
				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X+__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = 0.0;
				vertices[pos++] = 1.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_X;
				vertices[pos++] = 0.0;
				// 3 vertex
				tmpY += __TERRAIN_SIZE_QUAD_XY;
				vertices[pos++] = tmpX;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((X == 0) || (Y == (m_dwSizeY-1)))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}

				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE_Y;
				vertices[pos++] = 0.0;
				vertices[pos++] = 1.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_Y;
				// 4 vertex
				vertices[pos++] = tmpX + __TERRAIN_SIZE_QUAD_XY;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((Y == (m_dwSizeY-1)) || (X == (m_dwSizeX-1)))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}

				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X+__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE_Y;
				vertices[pos++] = 1.0;
				vertices[pos++] = 1.0;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_X;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_Y;

				// index
				faces[index++] = indexPos+1;
				faces[index++] = indexPos;
				if (((X==0) && (Y==0)) || ((X==(m_dwSizeX-1))&&(Y==(m_dwSizeY-1)))){
					faces[index++] = indexPos+3;//???
					faces[index++] = indexPos;//???
				}
				else
				{
					faces[index++] = indexPos+2;
					faces[index++] = indexPos+1;
				}			
				faces[index++] = indexPos+2;
				faces[index++] = indexPos+3;
				indexPos += __TERRAIN_COUNT_QUAD_VERTEX;//???4

			}
		}

	#ifdef __SMOOTH_RANDOM
		m_wSmoothStartX = 1;
		m_wSmoothStartY = 1;
		m_wSmoothEndX = m_dwSizeX-1;
		m_wSmoothEndY = m_dwSizeY-1;
		SmoothTerrain(vertices, faces);
	#endif

		m_meshTerrain->sharedVertexData = new VertexData();
		m_meshTerrain->sharedVertexData->vertexCount = nVertices;

		VertexDeclaration* decl = m_meshTerrain->sharedVertexData->vertexDeclaration;

		decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
		decl->addElement(0, sizeof(float)*3, VET_FLOAT3, VES_NORMAL);
		decl->addElement(0, sizeof(float)*6, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
		decl->addElement(0, sizeof(float)*8, VET_FLOAT2, VES_TEXTURE_COORDINATES, 1);
		decl->addElement(0, sizeof(float)*10, VET_FLOAT2, VES_TEXTURE_COORDINATES, 2);

		HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(__TERRAIN_SIZE_VERTEX*sizeof(float), m_meshTerrain->sharedVertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC );//HBU_STATIC_WRITE_ONLY);
		vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);

		VertexBufferBinding* bind = m_meshTerrain->sharedVertexData->vertexBufferBinding; 
		bind->setBinding(0, vbuf);

		HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_32BIT, ibufCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);

		sub->useSharedVertices = true;
		sub->indexData->indexBuffer = ibuf;
		sub->indexData->indexCount = ibufCount;
		sub->indexData->indexStart = 0;

		m_meshTerrain->_setBounds(AxisAlignedBox(0,0,0, m_dwSizeX*__TERRAIN_SIZE_QUAD_XY,__TERRAIN_MAX_HEIGHT_Z2,m_dwSizeY*__TERRAIN_SIZE_QUAD_XY));
		m_meshTerrain->_setBoundingSphereRadius(Math::Sqrt(3*m_dwSizeX*__TERRAIN_SIZE_QUAD_XY*m_dwSizeX*__TERRAIN_SIZE_QUAD_XY));

		m_meshTerrain->load();

		delete[] faces;
		delete[] vertices;

		m_entityTerrain = m_SceneManager->createEntity("entityTerrain", nameFormat);
		m_entityTerrain->setMaterialName(__NAME_MATERIAL_TERRAIN_MAIN);

		m_entityTerrain->setQueryFlags(__TERRAIN_MASK_RAY);
		//m_entityTerrain->setCastShadows(false);

		m_nodeTerrain->attachObject(m_entityTerrain);
		//m_nodeTerrain->showBoundingBox( true );

		m_meshTerrain->load();
		//m_meshQuadSelect->load();
		
		CreateQuadSelect();

		return true;
	}

	bool TileTerrain::setQuadTexture3(WORD wPosX, WORD wPosY, BYTE bNumTexture)
	{
		if (wPosX >= m_dwSizeX) return false;
		if (wPosY >= m_dwSizeY) return false;
		float fPosX = (float)(bNumTexture % 10) * __TERRAIN_SIZE_TEXTURE3_X;
		float fPosY = (float)(bNumTexture / 10) * __TERRAIN_SIZE_TEXTURE3_Y;

		DWORD vertex_count = 0;
		SubMesh* submesh = m_meshTerrain->getSubMesh(0);
		Ogre::VertexData* vertex_data;
		if (submesh->useSharedVertices) vertex_data = m_meshTerrain->sharedVertexData;
		else vertex_data = submesh->vertexData;
		vertex_count = (DWORD)vertex_data->vertexCount;
		const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
		Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
		float * ptrVertex = static_cast<float*>(vbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));

		DWORD dwOffset = ((DWORD)wPosY * m_dwSizeX + (DWORD)wPosX) * __TERRAIN_SIZE_QUAD_VERTEX;

		dwOffset += 10; // texture
		ptrVertex[dwOffset] = fPosX;
		ptrVertex[dwOffset+1] = fPosY;

		dwOffset += __TERRAIN_SIZE_VERTEX;
		ptrVertex[dwOffset] = fPosX+__TERRAIN_SIZE_TEXTURE3_X;
		ptrVertex[dwOffset+1] = fPosY;

		dwOffset += __TERRAIN_SIZE_VERTEX;
		fPosY += __TERRAIN_SIZE_TEXTURE3_Y;
		ptrVertex[dwOffset] = fPosX;
		ptrVertex[dwOffset+1] = fPosY;

		dwOffset += __TERRAIN_SIZE_VERTEX;
		ptrVertex[dwOffset] = fPosX+__TERRAIN_SIZE_TEXTURE3_X;
		ptrVertex[dwOffset+1] = fPosY;

		vbuf->unlock();

		return true;
	}

	void TileTerrain::SmoothTerrain(float *ptrVertex, DWORD *ptrIndex) // сглаживает нормали и напрвления треугольников
	{

		if (m_wSmoothStartX > 1) m_wSmoothStartX --;
		else m_wSmoothStartX = 1;
		if (m_wSmoothStartY > 1) m_wSmoothStartY --;
		else m_wSmoothStartY = 1;

		m_wSmoothEndX += 2;
		m_wSmoothEndY += 2;

		if (m_wSmoothEndX > (m_dwSizeX-1)) m_wSmoothEndX = m_dwSizeX-1;
		if (m_wSmoothEndY > (m_dwSizeY-1)) m_wSmoothEndY = m_dwSizeY-1;

		for (WORD y=m_wSmoothStartY; y<m_wSmoothEndY; y++)
		{
			DWORD dwOffset1 = (DWORD)(y-1)*m_dwSizeX+m_wSmoothStartX-1; // начало строки
			DWORD dwOffset2 = dwOffset1 + m_dwSizeX;
			DWORD dwOffset3 = dwOffset2 + m_dwSizeX;

			//   0---1---2
			//   |     |    |
			//   3---4---5
			//   |     |    |
			//   6---7---8

			float height0 = m_aTerrainQuadInfo[dwOffset1++].getRealHeight();
			float height1 = m_aTerrainQuadInfo[dwOffset1++].getRealHeight();
			float height2 = m_aTerrainQuadInfo[dwOffset1++].getRealHeight();

			float height3 = m_aTerrainQuadInfo[dwOffset2++].getRealHeight();
			float height4 = m_aTerrainQuadInfo[dwOffset2].getRealHeight();
			BYTE bHeight0 = m_aTerrainQuadInfo[dwOffset2++].getHeight();
			float height5 = m_aTerrainQuadInfo[dwOffset2].getRealHeight();
			BYTE bHeight1 = m_aTerrainQuadInfo[dwOffset2++].getHeight();

			float height6 = m_aTerrainQuadInfo[dwOffset3++].getRealHeight();
			float height7 = m_aTerrainQuadInfo[dwOffset3].getRealHeight();
			BYTE bHeight3 = m_aTerrainQuadInfo[dwOffset3++].getHeight();
			float height8 = m_aTerrainQuadInfo[dwOffset3].getRealHeight();
			BYTE bHeight2 = m_aTerrainQuadInfo[dwOffset3++].getHeight();

			for (WORD x=m_wSmoothStartX; x<m_wSmoothEndX; x++)
			{
				//__вычисление_индексов_________________________________
				DWORD dwRealPos = dwOffset2-2;
				bool bIsLeft = m_aTerrainQuadInfo[dwRealPos].bIsLeft;
				bool bIsRight = true;
				if (height5 != height7) bIsRight = false;
				else if ((height5 > height4) && (height5 > height8)) bIsRight = false;

				DWORD dwOffsetVertex;
				DWORD dwOffsetIndex;
				if (bIsLeft == bIsRight)
				{
					dwOffsetIndex = dwRealPos*6;
					dwOffsetVertex = dwRealPos * __TERRAIN_COUNT_QUAD_VERTEX;
					if (!bIsRight)
					{
						ptrIndex[dwOffsetIndex+2] = dwOffsetVertex+3;
						ptrIndex[dwOffsetIndex+3] = dwOffsetVertex;
					}
					else
					{
						ptrIndex[dwOffsetIndex+2] = dwOffsetVertex+2;
						ptrIndex[dwOffsetIndex+3] = dwOffsetVertex+1;
					}
					m_aTerrainQuadInfo[dwRealPos].bIsLeft = !bIsRight;
				}

				//___вычесление_нормалей_______________________________________
				Plane plane;

	#ifdef __SMOOTH_RANDOM
				Vector3 QuadRnd(((float)((rand()%9)-4))*0.09, ((float)((rand()%9)-4))*0.09, ((float)((rand()%9)-4))*0.09);
	#else
				Vector3 QuadRnd(0.0, 0.0, 0.0);
	#endif

				// первая клетка
				if (m_aTerrainQuadInfo[dwOffset1-3].bIsLeft)
				{
					plane.redefine(Vector3(0.0, height0, 0.0), Vector3(0.0, height3, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height4, __TERRAIN_SIZE_QUAD_XY));
					QuadRnd += plane.normal;
					plane.redefine(Vector3(0.0, height0, 0.0), Vector3(__TERRAIN_SIZE_QUAD_XY, height4, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height1, 0.0));
					QuadRnd += plane.normal;
				}
				else
				{
					plane.redefine(Vector3(__TERRAIN_SIZE_QUAD_XY, height1, 0.0), Vector3(0.0, height3, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height4, __TERRAIN_SIZE_QUAD_XY));
					QuadRnd += plane.normal;
				}

				// вторая клетка
				if (m_aTerrainQuadInfo[dwOffset1-2].bIsLeft)
				{
					plane.redefine(Vector3(0.0, height1, 0.0), Vector3(0.0, height4, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height5, __TERRAIN_SIZE_QUAD_XY));
					QuadRnd += plane.normal;
				}
				else
				{
					plane.redefine(Vector3(0.0, height1, 0.0), Vector3(0.0, height4, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height2, 0.0));
					QuadRnd += plane.normal;
					plane.redefine(Vector3(__TERRAIN_SIZE_QUAD_XY, height2, 0.0), Vector3(0.0, height4, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height5, __TERRAIN_SIZE_QUAD_XY));
					QuadRnd += plane.normal;
				}

				// третья клетка
				if (m_aTerrainQuadInfo[dwRealPos-1].bIsLeft)
				{
					plane.redefine(Vector3(0.0, height3, 0.0), Vector3(__TERRAIN_SIZE_QUAD_XY, height7, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height4, 0.0));
					QuadRnd += plane.normal;
				}
				else
				{
					plane.redefine(Vector3(0.0, height3, 0.0), Vector3(0.0, height6, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height4, 0.0));
					QuadRnd += plane.normal;
					plane.redefine(Vector3(__TERRAIN_SIZE_QUAD_XY, height4, 0.0), Vector3(0.0, height6, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height7, __TERRAIN_SIZE_QUAD_XY));
					QuadRnd += plane.normal;
				}

				// четвертая клетка
				if (m_aTerrainQuadInfo[dwRealPos].bIsLeft)
				{
					plane.redefine(Vector3(0.0, height4, 0.0), Vector3(0.0, height7, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height8, __TERRAIN_SIZE_QUAD_XY));
					QuadRnd += plane.normal;
					plane.redefine(Vector3(0.0, height4, 0.0), Vector3(__TERRAIN_SIZE_QUAD_XY, height8, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height5, 0.0));
					QuadRnd += plane.normal;
				}
				else
				{
					plane.redefine(Vector3(0.0, height4, 0.0), Vector3(0.0, height7, __TERRAIN_SIZE_QUAD_XY), Vector3(__TERRAIN_SIZE_QUAD_XY, height5, 0.0));
					QuadRnd += plane.normal;
				}

				QuadRnd.normalise();

				dwOffsetVertex = dwRealPos * __TERRAIN_SIZE_QUAD_VERTEX + 3;
				*((Vector3*)&ptrVertex[dwOffsetVertex]) = QuadRnd;
				dwOffsetVertex -= __TERRAIN_OFFSET_VERTEX1;
				*((Vector3*)&ptrVertex[dwOffsetVertex]) = QuadRnd;
				dwOffsetVertex -= __TERRAIN_OFFSET_VERTEX2;
				*((Vector3*)&ptrVertex[dwOffsetVertex]) = QuadRnd;
				dwOffsetVertex += __TERRAIN_OFFSET_VERTEX1;
				*((Vector3*)&ptrVertex[dwOffsetVertex]) = QuadRnd;

				//__вычисление_текстур___________________________________

				BYTE bIndex = 0;
				if (bHeight1 > bHeight0) bIndex |= 0x20; // 100000
				else if (bHeight1 < bHeight0) bIndex |= 0x10; // 010000
				if (bHeight2 > bHeight1) bIndex |= 0x08; // 001000
				else if (bHeight2 < bHeight1) bIndex |= 0x04; // 000100
				if (bHeight3 > bHeight2) bIndex |= 0x02; // 000010
				else if (bHeight3 < bHeight2) bIndex |= 0x01; // 000001

				BYTE tile_height = bHeight0;
				if (bHeight1 < tile_height) tile_height = bHeight1;
				if (bHeight2 < tile_height) tile_height = bHeight2;
				if (bHeight3 < tile_height) tile_height = bHeight3;


	#ifdef __SMOOTH_RANDOM
				BYTE bHeightVariant1 = (BYTE)(::rand()%6);
	#else
				BYTE bHeightVariant1 = 0;
	#endif
				bIndex = m_bHeightInQuadTexture[bHeightVariant1][bIndex];

				BYTE bPosX = bIndex % 10;
				BYTE bPosY = bIndex / 10; // ??
				assert(bPosY < 4);
				bPosY += mHeightMapping[tile_height] * 4;

				float fPosX = (float)bPosX*__TERRAIN_SIZE_TEXTURE_X;
				float fPosY = (float)bPosY*__TERRAIN_SIZE_TEXTURE_Y;

				//__непосредственная_запись_текстуры____________
				dwOffsetVertex = dwRealPos * __TERRAIN_SIZE_QUAD_VERTEX + 6;

				ptrVertex[dwOffsetVertex] = fPosX;
				ptrVertex[dwOffsetVertex+1] = fPosY;

				dwOffsetVertex += __TERRAIN_SIZE_VERTEX;
				ptrVertex[dwOffsetVertex] = fPosX+__TERRAIN_SIZE_TEXTURE_X;
				ptrVertex[dwOffsetVertex+1] = fPosY;

				dwOffsetVertex += __TERRAIN_SIZE_VERTEX;
				fPosY += __TERRAIN_SIZE_TEXTURE_Y;
				ptrVertex[dwOffsetVertex] = fPosX;
				ptrVertex[dwOffsetVertex+1] = fPosY;

				dwOffsetVertex += __TERRAIN_SIZE_VERTEX;
				ptrVertex[dwOffsetVertex] = fPosX+__TERRAIN_SIZE_TEXTURE_X;
				ptrVertex[dwOffsetVertex+1] = fPosY;


				//   0---1---2
				//   |     |    |
				//   3---4---5
				//   |     |    |
				//   6---7---8
				//__последняя_строчка_____________________________________________
				height0 = height1;
				height1 = height2;
				height2 = m_aTerrainQuadInfo[dwOffset1++].getRealHeight();
				height3 = height4;
				height4 = height5;
				height5 = m_aTerrainQuadInfo[dwOffset2].getRealHeight();
				bHeight0 = bHeight1;
				bHeight1 = m_aTerrainQuadInfo[dwOffset2++].getHeight();
				height6 = height7;
				height7 = height8;
				height8 = m_aTerrainQuadInfo[dwOffset3].getRealHeight();
				bHeight3 = bHeight2;
				bHeight2 = m_aTerrainQuadInfo[dwOffset3++].getHeight();

			}
		}

	}

	void TileTerrain::showGrid(bool bShowGrid) // показывает решетку
	{
		if (m_bIsShowGrid == bShowGrid) return;
		m_bIsShowGrid = bShowGrid;
		MaterialPtr pMat = MaterialManager::getSingleton().getByName(__NAME_MATERIAL_TERRAIN_MAIN);
		pMat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setCurrentFrame(bShowGrid?1:0);
		pMat->compile();
	}

	bool TileTerrain::saveCurrentMap(const std::string& strMapName)
	{
		HANDLE hFile = ::CreateFileA(strMapName.c_str(), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			for (DWORD y=0; y<m_dwSizeY; y++)
			{
				for (DWORD x=0; x<m_dwSizeX; x++)
				{
					BYTE height = m_aTerrainQuadInfo[(DWORD)y*m_dwSizeX+x].getHeight();
					if (height != 0)
					{
						DWORD tmp = 0;
						if (::WriteFile(hFile, (LPVOID)&x, 1, &tmp, NULL) == 0) _OUT("error write file");
						if (::WriteFile(hFile, (LPVOID)&y, 1, &tmp, NULL) == 0) _OUT("error write file");
					}
				}
			}
			::CloseHandle(hFile);
			_OUT("File is create %s", strMapName);
		}
		else
		{
			return false;
		}
		
		return true;
	}

	void TileTerrain::loadMap()
	{
		for (DWORD y=0; y<m_dwSizeY; y++)
		{
			for (DWORD x=0; x<m_dwSizeX; x++)
			{
				TerrainDown(x, y);
			}
		}

		char strFileName[16];
		static BYTE bFileNum = 0;
		bool bLoop = true;
		do
		{
			sprintf(strFileName, "%.03d.map", bFileNum);
			if (GetFileAttributesA(strFileName) != DWORD(-1))
			{
				HANDLE hFile = ::CreateFileA(strFileName, FILE_ALL_ACCESS, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hFile != INVALID_HANDLE_VALUE)
				{
					DWORD tmp;
					bool bEnd = false;
					while (bEnd == false)
					{
						BYTE x = 0, y = 0;
						::ReadFile(hFile, (LPVOID)&x, 1, &tmp, NULL);
						if (tmp == 0) bEnd = true;
						else
						{
							::ReadFile(hFile, (LPVOID)&y, 1, &tmp, NULL);
							if (tmp == 0)
								bEnd = true;
							else
								TerrainUp(x, y);
						}
						if (bEnd)
						{
							::CloseHandle(hFile);
						}
					}
					::CloseHandle(hFile);
					_OUT("File is load %s", strFileName);
					bLoop = false;
					bFileNum ++;
				}
				else
				{
					if (bFileNum == 0)
					{
						bLoop = false; // ни одного файла
						_OUT("No find file");
					}
					else
					{
						bFileNum = 0;
					}
				}
			}
			else
			{
				if (bFileNum == 0)
				{
					bLoop = false; // ни одного файла
					_OUT("No find file");
				}
				else
				{
					bFileNum = 0;
				}
			}
		}
		while (bLoop);

	}

	void TileTerrain::_OUT(const char* _str, ...)
	{
	}

	bool TileTerrain::resizeTerrain(size_t wSizeX, size_t wSizeY)
	{
		if (!m_SceneManager) return false;

		if (wSizeX < 2) return false;
		if (wSizeY < 2) return false;

		m_dwSizeX = wSizeX;
		m_dwSizeY = wSizeY;
		__TERRAIN_OFFSET_VERTEX2 = (__TERRAIN_SIZE_QUAD_VERTEX*m_dwSizeX)-(__TERRAIN_SIZE_VERTEX*2);

		if (m_aTerrainQuadInfo) delete[] m_aTerrainQuadInfo;
		DWORD tmp = m_dwSizeX*m_dwSizeY;
		m_aTerrainQuadInfo = new TerrainTileInfo[tmp];
		memset((void*)m_aTerrainQuadInfo, 0, sizeof(TerrainTileInfo)*tmp);

		char nameFormat[32];
		::sprintf(nameFormat, "meshTerrain_%p", this);

		DWORD nVertices = m_dwSizeX * m_dwSizeY * __TERRAIN_COUNT_QUAD_VERTEX;

		DWORD vbufCount = nVertices * __TERRAIN_SIZE_VERTEX;
		float *vertices = (float*)(new float[vbufCount]);

		const DWORD ibufCount = m_dwSizeX * m_dwSizeY * __TERRAIN_COUNT_QUAD_INDEX;
		DWORD *faces = (DWORD*)(new DWORD[ibufCount]);

		DWORD index = 0;
		DWORD pos = 0;
		DWORD indexPos = 0;
		BYTE bNumTexture = 0;
		for (DWORD Y=0; Y<m_dwSizeY; Y ++)
		{
			for (DWORD X=0; X<m_dwSizeX; X ++)
			{

	#ifdef __SMOOTH_RANDOM
				bNumTexture = 1;//(BYTE)(::rand()%5);
	#else
				bNumTexture = 0;
	#endif

				float tmpX = X * __TERRAIN_SIZE_QUAD_XY;
				float tmpY = Y * __TERRAIN_SIZE_QUAD_XY;
				// 1 vertex
				vertices[pos++] = tmpX;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((X==0) || (Y == 0))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}

				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = 0.0;
				// 2 vertex
				vertices[pos++] = tmpX + __TERRAIN_SIZE_QUAD_XY;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((Y == 0) || (X == (m_dwSizeX-1)))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}
				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X+__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = 0.0;
				vertices[pos++] = 1.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_X;
				vertices[pos++] = 0.0;
				// 3 vertex
				tmpY += __TERRAIN_SIZE_QUAD_XY;
				vertices[pos++] = tmpX;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((X == 0) || (Y == (m_dwSizeY-1)))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}

				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE_Y;
				vertices[pos++] = 0.0;
				vertices[pos++] = 1.0;
				vertices[pos++] = 0.0;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_Y;
				// 4 vertex
				vertices[pos++] = tmpX + __TERRAIN_SIZE_QUAD_XY;
				vertices[pos++] = 0.0;
				vertices[pos++] = tmpY;
				if ((Y == (m_dwSizeY-1)) || (X == (m_dwSizeX-1)))
				{
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
					vertices[pos++] = -0.577350;
				}
				else
				{
					vertices[pos++] = 0.0;
					vertices[pos++] = 1.0;
					vertices[pos++] = 0.0;
				}

				vertices[pos++] = bNumTexture*__TERRAIN_SIZE_TEXTURE_X+__TERRAIN_SIZE_TEXTURE_X;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE_Y;
				vertices[pos++] = 1.0;
				vertices[pos++] = 1.0;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_X;
				vertices[pos++] = __TERRAIN_SIZE_TEXTURE3_Y;

				// index
				faces[index++] = indexPos+1;
				faces[index++] = indexPos;
				if (((X==0) && (Y==0)) || ((X==(m_dwSizeX-1))&&(Y==(m_dwSizeY-1))))
				{
					faces[index++] = indexPos+3;//???
					faces[index++] = indexPos;//???
				}
				else
				{
					faces[index++] = indexPos+2;
					faces[index++] = indexPos+1;
				}			
				faces[index++] = indexPos+2;
				faces[index++] = indexPos+3;
				indexPos += __TERRAIN_COUNT_QUAD_VERTEX;//???4

			}
		}

	#ifdef __SMOOTH_RANDOM
		m_wSmoothStartX = 1;
		m_wSmoothStartY = 1;
		m_wSmoothEndX = m_dwSizeX-1;
		m_wSmoothEndY = m_dwSizeY-1;
		SmoothTerrain(vertices, faces);
	#endif

		SubMesh* sub = m_meshTerrain->getSubMesh(0);

		m_meshTerrain->sharedVertexData = new VertexData();
		m_meshTerrain->sharedVertexData->vertexCount = nVertices;

		VertexDeclaration* decl = m_meshTerrain->sharedVertexData->vertexDeclaration;

		decl->addElement(0, 0, VET_FLOAT3, VES_POSITION);
		decl->addElement(0, sizeof(float)*3, VET_FLOAT3, VES_NORMAL);
		decl->addElement(0, sizeof(float)*6, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
		decl->addElement(0, sizeof(float)*8, VET_FLOAT2, VES_TEXTURE_COORDINATES, 1);
		decl->addElement(0, sizeof(float)*10, VET_FLOAT2, VES_TEXTURE_COORDINATES, 2);

		HardwareVertexBufferSharedPtr vbuf = HardwareBufferManager::getSingleton().createVertexBuffer(__TERRAIN_SIZE_VERTEX*sizeof(float), m_meshTerrain->sharedVertexData->vertexCount, HardwareBuffer::HBU_DYNAMIC );//HBU_STATIC_WRITE_ONLY);
		vbuf->writeData(0, vbuf->getSizeInBytes(), vertices, true);

		VertexBufferBinding* bind = m_meshTerrain->sharedVertexData->vertexBufferBinding; 
		bind->setBinding(0, vbuf);

		HardwareIndexBufferSharedPtr ibuf = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_32BIT, ibufCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY);

		ibuf->writeData(0, ibuf->getSizeInBytes(), faces, true);

		sub->useSharedVertices = true;
		sub->indexData->indexBuffer = ibuf;
		sub->indexData->indexCount = ibufCount;
		sub->indexData->indexStart = 0;

		m_meshTerrain->_setBounds(AxisAlignedBox(0,0,0, m_dwSizeX*__TERRAIN_SIZE_QUAD_XY,__TERRAIN_MAX_HEIGHT_Z2,m_dwSizeY*__TERRAIN_SIZE_QUAD_XY));
		m_meshTerrain->_setBoundingSphereRadius(Math::Sqrt(3*m_dwSizeX*__TERRAIN_SIZE_QUAD_XY*m_dwSizeX*__TERRAIN_SIZE_QUAD_XY));

		delete[] faces;
		delete[] vertices;

		return true;
	}

	bool TileTerrain::getTileHeight(size_t wPosX, size_t wPosY, int& _height)
	{
		if (wPosX < 2) return false;
		if (wPosY < 2) return false;
		if (wPosX >= (m_dwSizeX-1)) return false;
		if (wPosY >= (m_dwSizeY-1)) return false;

		DWORD dwOffset = (DWORD)wPosY*m_dwSizeX+(DWORD)wPosX;

		_height = m_aTerrainQuadInfo[dwOffset].getHeight();

		return true;
	}

}

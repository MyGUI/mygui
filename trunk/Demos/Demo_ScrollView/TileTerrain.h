/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef __TILE_TERRAIN_H__
#define __TILE_TERRAIN_H__

#include <Ogre.h>

namespace terrain
{

	using namespace Ogre;
	// ������ ����� �������
	#define __TERRAIN_SIZE_VERTEX 12
	// ����������� ������ � ��������
	#define __TERRAIN_COUNT_QUAD_VERTEX 4
	// ����������� �������� � ��������
	#define __TERRAIN_COUNT_QUAD_INDEX 6
	// ������ ����� �� 4 ������
	#define __TERRAIN_SIZE_QUAD_VERTEX (__TERRAIN_SIZE_VERTEX*__TERRAIN_COUNT_QUAD_VERTEX)
	// �������� � ������� ����������� �����
	#define __TERRAIN_OFFSET_VERTEX1 (__TERRAIN_SIZE_QUAD_VERTEX-__TERRAIN_SIZE_VERTEX)
	// �������� �� ������� ����, ������������� � ������
	//#define __TERRAIN_OFFSET_VERTEX2 (__TERRAIN_SIZE_QUAD_VERTEX*m_dwSizeX)-(__TERRAIN_SIZE_VERTEX*2)
	//=========================================================================================
	#define __QUADSELECT_SIZE_VERTEX 5
	#define __QUADSELECT_COUNT_QUAD_VERTEX    __TERRAIN_COUNT_QUAD_VERTEX
	#define __QUADSELECT_COUNT_QUAD_INDEX    __TERRAIN_COUNT_QUAD_INDEX
	#define __QUADSELECT_SIZE_QUAD_VERTEX (__QUADSELECT_SIZE_VERTEX*__QUADSELECT_COUNT_QUAD_VERTEX)
	#define __QUADSELECT_MAX_SIZE_SELECT 400
	#define __QUADSELECT_OFFSET_UP 0.2f
	#define __QUADSELECT_OFFSET_UP_HIDE 100000.0f
	#define __POINTSELECT_SIZE_XY 5.0f
	#define __POINTSELECT_OFFSET_UP 3.0f
	#define __QUADSELECT_OFFSET_TEXTURE_1 0.8f
	#define __QUADSELECT_OFFSET_TEXTURE_2 0.4f
	#define __QUADSELECT_OFFSET_TEXTURE_3 0.9f
	//=========================================================================================
	//=========================================================================================
	#define __TERRAIN_MASK_RAY 0x0004
	#define __TERRAIN_SIZE_QUAD_XY 50.0f
	#define __TERRAIN_SIZE_QUAD_HALF_XY (__TERRAIN_SIZE_QUAD_XY*0.5)
	#define __TERRAIN_SIZE_QUAD_Z 30.0f
	#define __TERRAIN_COUNT_HEIGHT_LEVELS 1
	const int __TERRAIN_MAX_HEIGHT_Z2 = 10;
	#define __TERRAIN_SIZE_TEXTURE_X 0.1
	#define __TERRAIN_SIZE_TEXTURE_Y 0.0833333
	#define __TERRAIN_SIZE_TEXTURE3_X __TERRAIN_SIZE_TEXTURE_X
	#define __TERRAIN_SIZE_TEXTURE3_Y __TERRAIN_SIZE_TEXTURE_Y
	#define __TERRAIN_COUNT_TEXTURE_THIS_QUAD 64
	//=========================================================================================
	#define __SMOOTH_RANDOM
	#define __NAME_MATERIAL_TERRAIN_MAIN "MyGame/TerrainMainBoy"
	#define __NAME_MATERIAL_TERRAIN_QUADSELECT "MyGame/TerrainCursor"

	class TileTerrain
	{
	public:
		struct TerrainTileInfo
		{
		public:
			float getRealHeight() { return (float)mHeight * __TERRAIN_SIZE_QUAD_Z; }
			int getHeight() { return mHeight; }
			void setHeight(int _height) { mHeight = _height; }
		private:
			int mHeight;
		public:
			BYTE bTextureNum; // ����� �������� �� ��������
			bool bIsLeft; // ���������� ���������� ��� ��� � ��������
			bool bNoModify; // ����� �� ������� �������� �����
		};

		// ���������� ��� ���� ������� ���� ��� �����������
		typedef struct __TERRAIN_DATA_INFO
		{
			bool bIsTerrainUp; // ������� �� �����
			BYTE bTextureNum2; // ����� �������� �������� ����
			BYTE bNumUnit; // ����� �����
			bool bIsDestroy; // ������ �� ���� �����
			BYTE bQuadInfo; // ���������� �� �������� ��������
			WORD wIndexBillBoard; // ���������� ����� �������
		} *__LP_TERRAIN_DATA_INFO;

	public:
		TileTerrain();
		~TileTerrain();
		bool createTerrain(WORD wSizeX, WORD wSizeY); // ������� �����, ���� ���� �� ���������
		bool initTerrain(SceneManager *mSceneMgr); // �������������
		void destroyTerrain(); // ������� ������ �����
		void releaseTerrain(); // ���������� ���, ����� ����� �������������
		bool TerrainUp(WORD wPosX, WORD wPosY); // ��������������� ��������
		bool TerrainDown(WORD wPosX, WORD wPosY); // ��������������� ���������
		bool TerrainEqviv(WORD wStartX, WORD wStartY, WORD wEndX, WORD wEndY); // ��������������� ������������
		bool setQuadTexture(WORD wPosX, WORD wPosY, BYTE bNumTexture); // ������������� �������� �� ������ ����
		bool setQuadTexture3(WORD wPosX, WORD wPosY, BYTE bNumTexture); // ������������� �������� �� ������ ����
		bool TestTerrainRay(WORD &wQuadX, WORD &wQuadY, Ray ray, bool bSelectQuad = true); // ���� ����� ����������� ���� � �����

		bool showQuadSelect(WORD wStartX, WORD wStartY, bool bIsWhite = true); // ���������� ���� ������ ���������
		bool showQuadSelect(WORD wStartX, WORD wStartY, WORD &wEndX, WORD &wEndY, bool bIsWhite = true); // ���������� ����� ���������
		bool showPointSelect(WORD wStartX, WORD wStartY, bool bIsWhite = true); // ���������� ����� ���������
		bool showPointSelect(WORD wStartX, WORD wStartY, WORD &wEndX, WORD &wEndY, bool bIsWhite = true); // ���������� ����� ���������
		void hideQuadSelect(); // �������� ����� ���������
		bool CreateQuadSelect(); // ������� ����� ���������
		void showGrid(bool bShowGrid); // ���������� �������
		DWORD getSizeTerrainX() { return m_dwSizeX; }
		DWORD getSizeTerrainY() { return m_dwSizeY; }
		bool saveCurrentMap(const std::string& strMapName); // ��������� ������� �����
		void loadMap();
		bool resizeTerrain(size_t _sx, size_t _sy);
		bool getTileHeight(size_t _sx, size_t _sy, int& _height);

	private:
		bool isVertexUp(WORD wPosX, WORD wPosY); // ����� �� ������� �����
		bool isVertexDown(WORD wPosX, WORD wPosY); // ����� �� �������� �����
		void VertexUp(WORD wPosX, WORD wPosY, float * ptrVertex); // �������� ������
		void VertexDown(WORD wPosX, WORD wPosY, float * ptrVertex); // ��������� ������
		void SmoothTerrain(float *ptrVertex, DWORD *ptrIndex); // ���������� ������� � ���������� �������������
		void _OUT(const char* _str, ...);

	private:
		SceneManager *m_SceneManager; // ���� �������� ��� �������� ����
		SceneNode *m_nodeTerrain; // ��� ��� �������� ������
		MeshPtr m_meshTerrain; // ��������� �����
		MeshPtr m_meshQuadSelect; // ��������� ����� ��� ����� ���������
		Entity *m_entityTerrain; // ������ � �����
		Entity *m_entityQuadSelect; // ������ � ����� �����
		BillboardSet* m_bbsPointSet; // ����������� ����� ��������� ������
		TerrainTileInfo* m_aTerrainQuadInfo; // ���������� �� ������� � ��������
		Billboard *m_billBoardsPoint[__QUADSELECT_MAX_SIZE_SELECT]; // ������ ���������� �� �����
		BYTE m_bHeightInQuadTexture[6][__TERRAIN_COUNT_TEXTURE_THIS_QUAD]; // ������ � ������� �������� ��� 4 �����
		BYTE mHeightMapping[__TERRAIN_MAX_HEIGHT_Z2];
		DWORD m_dwSizeX;
		DWORD m_dwSizeY;

		WORD m_wSmoothStartX; // ������ �������
		WORD m_wSmoothStartY; // ������ �������
		WORD m_wSmoothEndX; // ����� �������
		WORD m_wSmoothEndY; // ����� �������
		DWORD __TERRAIN_OFFSET_VERTEX2; // ����������� ���������������� ��� ���������� (__TERRAIN_SIZE_QUAD_VERTEX*m_dwSizeX)-(__TERRAIN_SIZE_VERTEX*2)
		DWORD m_dwOldPositionCleanQuadSelect; // � ����� ������� �������� ������ ��� ��������� � ���������� ������
		DWORD m_dwOldPositionCleanPointSelect; // � ����� ������� �������� ������ ��� ��������� � ���������� ������
		bool m_bIsShowSelectQuad; // ����� �� ����� ���������
		bool m_bIsShowSelectPoint; // ����� �� ����� ���������
		bool m_bIsShowGrid; // ����� �� �������
	};

}
#endif // __TILE_TERRAIN_H__

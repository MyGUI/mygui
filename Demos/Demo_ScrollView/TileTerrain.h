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
	// размер одной вершины
	#define __TERRAIN_SIZE_VERTEX 12
	// колличество вершин в квадрате
	#define __TERRAIN_COUNT_QUAD_VERTEX 4
	// колличество индексов в квадрате
	#define __TERRAIN_COUNT_QUAD_INDEX 6
	// размер блока из 4 вершин
	#define __TERRAIN_SIZE_QUAD_VERTEX (__TERRAIN_SIZE_VERTEX*__TERRAIN_COUNT_QUAD_VERTEX)
	// смещение с вершине предыдущего квада
	#define __TERRAIN_OFFSET_VERTEX1 (__TERRAIN_SIZE_QUAD_VERTEX-__TERRAIN_SIZE_VERTEX)
	// смещение на строчку выше, инициализация в классе
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
			BYTE bTextureNum; // номер текстуры на квадрате
			bool bIsLeft; // правильная ориентация или нет в квадрате
			bool bNoModify; // можно ли поднять опустить землю
		};

		// Информация обо всем игровом поле для отображения
		typedef struct __TERRAIN_DATA_INFO
		{
			bool bIsTerrainUp; // поднята ли земля
			BYTE bTextureNum2; // номер текстуры верхнего слоя
			BYTE bNumUnit; // номер юнита
			bool bIsDestroy; // подбит ли этот кусок
			BYTE bQuadInfo; // информация об вершинах квадрата
			WORD wIndexBillBoard; // порядковый номер индекса
		} *__LP_TERRAIN_DATA_INFO;

	public:
		TileTerrain();
		~TileTerrain();
		bool createTerrain(WORD wSizeX, WORD wSizeY); // создает землю, если есть то удаляется
		bool initTerrain(SceneManager *mSceneMgr); // инициализация
		void destroyTerrain(); // удаляет только землю
		void releaseTerrain(); // сбрасывает все, будет нужна инициализация
		bool TerrainUp(WORD wPosX, WORD wPosY); // непосредственно поднятие
		bool TerrainDown(WORD wPosX, WORD wPosY); // непосредственно опускание
		bool TerrainEqviv(WORD wStartX, WORD wStartY, WORD wEndX, WORD wEndY); // непосредственно выравнивание
		bool setQuadTexture(WORD wPosX, WORD wPosY, BYTE bNumTexture); // устанавливаем текстуру на первый слой
		bool setQuadTexture3(WORD wPosX, WORD wPosY, BYTE bNumTexture); // устанавливаем текстуру на третий слой
		bool TestTerrainRay(WORD &wQuadX, WORD &wQuadY, Ray ray, bool bSelectQuad = true); // ищем точку пересечения луча и земли

		bool showQuadSelect(WORD wStartX, WORD wStartY, bool bIsWhite = true); // показывает одну клетку выделения
		bool showQuadSelect(WORD wStartX, WORD wStartY, WORD &wEndX, WORD &wEndY, bool bIsWhite = true); // показывает сетку выделения
		bool showPointSelect(WORD wStartX, WORD wStartY, bool bIsWhite = true); // показывает точку выделения
		bool showPointSelect(WORD wStartX, WORD wStartY, WORD &wEndX, WORD &wEndY, bool bIsWhite = true); // показывает точку выделения
		void hideQuadSelect(); // скрывает сетку выделения
		bool CreateQuadSelect(); // создает сетку выделения
		void showGrid(bool bShowGrid); // показывает решетку
		DWORD getSizeTerrainX() { return m_dwSizeX; }
		DWORD getSizeTerrainY() { return m_dwSizeY; }
		bool saveCurrentMap(const std::string& strMapName); // сохраняет текущую карту
		void loadMap();
		bool resizeTerrain(size_t _sx, size_t _sy);
		bool getTileHeight(size_t _sx, size_t _sy, int& _height);

	private:
		bool isVertexUp(WORD wPosX, WORD wPosY); // можно ли поднять землю
		bool isVertexDown(WORD wPosX, WORD wPosY); // можно ли опустить землю
		void VertexUp(WORD wPosX, WORD wPosY, float * ptrVertex); // поднятие вершин
		void VertexDown(WORD wPosX, WORD wPosY, float * ptrVertex); // опускание вершин
		void SmoothTerrain(float *ptrVertex, DWORD *ptrIndex); // сглаживает нормали и напрвления треугольников
		void _OUT(const char* _str, ...);

	private:
		SceneManager *m_SceneManager; // сцен менеджер для привязки нода
		SceneNode *m_nodeTerrain; // нод для привязки интити
		MeshPtr m_meshTerrain; // вершинный буфер
		MeshPtr m_meshQuadSelect; // вершинный буфер для сетки выделения
		Entity *m_entityTerrain; // интити с мешем
		Entity *m_entityQuadSelect; // интити с мешем рамки
		BillboardSet* m_bbsPointSet; // билбоардсет точек выделения вершин
		TerrainTileInfo* m_aTerrainQuadInfo; // информация об вершине и квадрате
		Billboard *m_billBoardsPoint[__QUADSELECT_MAX_SIZE_SELECT]; // массив указателей на точки
		BYTE m_bHeightInQuadTexture[6][__TERRAIN_COUNT_TEXTURE_THIS_QUAD]; // массив с номером текстуры для 4 высот
		BYTE mHeightMapping[__TERRAIN_MAX_HEIGHT_Z2];
		DWORD m_dwSizeX;
		DWORD m_dwSizeY;

		WORD m_wSmoothStartX; // начало участка
		WORD m_wSmoothStartY; // начало участка
		WORD m_wSmoothEndX; // конец участка
		WORD m_wSmoothEndY; // конец участка
		DWORD __TERRAIN_OFFSET_VERTEX2; // обязательно инициализировать эту переменную (__TERRAIN_SIZE_QUAD_VERTEX*m_dwSizeX)-(__TERRAIN_SIZE_VERTEX*2)
		DWORD m_dwOldPositionCleanQuadSelect; // с какой позиции очищенна память для выделения в вертексном буфере
		DWORD m_dwOldPositionCleanPointSelect; // с какой позиции очищенна память для выделения в вертексном буфере
		bool m_bIsShowSelectQuad; // видна ли сетка выделения
		bool m_bIsShowSelectPoint; // видна ли точка выделения
		bool m_bIsShowGrid; // видна ли решетка
	};

}
#endif // __TILE_TERRAIN_H__

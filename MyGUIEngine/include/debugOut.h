
#ifndef _DEBUGOUT_H_
#define _DEBUGOUT_H_

#include "Prerequest.h"

	//------------------------------------------------------------------------------//
	// Окно отладки [my.name (my.work(a)inbox.ru)]
	//------------------------------------------------------------------------------//
	#pragma once
	//------------------------------------------------------------------------------//
	#include <Ogre.h>
	//------------------------------------------------------------------------------//
	using namespace Ogre;
	//------------------------------------------------------------------------------//
	// колличество строк
	#define __DEBUG_COUNT_LINE 30
	// показывать ли тень текста
	#define __DEBUG_SHOW_SHADOW  1
	// шрифт по умолчанию
	#define __DEBUG_FONT_DEFAULT "BlueHighway"
	// размер шрифта
	#define __DEBUG_FONT_SIZE 16
	// цвет шрифта
	#define __DEBUG_FONT_COLOUR "1.0 1.0 1.0"
	// цвет шрифта
	#define __DEBUG_FONT_SHADOW_COLOUR "0.0 0.0 0.0"
	// высота оверлея отладки
	#define __DEBUG_OVERLAY_ZORDER 649
	// имя оверлея отладки
	#define __DEBUG_OVERLAY_NAME "__debugOverlay"
	// имя елемента
	#define __DEBUG_OVERLAY_ELEMENT_NAME "__debugOverlayElement"
	// имя елемента тени
	#define __DEBUG_OVERLAY_ELEMENT_SHADOW_NAME "__debugOverlayElementShadow"
	// смещение от края
	#define __DEBUG_OVERLAY_OFFSET 10
	// ширина поля вывода
	#define __DEBUG_OVERLAY_INFO_WIDTH 500
	//------------------------------------------------------------------------------//
	namespace debugOverlay {
	//------------------------------------------------------------------------------//
	// класс представления дополнительных полей
	//------------------------------------------------------------------------------//
	class debugOverlayInfo
	{
	public:
		void createOverlayInfo(const String & name, uint16 x, uint16 y, void * value);
		void destroyOverlayInfo();

		virtual bool update() = 0;
		inline const String & getName();

	protected:
		OverlayContainer * m_overlayContainer;
		OverlayContainer * m_overlayContainerShadow; // для тени текста
		String m_strName; // имя поля вывода
		void * m_value; // указатель на переменную вывода
	};
	//------------------------------------------------------------------------------//
	// основной класс для вывода информации
	//------------------------------------------------------------------------------//
	class debugOverlay : public Ogre::FrameListener
	{
	public: 
		// единственный екземпляр
		static debugOverlay & Instance();
		// метод для вывода информации
		void out(char* fmt, ... );
		// методы для добавления полей вывода
		void add(const String & name, uint16 x, uint16 y, bool & value);
		void add(const String & name, uint16 x, uint16 y, char & value);
		void add(const String & name, uint16 x, uint16 y, uint8 & value);
		void add(const String & name, uint16 x, uint16 y, short & value);
		void add(const String & name, uint16 x, uint16 y, uint16 & value);
		void add(const String & name, uint16 x, uint16 y, int & value);
		void add(const String & name, uint16 x, uint16 y, uint32 & value);
		void add(const String & name, uint16 x, uint16 y, Real & value);
		void add(const String & name, uint16 x, uint16 y, Vector2 & value);
		void add(const String & name, uint16 x, uint16 y, Vector3 & value);
		void add(const String & name, uint16 x, uint16 y, Vector4 & value);
		void add(const String & name, uint16 x, uint16 y, Quaternion & value);
		void add(const String & name, uint16 x, uint16 y, Matrix3 & value);
		void add(const String & name, uint16 x, uint16 y, Matrix4 & value);
		void add(const String & name, uint16 x, uint16 y, Radian & value);
		void add(const String & name, uint16 x, uint16 y, Degree & value);
		void add(const String & name, uint16 x, uint16 y, ColourValue & value);
		// метод для удаления поля вывода
		void remove(const String & name);
		// метод для скрытия показа оверлея
		void show(bool _show = true);

		~debugOverlay();
		// для обновления данных
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

	private:
		// для доступа
		friend class debugOverlayInfo;

		debugOverlay();
		void check(); // проверяет все ли созданно, если нет, то создает

	private:
		std::vector<debugOverlayInfo*> mOverlayInfo; // список всех полей вывода
		Overlay * m_overlay; // оверлей для всех полей , прям стихи =)
		String m_strFontName; // имя шрифта
		uint16 m_countCharInLine[__DEBUG_COUNT_LINE]; // колличество символов в строках
		char m_stringDebugBuffer[1024]; // буфер для преобразования строки
		OverlayContainer* m_overlayDebugInfo;
		#if __DEBUG_SHOW_SHADOW == 1
			OverlayContainer* m_overlayDebugInfoShadow; // для тени текста
		#endif
	};
	//------------------------------------------------------------------------------//
	} // namespace debugOverlay 
	//------------------------------------------------------------------------------//
	extern debugOverlay::debugOverlay & debug;
	//------------------------------------------------------------------------------//


#endif
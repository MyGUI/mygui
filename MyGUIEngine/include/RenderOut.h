#ifndef _RENDEROUT_H_
#define _RENDEROUT_H_

#include "Prerequest.h"
#include "stringUtil.h"
#include <deque>

#include <OgreFontManager.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreOverlayContainer.h>
#include <OgreRenderWindow.h>

namespace MyGUI
{
	#undef OUT

	#ifdef _DEBUG

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

		namespace templates
		{
			template <class T>
			void render_out(const std::string & _value)
			{
				// структура информации об одной строке
				struct info
				{
					info() : num(0), count(1)  {}
					info(size_t _num, const std::string & _line) : num(_num), count(1), line(_line) {}

					size_t num;
					size_t count;
					std::string line;
				};

				// очередь
				typedef std::deque<info> DequeInfo;

				// текущая строка
				static size_t num = 0;
				// очередь всех наших строк
				static DequeInfo lines;
				// это наши оверлеи
				static Ogre::OverlayContainer * overlayDebugInfo = 0;
				static Ogre::OverlayContainer * overlayDebugInfoShadow = 0;
				static Ogre::Overlay * overlay = 0;

				// если первый раз, то создаем оверлей
				if (num == 0) {
					// ищем шрифт
					Ogre::ResourceManager::ResourceMapIterator fonts = Ogre::FontManager::getSingleton().getResourceIterator();
					assert(fonts.hasMoreElements()); // шрифтов чета нет =(

					// первый шрифт
					std::string fontName = fonts.getNext().getPointer()->getName();
					// ищем дефолтный, если нет то будет первый
					while (fonts.hasMoreElements()) {
						const std::string & name = fonts.getNext().getPointer()->getName();
						if (name == __DEBUG_FONT_DEFAULT) fontName = __DEBUG_FONT_DEFAULT;
					}

					// создаем оверлей, если не был создан
					Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
					overlay = overlayManager.create(__DEBUG_OVERLAY_NAME);
					overlay->setZOrder(__DEBUG_OVERLAY_ZORDER);
					Ogre::RenderWindow * win = Ogre::Root::getSingleton().getAutoCreatedWindow();

					#if __DEBUG_SHOW_SHADOW == 1
						// текстовое поле,  тень
						overlayDebugInfoShadow = static_cast< Ogre::OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_SHADOW_NAME, false));
						overlayDebugInfoShadow->setMetricsMode(Ogre::GMM_PIXELS);
						overlayDebugInfoShadow->setLeft(__DEBUG_OVERLAY_OFFSET + 1);
						overlayDebugInfoShadow->setTop(__DEBUG_OVERLAY_OFFSET + 1);
						overlayDebugInfoShadow->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET-1);
						overlayDebugInfoShadow->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET-1);
						overlayDebugInfoShadow->setParameter("font_name", fontName);
						overlayDebugInfoShadow->setParameter("char_height", util::toString(__DEBUG_FONT_SIZE) );
						overlayDebugInfoShadow->setParameter("colour_top", __DEBUG_FONT_SHADOW_COLOUR);
						overlayDebugInfoShadow->setParameter("colour_bottom", __DEBUG_FONT_SHADOW_COLOUR);
						overlay->add2D(overlayDebugInfoShadow); // добавляем контейнер на оверлей
					#endif
					// текстовое поле
					overlayDebugInfo = static_cast< Ogre::OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_NAME, false));
					overlayDebugInfo->setMetricsMode(Ogre::GMM_PIXELS);
					overlayDebugInfo->setLeft(__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setTop(__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setParameter("font_name", fontName);
					overlayDebugInfo->setParameter("char_height", util::toString(__DEBUG_FONT_SIZE) );
					overlayDebugInfo->setParameter("colour_top", __DEBUG_FONT_COLOUR);
					overlayDebugInfo->setParameter("colour_bottom", __DEBUG_FONT_COLOUR);

					overlay->add2D(overlayDebugInfo); // добавляем контейнер на оверлей
					overlay->show();

				}

				if (lines.empty()) { // первый раз просто добавляем
					lines.push_back(info(num++, _value));
					
				} else { // не первый раз мы тут
					// сравниваем последнюю строку
					if (lines.back().line == _value) lines.back().count ++;
					else {
						lines.push_back(info(num++, _value));
						// удаляем лишнее
						if (lines.size() > __DEBUG_COUNT_LINE) lines.pop_front();
					}
					
				}

				// а вот теперь выводми строки
				std::string str_out; 
				str_out.reserve(2048);

				for (DequeInfo::iterator iter=lines.begin(); iter != lines.end(); iter++) {
					str_out += util::toString("[ ", (unsigned int)iter->num, (iter->count > 1) ? (" , " + util::toString((unsigned int)iter->count)) : "", " ]  ", iter->line, "\n");
				}

				// непосредственный вывод
				if (overlayDebugInfoShadow) overlayDebugInfoShadow->setCaption(str_out);
				overlayDebugInfo->setCaption(str_out);
			}

		} // namespace templates

		template< class T1>
		inline void OUT (T1 p1){templates::render_out<void>(util::toString(p1));}

		template< class T1,  class T2 >
		inline void OUT (T1 p1, T2 p2)	{templates::render_out<void>(util::toString(p1, p2));}

		template< class T1,  class T2,  class T3 >
		inline void OUT (T1 p1, T2 p2, T3 p3) {templates::render_out<void>(util::toString(p1, p2, p3));}

		template< class T1,  class T2,  class T3, class T4 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4) {templates::render_out<void>(util::toString(p1, p2, p3, p4));}

		template< class T1,  class T2,  class T3, class T4, class T5 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)	{templates::render_out<void>(util::toString(p1, p2, p3, p4, p5));}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {templates::render_out<void>(util::toString(p1, p2, p3, p4, p5, p6));}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {templates::render_out<void>(util::toString(p1, p2, p3, p4, p5, p6, p7));}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7, class T8 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8) {templates::render_out<void>(util::toString(p1, p2, p3, p4, p5, p6, p7, p8));}

	#else

		template< class T1>
		inline void OUT (T1 p1) {}

		template< class T1,  class T2 >
		inline void OUT (T1 p1, T2 p2) {}

		template< class T1,  class T2,  class T3 >
		inline void OUT (T1 p1, T2 p2, T3 p3) {}

		template< class T1,  class T2,  class T3, class T4 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4) {}

		template< class T1,  class T2,  class T3, class T4, class T5 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {}

	#endif

} // namespace MyGUI

#endif

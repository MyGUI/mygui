#ifndef _RENDEROUT_H_
#define _RENDEROUT_H_

#include "Prerequest.h"
#include "stringUtil.h"
#include <deque>
#include "Ogre.h"
#include "OgreFontManager.h"

namespace widget
{
	#undef OUT

	#ifdef _DEBUG

		#define __DEBUG_COUNT_LINE 30
		// ���������� �� ���� ������
		#define __DEBUG_SHOW_SHADOW  1
		// ����� �� ���������
		#define __DEBUG_FONT_DEFAULT "BlueHighway"
		// ������ ������
		#define __DEBUG_FONT_SIZE 16
		// ���� ������
		#define __DEBUG_FONT_COLOUR "1.0 1.0 1.0"
		// ���� ������
		#define __DEBUG_FONT_SHADOW_COLOUR "0.0 0.0 0.0"
		// ������ ������� �������
		#define __DEBUG_OVERLAY_ZORDER 649
		// ��� ������� �������
		#define __DEBUG_OVERLAY_NAME "__debugOverlay"
		// ��� ��������
		#define __DEBUG_OVERLAY_ELEMENT_NAME "__debugOverlayElement"
		// ��� �������� ����
		#define __DEBUG_OVERLAY_ELEMENT_SHADOW_NAME "__debugOverlayElementShadow"
		// �������� �� ����
		#define __DEBUG_OVERLAY_OFFSET 10
		// ������ ���� ������
		#define __DEBUG_OVERLAY_INFO_WIDTH 500

		// ��������� ���������� �� ����� ������
		struct info
		{
			info() : num(0), count(1)  {}
			info(size_t _num, const std::string & _line) : num(_num), count(1), line(_line) {}

			size_t num;
			size_t count;
			std::string line;
		};

		typedef std::deque<info> DequeInfo;

		struct render_out
		{
			static void out(const std::string & _value)
			{
				// ������� ������
				static size_t num = 0;
				// ������� ���� ����� �����
				static DequeInfo lines;
				// ��� ���� �������
				static Ogre::OverlayContainer * overlayDebugInfo = 0;
				static Ogre::OverlayContainer * overlayDebugInfoShadow = 0;
				static Ogre::Overlay * overlay = 0;

				// ���� ������ ���, �� ������� �������
				if (num == 0) {
					// ���� �����
					Ogre::ResourceManager::ResourceMapIterator fonts = Ogre::FontManager::getSingleton().getResourceIterator();
					assert(fonts.hasMoreElements()); // ������� ���� ��� =(

					// ������ �����
					std::string fontName = fonts.getNext().getPointer()->getName();
					// ���� ���������, ���� ��� �� ����� ������
					while (fonts.hasMoreElements()) {
						const std::string & name = fonts.getNext().getPointer()->getName();
						if (name == __DEBUG_FONT_DEFAULT) fontName = __DEBUG_FONT_DEFAULT;
					}

					// ������� �������, ���� �� ��� ������
					Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
					overlay = overlayManager.create(__DEBUG_OVERLAY_NAME);
					overlay->setZOrder(__DEBUG_OVERLAY_ZORDER);
					Ogre::RenderWindow * win = Ogre::Root::getSingleton().getAutoCreatedWindow();

					#if __DEBUG_SHOW_SHADOW == 1
						// ��������� ����,  ����
						overlayDebugInfoShadow = static_cast< Ogre::OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_SHADOW_NAME, false));
						overlayDebugInfoShadow->setMetricsMode(Ogre::GMM_PIXELS);
						overlayDebugInfoShadow->setLeft(__DEBUG_OVERLAY_OFFSET + 1);
						overlayDebugInfoShadow->setTop(__DEBUG_OVERLAY_OFFSET + 1);
						overlayDebugInfoShadow->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET-1);
						overlayDebugInfoShadow->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET-1);
						overlayDebugInfoShadow->setParameter("font_name", fontName);
						overlayDebugInfoShadow->setParameter("char_height", toString(__DEBUG_FONT_SIZE) );
						overlayDebugInfoShadow->setParameter("colour_top", __DEBUG_FONT_SHADOW_COLOUR);
						overlayDebugInfoShadow->setParameter("colour_bottom", __DEBUG_FONT_SHADOW_COLOUR);
						overlay->add2D(overlayDebugInfoShadow); // ��������� ��������� �� �������
					#endif
					// ��������� ����
					overlayDebugInfo = static_cast< Ogre::OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_NAME, false));
					overlayDebugInfo->setMetricsMode(Ogre::GMM_PIXELS);
					overlayDebugInfo->setLeft(__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setTop(__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET);
					overlayDebugInfo->setParameter("font_name", fontName);
					overlayDebugInfo->setParameter("char_height", toString(__DEBUG_FONT_SIZE) );
					overlayDebugInfo->setParameter("colour_top", __DEBUG_FONT_COLOUR);
					overlayDebugInfo->setParameter("colour_bottom", __DEBUG_FONT_COLOUR);

					overlay->add2D(overlayDebugInfo); // ��������� ��������� �� �������
					overlay->show();

				}

				if (lines.empty()) { // ������ ��� ������ ���������
					lines.push_back(info(num++, _value));
					
				} else { // �� ������ ��� �� ���
					// ���������� ��������� ������
					if (lines.back().line == _value) lines.back().count ++;
					else {
						lines.push_back(info(num++, _value));
						// ������� ������
						if (lines.size() > __DEBUG_COUNT_LINE) lines.pop_front();
					}
					
				}

				// � ��� ������ ������� ������
				std::string str_out; 

				for (DequeInfo::iterator iter=lines.begin(); iter != lines.end(); iter++) {
					str_out += toString("[ ", (unsigned int)iter->num, (iter->count > 1) ? (" , " + toString((unsigned int)iter->count)) : "", " ]  ", iter->line, "\n");
				}

				// ���������������� �����
				if (overlayDebugInfoShadow) overlayDebugInfoShadow->setCaption(str_out);
				overlayDebugInfo->setCaption(str_out);

			}
		};

		template< class T1>
		inline void OUT (T1 p1)
		{
			render_out::out(toString(p1));
		}

		template< class T1,  class T2 >
		inline void OUT (T1 p1, T2 p2)
		{
			render_out::out(toString(p1) + toString(p2));
		}

		template< class T1,  class T2,  class T3 >
		inline void OUT (T1 p1, T2 p2, T3 p3)
		{
			render_out::out(toString(p1) + toString(p2) + toString(p3));
		}

		template< class T1,  class T2,  class T3, class T4 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4)
		{
			render_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4));
		}

		template< class T1,  class T2,  class T3, class T4, class T5 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
		{
			render_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5));
		}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
		inline void OUT (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
		{
			render_out::out(toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5) + toString(p6));
		}

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

} // namespace widget

#endif

/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/

#include "MyGUI_RenderOut.h"
#include "MyGUI_Utility.h"

#include <OgreFontManager.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreOverlayContainer.h>
#include <OgreRenderWindow.h>

namespace MyGUI
{
	namespace implement
	{

		const size_t __DEBUG_COUNT_LINE = 30;
		// ���������� �� ���� ������
		const bool __DEBUG_SHOW_SHADOW = 1;
		// ����� �� ���������
		const char * __DEBUG_FONT_DEFAULT = "BlueHighway";
		// ������ ������
		const float __DEBUG_FONT_SIZE = 16;
		// ���� ������
		const char * __DEBUG_FONT_COLOUR = "1.0 1.0 1.0";
		// ���� ������
		const char * __DEBUG_FONT_SHADOW_COLOUR = "0.0 0.0 0.0";
		// ������ ������� �������
		const unsigned short __DEBUG_OVERLAY_ZORDER = 649;
		// ��� ������� �������
		const char * __DEBUG_OVERLAY_NAME = "__debugOverlay";
		// ��� ��������
		const char * __DEBUG_OVERLAY_ELEMENT_NAME = "__debugOverlayElement";
		// ��� �������� ����
		const char *__DEBUG_OVERLAY_ELEMENT_SHADOW_NAME = "__debugOverlayElementShadow";
		// �������� �� ����
		const float __DEBUG_OVERLAY_OFFSET = 10;
		// ������ ���� ������
		const float __DEBUG_OVERLAY_INFO_WIDTH = 500;

		// ��������� ���������� �� ����� ������
		struct info
		{
			info() : num(0), count(1)  {}
			info(size_t _num, const std::string & _line) : num(_num), count(1), line(_line) {}

			size_t num;
			size_t count;
			std::string line;
		};

		void render_out(const std::string & _value)
		{
			// �������
			typedef std::deque<info> DequeInfo;

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
				};

				// ������� �������, ���� �� ��� ������
				Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();
				overlay = overlayManager.create(__DEBUG_OVERLAY_NAME);
				overlay->setZOrder(__DEBUG_OVERLAY_ZORDER);
				Ogre::RenderWindow * win = Ogre::Root::getSingleton().getAutoCreatedWindow();

				if (__DEBUG_SHOW_SHADOW) {
					// ��������� ����,  ����
					overlayDebugInfoShadow = static_cast< Ogre::OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_SHADOW_NAME, false));
					overlayDebugInfoShadow->setMetricsMode(Ogre::GMM_PIXELS);
					overlayDebugInfoShadow->setLeft(__DEBUG_OVERLAY_OFFSET + 1);
					overlayDebugInfoShadow->setTop(__DEBUG_OVERLAY_OFFSET + 1);
					overlayDebugInfoShadow->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET-1);
					overlayDebugInfoShadow->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET-1);
					overlayDebugInfoShadow->setParameter("font_name", fontName);
					overlayDebugInfoShadow->setParameter("char_height", utility::toString(__DEBUG_FONT_SIZE) );
					overlayDebugInfoShadow->setParameter("colour_top", __DEBUG_FONT_SHADOW_COLOUR);
					overlayDebugInfoShadow->setParameter("colour_bottom", __DEBUG_FONT_SHADOW_COLOUR);
					overlay->add2D(overlayDebugInfoShadow); // ��������� ��������� �� �������
				}
				// ��������� ����
				overlayDebugInfo = static_cast< Ogre::OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_NAME, false));
				overlayDebugInfo->setMetricsMode(Ogre::GMM_PIXELS);
				overlayDebugInfo->setLeft(__DEBUG_OVERLAY_OFFSET);
				overlayDebugInfo->setTop(__DEBUG_OVERLAY_OFFSET);
				overlayDebugInfo->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET);
				overlayDebugInfo->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET);
				overlayDebugInfo->setParameter("font_name", fontName);
				overlayDebugInfo->setParameter("char_height", utility::toString(__DEBUG_FONT_SIZE) );
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
			str_out.reserve(2048);

			for (DequeInfo::iterator iter=lines.begin(); iter != lines.end(); ++iter) {
				str_out += utility::toString("[ ", (unsigned int)iter->num, (iter->count > 1) ? (" , " + utility::toString((unsigned int)iter->count)) : "", " ]  ", iter->line, "\n");
			}

			// ���������������� �����
			if (overlayDebugInfoShadow) overlayDebugInfoShadow->setCaption(str_out);
			overlayDebugInfo->setCaption(str_out);
		}
	} // namespace implement
} // namespace MyGUI

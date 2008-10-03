/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/

#include "MyGUI_RenderOut.h"
#include "MyGUI_Utility.h"

#include "MyGUI_Gui.h"
#include "MyGUI_FontManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{
	namespace implement
	{

		// ��������� ���������� �� ����� ������
		struct info
		{
			info() : num(0), count(1)  { }
			info(size_t _num, const std::string & _line) : num(_num), count(1), line(_line) { }

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

			const int offset = 10;
			const int count_lines = 20;
			static const std::string font = "Default";
			static const std::string layer = "Statistic";
			static const std::string skin = "StaticText";

			static WidgetPtr widget = null;
			static WidgetPtr widget_shadow = null;

			if (widget == null) {
				Gui * gui = Gui::getInstancePtr();
				if (gui == null) return;

				const IntSize & size = gui->getViewSize();

				if (!LayerManager::getInstance().isExist(layer)) return;
				if (!SkinManager::getInstance().isExist(skin)) return;
				if (!FontManager::getInstance().isExist(font)) return;


				widget_shadow = gui->createWidget<Widget>(skin, IntCoord(offset + 1, offset + 1, size.width - offset - offset, size.height - offset - offset), Align::Stretch, layer);
				widget_shadow->setNeedMouseFocus(false);
				widget_shadow->setTextAlign(Align::Default);
				widget_shadow->setColour(Ogre::ColourValue::Black);

				widget = gui->createWidget<Widget>(skin, IntCoord(offset, offset, size.width - offset - offset, size.height - offset - offset), Align::Stretch, layer);
				widget->setNeedMouseFocus(false);
				widget->setTextAlign(Align::Default);
				widget->setColour(Ogre::ColourValue::White);
			}

			if (lines.empty()) { // ������ ��� ������ ���������
				lines.push_back(info(num++, _value));

			}
			else { // �� ������ ��� �� ���
				// ���������� ��������� ������
				if (lines.back().line == _value) lines.back().count ++;
				else {
					lines.push_back(info(num++, _value));
					// ������� ������
					if (lines.size() > count_lines) lines.pop_front();
				}

			}

			// � ��� ������ ������� ������
			std::string str_out;
			str_out.reserve(2048);

			for (DequeInfo::iterator iter=lines.begin(); iter != lines.end(); ++iter) {
				str_out += utility::toString("[ ", (unsigned int)iter->num, (iter->count > 1) ? (" , " + utility::toString((unsigned int)iter->count)) : "", " ]  ", iter->line, "\n");
			}

			// ���������������� �����
			widget_shadow->setCaption(str_out);
			widget->setCaption(str_out);
		}
	}

} // namespace MyGUI

/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	INSTANCE_IMPLEMENT(ControllerFadeAlpha);

	void ControllerFadeAlpha::addItem(WidgetPtr _widget, float _alpha, float _coef, bool _hide, bool _enabled, bool _destroy)
	{
		MYGUI_DEBUG_ASSERT(_coef > 0, "coef must be > 0");
		// ���������� �������
		_widget->setEnabled(_enabled, true);

		if ((ALPHA_MIN != _alpha) && (false == _widget->isShow())) {
			_widget->setAlpha(ALPHA_MIN);
			_widget->show();
		}

		// ���������� ��� �� �����
		if (false == _enabled) InputManager::getInstance().unlinkWidget(_widget);

		if (0 == mListItem.size()) Gui::getInstance().addFrameListener(this);

		for (ListAlphaItem::iterator iter=mListItem.begin(); iter!=mListItem.end(); ++iter) {
			if ((*iter).widget == _widget) {
				if ((*iter).destroy) {
					MYGUI_LOG(Warning, "widget '" << _widget->getName() << "' must be deleted, ignore change status alpha in controller");
					return;
				}
				(*iter).alpha = _alpha;
				(*iter).destroy = _destroy;
				(*iter).hide = _hide;
				return;
			}
		}

		mListItem.insert(mListItem.end(), AlphaItem(_widget, _alpha, _coef, _hide, _destroy));
	}

	void ControllerFadeAlpha::removeItem(WidgetPtr _widget)
	{
		for (ListAlphaItem::iterator iter=mListItem.begin(); iter!=mListItem.end(); ++iter) {
			if ((*iter).widget == _widget) {
				(*iter).widget = null;
				break;
			}
		}
	}

	void ControllerFadeAlpha::_frameEntered(float _time)
	{
		for (ListAlphaItem::iterator iter=mListItem.begin(); iter!=mListItem.end();) {

			if (null == (*iter).widget) {
				// ������� �� ������, �������� �� ����������� � �� ����� ����
				iter = mListItem.erase(iter);
				continue;
			}

			float alpha = (*iter).widget->getAlpha();

			// ��������� ����� �� � ���� ��� ����������
			if ((*iter).alpha > alpha) {
				alpha += _time * (*iter).coef;
				if ((*iter).alpha > alpha) {
					(*iter).widget->setAlpha(alpha);
					// ����������� �������� � �� ��������� ����
					++iter;
					continue;
				}
			}
			else if ((*iter).alpha < alpha) {
				alpha -= _time * (*iter).coef;
				if ((*iter).alpha < alpha) {
					(*iter).widget->setAlpha(alpha);
					// ����������� �������� � �� ��������� ����
					++iter;
					continue;
				}
			}

			// ���� �� ��� ������ ������� ����������
			if ((*iter).destroy) WidgetManager::getInstance().destroyWidget((*iter).widget);
			else if ((*iter).hide) (*iter).widget->hide();

			// �� ��������� �������� ������ ������� �� ������
			(*iter).widget = null;
		}

		if (0 == mListItem.size()) Gui::getInstance().removeFrameListener(this);
	}

	void ControllerFadeAlpha::_unlinkWidget(WidgetPtr _widget)
	{
		removeItem(_widget);
	}

} // namespace MyGUI

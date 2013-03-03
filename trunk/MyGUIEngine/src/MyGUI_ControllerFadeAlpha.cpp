/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_Gui.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_Widget.h"

namespace MyGUI
{

	ControllerFadeAlpha::ControllerFadeAlpha() :
		mAlpha(1),
		mCoef(1),
		mEnabled(true)
	{
	}

	ControllerFadeAlpha::~ControllerFadeAlpha()
	{
	}

	void ControllerFadeAlpha::prepareItem(Widget* _widget)
	{
		// подготовка виджета, блокируем если только нужно
		if (!mEnabled) _widget->setEnabledSilent(mEnabled);

		if ((ALPHA_MIN != mAlpha) && (!_widget->getVisible()))
		{
			_widget->setAlpha(ALPHA_MIN);
			_widget->setVisible(true);
		}

		// отписываем его от ввода
		if (!mEnabled) InputManager::getInstance().unlinkWidget(_widget);

		// вызываем пользовательский делегат для подготовки
		eventPreAction(_widget, this);
	}

	bool ControllerFadeAlpha::addTime(Widget* _widget, float _time)
	{
		float alpha = _widget->getAlpha();

		// проверяем нужно ли к чему еще стремиться
		if (mAlpha > alpha)
		{
			alpha += _time * mCoef;
			if (mAlpha > alpha)
			{
				_widget->setAlpha(alpha);
				eventUpdateAction(_widget, this);
				return true;
			}
			else
			{
				_widget->setAlpha(mAlpha);
			}
		}
		else if (mAlpha < alpha)
		{
			alpha -= _time * mCoef;
			if (mAlpha < alpha)
			{
				_widget->setAlpha(alpha);
				eventUpdateAction(_widget, this);
				return true;
			}
			else
			{
				_widget->setAlpha(mAlpha);
			}
		}

		// вызываем пользовательский делегат пост обработки
		eventPostAction(_widget, this);

		return false;
	}

	void ControllerFadeAlpha::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "Alpha")
			setAlpha(utility::parseValue<float>(_value));
		else if (_key == "Coef")
			setCoef(utility::parseValue<float>(_value));
		else if (_key == "Enabled")
			setEnabled(utility::parseValue<bool>(_value));
	}

	void ControllerFadeAlpha::setAlpha(float _value)
	{
		mAlpha = _value;
	}

	void ControllerFadeAlpha::setCoef(float _value)
	{
		mCoef = _value;
	}

	void ControllerFadeAlpha::setEnabled(bool _value)
	{
		mEnabled = _value;
	}

} // namespace MyGUI

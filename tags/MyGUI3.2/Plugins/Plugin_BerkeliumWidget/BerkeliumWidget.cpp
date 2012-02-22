/*!
	@file
	@author		Albert Semenov
	@date		02/2010
*/
#include "BerkeliumWidget.h"
#include "BerkeliumDelegate.h"
#include "../../Common/Input/InputConverter.h"
#include <berkelium/Window.hpp>
#include <berkelium/Context.hpp>

namespace MyGUI
{

	BerkeliumWidget::BerkeliumWidget() :
		mWindow(nullptr),
		mOldWidth(0),
		mOldHeight(0),
		mWindowDelegate(nullptr)
	{
	}

	void BerkeliumWidget::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		updateSize();

		createTexture(mOldWidth, mOldHeight, TRM_PT_VIEW_REQUESTED);
		requestUpdateCanvas = newDelegate(this, &BerkeliumWidget::notifyUpdateCanvas);

		Berkelium::Context *context = Berkelium::Context::create();
		mWindow = Berkelium::Window::create(context);

		updateSize();

		mBuffer.resize(mOldWidth, mOldHeight);
		mWindow->resize(mOldWidth, mOldHeight);
		mWindowDelegate = new BerkeliumDelegate();
		setWindowDelegate(mWindowDelegate);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BerkeliumWidget::notifyFrameStart);
	}

	void BerkeliumWidget::shutdownOverride()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BerkeliumWidget::notifyFrameStart);

		destroyTexture();

		if (mWindow != nullptr)
		{
			mWindow->destroy();
			mWindow = nullptr;
		}
		requestUpdateCanvas = nullptr;

		if (mWindowDelegate)
		{
			delete mWindowDelegate;
			mWindowDelegate = nullptr;
		}

		Base::shutdownOverride();
	}

	void BerkeliumWidget::updateSize()
	{
		mOldWidth = std::max(1, getWidth());
		mOldHeight = std::max(1, getHeight());
	}

	void BerkeliumWidget::notifyUpdateCanvas(Canvas* _canvas, Canvas::Event _event)
	{
		if (mWindow == nullptr)
			return;

		if (_event.widgetResized)
		{
			if (mOldWidth != getWidth() || mOldHeight != getHeight())
			{
				updateSize();

				mBuffer.resize(mOldWidth, mOldHeight);
				mWindow->resize(mOldWidth, mOldHeight);

				update();
			}
			else if (_event.requested || _event.textureChanged)
			{
				update();
			}
		}
		else if (_event.requested || _event.textureChanged)
		{
			update();
		}
	}

	void BerkeliumWidget::loadURL(const std::string& _url)
	{
		if (mWindow != nullptr)
			mWindow->navigateTo(_url.c_str(), _url.length());
	}

	Berkelium::Window* BerkeliumWidget::getBerkeliumWindow()
	{
		return mWindow;
	}

	void BerkeliumWidget::setWindowDelegate(BerkeliumDelegate* _windowDelegate)
	{
		_windowDelegate->setBuffer(&mBuffer);
		_windowDelegate->setWidget(this);
		_windowDelegate->setWindow(mWindow);
		mWindow->setDelegate(_windowDelegate);
	}

	void BerkeliumWidget::notifyFrameStart(float _time)
	{
		if (mWindow == nullptr)
			return;

		if (mBuffer.isDirty())
			updateTexture();
	}

	void BerkeliumWidget::update()
	{
		void* data = lock();

		mBuffer.draw(data, getTextureRealWidth());

		unlock();
	}

	void BerkeliumWidget::onMouseDrag(int _left, int _top, MouseButton _id)
	{
		if (mWindow != nullptr)
			mWindow->mouseMoved(_left - getAbsoluteLeft(), _top - getAbsoluteTop());

		Base::onMouseDrag(_left, _top, _id);
	}

	void BerkeliumWidget::onMouseMove(int _left, int _top)
	{
		if (mWindow != nullptr)
			mWindow->mouseMoved(_left - getAbsoluteLeft(), _top - getAbsoluteTop());

		Base::onMouseMove(_left, _top);
	}

	void BerkeliumWidget::onMouseWheel(int _rel)
	{
		if (mWindow != nullptr)
			mWindow->mouseWheel(0, _rel);

		Base::onMouseWheel(_rel);
	}

	void BerkeliumWidget::onMouseButtonPressed(int _left, int _top, MouseButton _id)
	{
		if (mWindow != nullptr)
			mWindow->mouseButton(_id.toValue(), true);

		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void BerkeliumWidget::onMouseButtonReleased(int _left, int _top, MouseButton _id)
	{
		if (mWindow != nullptr)
			mWindow->mouseButton(_id.toValue(), false);

		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void BerkeliumWidget::onKeyLostFocus(Widget* _new)
	{
		if (mWindow != nullptr)
			mWindow->unfocus();

		Base::onKeyLostFocus(_new);
	}

	void BerkeliumWidget::onKeySetFocus(Widget* _old)
	{
		if (mWindow != nullptr)
			mWindow->focus();

		Base::onKeySetFocus(_old);
	}

	void BerkeliumWidget::onKeyButtonPressed(KeyCode _key, Char _char)
	{
		if (mWindow != nullptr)
		{
			InputManager& manager = InputManager::getInstance();
			int mod = 0;
			if (manager.isControlPressed())
				mod |= Berkelium::CONTROL_MOD;
			if (manager.isShiftPressed())
				mod |= Berkelium::SHIFT_MOD;

			mWindow->keyEvent(true, mod, input::ScanCodeToVirtualKey(_key.toValue()), 0);

			if (_char != 0)
			{
				wchar_t text = (wchar_t)_char;
				mWindow->textEvent(&text, 1);
			}
		}

		Base::onKeyButtonPressed(_key, _char);
	}

	void BerkeliumWidget::onKeyButtonReleased(KeyCode _key)
	{
		if (mWindow != nullptr)
		{
			InputManager& manager = InputManager::getInstance();
			int mod = 0;
			if (manager.isControlPressed())
				mod |= Berkelium::CONTROL_MOD;
			if (manager.isShiftPressed())
				mod |= Berkelium::SHIFT_MOD;

			mWindow->keyEvent(false, mod, input::ScanCodeToVirtualKey(_key.toValue()), 0);
		}

		Base::onKeyButtonReleased(_key);
	}

	void BerkeliumWidget::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "SourceURL")
			loadURL(_value);
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

}

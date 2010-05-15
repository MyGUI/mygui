/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#include "BerkeliumWidget.h"
#include "MyGUI.h"
#include "../../Common/Input/InputConverter.h"

namespace MyGUI
{

	BerkeliumWidget::BerkeliumWidget() :
		mWindow(nullptr),
		mOldWidth(0),
		mOldHeight(0)
	{
	}

	BerkeliumWidget::~BerkeliumWidget()
	{
		requestUpdateCanvas = nullptr;

		shutdownWidgetSkin();
	}

	void BerkeliumWidget::updateSize()
	{
		mOldWidth = std::max(1, getWidth());
		mOldHeight = std::max(1, getHeight());
	}

	void BerkeliumWidget::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

		updateSize();

		createTexture(mOldWidth, mOldHeight, TRM_PT_VIEW_REQUESTED);
		requestUpdateCanvas = newDelegate(this, &BerkeliumWidget::notifyUpdateCanvas);

		initialiseWidgetSkin(_info);
	}

	void BerkeliumWidget::baseChangeWidgetSkin(ResourceSkin* _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void BerkeliumWidget::initialiseWidgetSkin(ResourceSkin* _info)
	{
		mWindow = Berkelium::Window::create();

		updateSize();

		mBuffer.resize(mOldWidth, mOldHeight);
		mWindow->resize(mOldWidth, mOldHeight);
		mWindow->setDelegate(this);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BerkeliumWidget::notifyFrameStart);
	}

	void BerkeliumWidget::shutdownWidgetSkin()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BerkeliumWidget::notifyFrameStart);

		if (mWindow != nullptr)
		{
			delete mWindow;
			mWindow = nullptr;
		}
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

				update(true);
			}
			else if (_event.requested || _event.textureChanged)
			{
				update(true);
			}
		}
		else if (_event.requested || _event.textureChanged)
		{
			update(true);
		}
	}

	void BerkeliumWidget::loadURL(const std::string& _url)
	{
		if (mWindow != nullptr)
			mWindow->navigateTo(_url.data(), _url.length());
	}

	void BerkeliumWidget::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "Berkelium_SourceURL") loadURL(_value);
		else Base::setProperty(_key, _value);
	}

	void BerkeliumWidget::onPaint(Berkelium::Window *win, const unsigned char *sourceBuffer, const Berkelium::Rect &rect, int dx, int dy, const Berkelium::Rect &scrollRect)
	{
		if (mWindow == nullptr)
			return;

		mBuffer.scroll(dx, dy, scrollRect.left(), scrollRect.top(), scrollRect.width(), scrollRect.height());
		mBuffer.update((void*)sourceBuffer, rect.left(), rect.top(), rect.width(), rect.height());
	}

	void BerkeliumWidget::notifyFrameStart(float _time)
	{
		if (mWindow == nullptr)
			return;

		update(false);
	}

	void BerkeliumWidget::update(bool _invalidate)
	{
		if (!mBuffer.isDirty() && !_invalidate)
			return;

		void* data = lock();

		mBuffer.draw(data, getTextureRealWidth());

		unlock();
	}

	void BerkeliumWidget::onMouseDrag(int _left, int _top)
	{
		if (mWindow != nullptr)
			mWindow->mouseMoved(_left - getLeft(), _top - getTop());

		Base::onMouseDrag(_left, _top);
	}

	void BerkeliumWidget::onMouseMove(int _left, int _top)
	{
		if (mWindow != nullptr)
			mWindow->mouseMoved(_left - getLeft(), _top - getTop());

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
}

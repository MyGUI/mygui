/*!
	@file
	@author		Albert Semenov
	@date		10/2009
	@module
*/
#include "AwesomiumWidget.h"
#include "MyGUI.h"

namespace Awesomium
{

	AwesomiumWidget::AwesomiumWidget() :
		mOldCursor(0),
		mControl(0),
		mIsTransparent(false),
		mOldWidth(getWidth()),
		mOldHeight(getHeight())
	{
	}

	void AwesomiumWidget::_shutdown()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &AwesomiumWidget::notifyFrameStart);
		mControl->destroy();
		mControl = 0;

		requestUpdateCanvas = nullptr;

		shutdownWidgetSkin();

		Base::_shutdown();
	}

	void AwesomiumWidget::_initialise(MyGUI::WidgetStyle _style, const MyGUI::IntCoord& _coord, MyGUI::Align _align, MyGUI::ResourceSkin* _info, MyGUI::Widget* _parent, MyGUI::ICroppedRectangle * _croppedParent, MyGUI::IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

		initialiseWidgetSkin(_info);

		mControl = AwesomiumWidgetFactory::getCore()->createWebView(getWidth(), getHeight(), mIsTransparent, true, 90);
		mControl->setListener(this);

		createTexture(TRM_PT_VIEW_REQUESTED);
		requestUpdateCanvas = newDelegate(this, &AwesomiumWidget::notifyUpdateCanvas);

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &AwesomiumWidget::notifyFrameStart);
	}

	void AwesomiumWidget::baseChangeWidgetSkin(MyGUI::ResourceSkin* _info)
	{
		shutdownWidgetSkin();
		Base::baseChangeWidgetSkin(_info);
		initialiseWidgetSkin(_info);
	}

	void AwesomiumWidget::initialiseWidgetSkin(MyGUI::ResourceSkin* _info)
	{
	}

	void AwesomiumWidget::shutdownWidgetSkin()
	{
	}

	void AwesomiumWidget::loadURL(const std::string& _url)
	{
		mControl->loadURL(_url);
	}

	void AwesomiumWidget::setTransparent(bool _value)
	{
		mIsTransparent = _value;
		mControl->setTransparent(mIsTransparent);
	}

	void AwesomiumWidget::notifyUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
	{
		if (_event.widgetResized)
		{
			if (mOldWidth != getWidth() || mOldHeight != getHeight())
			{
				mOldWidth = getWidth();
				mOldHeight = getHeight();
				mControl->resize(getWidth(), getHeight());

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

	void AwesomiumWidget::notifyFrameStart(float _time)
	{
		update();
	}

	void AwesomiumWidget::update(bool _update)
	{
		if (!mControl->isDirty() && !_update)
			return;

		void* data = lock();
		mControl->render((MyGUI::uint8*)data, getTextureRealWidth() * 4, 4, 0);

		if (!mIsTransparent)
		{
			int count = getTextureRealWidth() * getTextureRealHeight();
			for (int index=0; index<count; ++index)
			{
				((unsigned char*)(&(((unsigned int*)data)[index])))[3] = 0xFF;
			}
		}

		unlock();
	}

	void AwesomiumWidget::onMouseDrag(int _left, int _top)
	{
		mControl->injectMouseMove(_left - getAbsoluteLeft(), _top - getAbsoluteTop());
		Base::onMouseDrag(_left, _top);
	}

	void AwesomiumWidget::onMouseMove(int _left, int _top)
	{
		mControl->injectMouseMove(_left - getAbsoluteLeft(), _top - getAbsoluteTop());
		Base::onMouseDrag(_left, _top);
	}

	void AwesomiumWidget::onMouseWheel(int _rel)
	{
		mControl->injectMouseWheel(_rel < 0 ? -10 : 10);
		Base::onMouseWheel(_rel);
	}

	void AwesomiumWidget::onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id)
	{
		mControl->injectMouseDown(Awesomium::MouseButton(_id.toValue()));
		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void AwesomiumWidget::onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id)
	{
		mControl->injectMouseUp(Awesomium::MouseButton(_id.toValue()));
		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void AwesomiumWidget::onKeyLostFocus(MyGUI::Widget* _new)
	{
		mControl->unfocus();
		Base::onKeyLostFocus(_new);
	}

	void AwesomiumWidget::onKeySetFocus(MyGUI::Widget* _old)
	{
		mControl->focus();
		Base::onKeySetFocus(_old);
	}

#if defined(_WIN32)
	void AwesomiumWidget::onChangeCursor(const HCURSOR& cursor)
	{
		static HCURSOR arrow_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
		static HCURSOR beam_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM));
		static HCURSOR link_cursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));

		if (mOldCursor != (size_t)cursor)
		{
			mOldCursor = (size_t)cursor;

			if (cursor == beam_cursor)
			{
				setPointer("beam");
				MyGUI::PointerManager::getInstance().setPointer("beam");
				MyGUI::PointerManager::getInstance().eventChangeMousePointer("beam");
			}
			else if (cursor == link_cursor)
			{
				setPointer("link");
				MyGUI::PointerManager::getInstance().setPointer("link");
				MyGUI::PointerManager::getInstance().eventChangeMousePointer("link");
			}
			else
			{
				setPointer("arrow");
				MyGUI::PointerManager::getInstance().setPointer("arrow");
				MyGUI::PointerManager::getInstance().eventChangeMousePointer("arrow");
			}
		}
	}
#endif

	void AwesomiumWidget::onChangeKeyboardFocus(bool isFocused)
	{
		if (isFocused)
			MyGUI::InputManager::getInstance().setKeyFocusWidget(this);
	}
		
	void AwesomiumWidget::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "Awesomium_SourceURL") loadURL(_value);
		else if (_key == "Awesomium_Transparent") setTransparent(MyGUI::utility::parseValue<bool>(_value));
		else Base::setProperty(_key, _value);
	}

} // namespace Awesomium

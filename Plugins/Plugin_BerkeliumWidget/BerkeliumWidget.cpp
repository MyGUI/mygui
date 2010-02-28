/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#include "BerkeliumWidget.h"
#include "MyGUI.h"

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

	void BerkeliumWidget::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
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

		std::string url = "http://www.youtube.com/watch?v=A37iIil3T68";
		mWindow->navigateTo(url.data(), url.length());

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

		mBuffer.update((void*)sourceBuffer, rect.left(), rect.top(), rect.width(), rect.height());
	}

	void BerkeliumWidget::notifyFrameStart(float _time)
	{
		if (mWindow == nullptr)
			return;

		Berkelium::update();
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

}

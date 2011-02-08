/*!
	@file
	@author		Albert Semenov
	@date		10/2009
*/
#include "HikariWidget.h"
#include "FlashSite.h"
#include "Plugin.h"
#include <objbase.h>

namespace Hikari
{

	HikariWidget::HikariWidget() :
		mControl(0)
	{
	}

	void HikariWidget::initialiseOverride()
	{
		Base::initialiseOverride();

		createTexture(TRM_PT_VIEW_REQUESTED);
		requestUpdateCanvas = newDelegate(this, &HikariWidget::notifyUpdateCanvas);

		mControl = new Hikari::FlashControl(this, plugin::Plugin::getLibHandle());
		updateTexture();

		MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &HikariWidget::notifyFrameStart);
	}

	void HikariWidget::shutdownOverride()
	{
		MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &HikariWidget::notifyFrameStart);

		requestUpdateCanvas = nullptr;
		delete mControl;
		mControl = 0;

		Base::shutdownOverride();
	}

	void HikariWidget::notifyUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
	{
		if (mControl)
		{
			bool resizeOnly = _event.widgetResized && !_event.requested && !_event.textureChanged;
			if (resizeOnly)
			{
				if (mControl->getWidth() != getWidth() || mControl->getHeight() != getHeight())
				{
					mControl->setSize(getWidth(), getHeight());
				}
			}
			else
			{
				if (mControl->getWidth() != getWidth() || mControl->getHeight() != getHeight())
				{
					mControl->setSize(getWidth(), getHeight());
				}
				else
				{
					mControl->invalidateTotally();
				}
			}

			mControl->update();
		}
	}

	void HikariWidget::notifyFrameStart(float _time)
	{
		if (mControl)
			mControl->update();
	}

	void HikariWidget::load(const std::string& movieFilename)
	{
		if (mControl)
			mControl->load(movieFilename);
	}

	void HikariWidget::play()
	{
		if (mControl)
			mControl->getFlashInterface()->Play();
	}

	void HikariWidget::stop()
	{
		if (mControl)
			mControl->getFlashInterface()->Stop();
	}

	void HikariWidget::rewind()
	{
		if (mControl)
			mControl->getFlashInterface()->Rewind();
	}

	void HikariWidget::gotoFrame(long frameNum)
	{
		if (mControl)
			mControl->getFlashInterface()->raw_GotoFrame(frameNum);
	}

	void HikariWidget::setLoop(bool shouldLoop)
	{
		if (mControl)
			mControl->getFlashInterface()->PutLoop(shouldLoop);
	}

	void HikariWidget::setTransparent(bool isTransparent)
	{
		if (mControl)
			mControl->setTransparent(isTransparent);
	}

	void HikariWidget::setQuality(short renderQuality)
	{
		if (mControl)
			mControl->setQuality(renderQuality);
	}

	void HikariWidget::setScaleMode(short scaleMode)
	{
		if (mControl)
			mControl->setScaleMode(scaleMode);
	}

	void HikariWidget::bind(const MyGUI::UString& funcName, FlashDelegate::IDelegate* callback)
	{
		if (mControl)
			mControl->bind(funcName, callback);
	}

	void HikariWidget::unbind(const MyGUI::UString& funcName)
	{
		if (mControl)
			mControl->unbind(funcName);
	}

	FlashValue HikariWidget::callFunction(MyGUI::UString funcName, const Arguments& args)
	{
		if (mControl)
			return mControl->callFunction(funcName, args);
		return FlashValue();
	}

	void HikariWidget::onMouseDrag(int _left, int _top, MyGUI::MouseButton _id)
	{
		if (mControl)
			mControl->injectMouseMove(_left - getAbsoluteLeft(), _top - getAbsoluteTop());
		Base::onMouseDrag(_left, _top, _id);
	}

	void HikariWidget::onMouseMove(int _left, int _top)
	{
		if (mControl)
			mControl->injectMouseMove(_left - getAbsoluteLeft(), _top - getAbsoluteTop());
		Base::onMouseMove(_left, _top);
	}

	void HikariWidget::onMouseWheel(int _rel)
	{
		const MyGUI::IntPoint& point = MyGUI::InputManager::getInstance().getMousePosition();
		if (mControl)
			mControl->injectMouseWheel(_rel, point.left - getAbsoluteLeft(), point.top - getAbsoluteTop());
		Base::onMouseWheel(_rel);
	}

	void HikariWidget::onMouseButtonPressed(int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left && mControl)
			mControl->injectMouseDown(_left - getAbsoluteLeft(), _top - getAbsoluteTop());
		Base::onMouseButtonPressed(_left, _top, _id);
	}

	void HikariWidget::onMouseButtonReleased(int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left && mControl)
			mControl->injectMouseUp(_left - getAbsoluteLeft(), _top - getAbsoluteTop());
		Base::onMouseButtonReleased(_left, _top, _id);
	}

	void HikariWidget::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "Source")
			load(_value);
		else if (_key == "Transparent")
			setTransparent(MyGUI::utility::parseValue<bool>(_value));
		else
		{
			Base::setProperty(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	FlashControl* HikariWidget::getControl()
	{
		return mControl;
	}

} // namespace Hikari

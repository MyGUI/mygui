/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Message.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_WidgetOIS.h"
#include "MyGUI_MessageFactory.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"

namespace MyGUI
{

	const float MESSAGE_ALPHA_MAX = 0.5f;
	const float MESSAGE_ALPHA_MIN = 0.0f;
	const float MESSAGE_SPEED_COEF = 3.0f;

	Message::Message(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Window(_coord, _align, _info, _parent, _name),
		mWidgetText(null),
		mInfoOk(Ok), mInfoCancel(Ok),
		mButton1Index(0),
		mSmooth(false),
		mWidgetFade(null)
	{
		// ищем индекс первой кнопки
		size_t but1 = (size_t)Button1;
		but1 >>= 1;
		while (0 != but1) {
			but1 >>= 1;
			mButton1Index++;
		}

		// парсим виджет для текста
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Text") {
				mWidgetText = (*iter);
				mOffsetText.set(mCoord.width - mWidgetText->getWidth(), mCoord.height - mWidgetText->getHeight());
			}
		}
		MYGUI_ASSERT(null != mWidgetText, "Child Text not found in skin (MessageBox must have widget for text)");

		// парсим свойства
		const MapString & param = _info->getParams();
		if (!param.empty()) {
			MapString::const_iterator iter = param.find("ButtonSkin");
			if (iter != param.end()) mButtonSkin = iter->second;
			iter = param.find("ButtonType");
			if (iter != param.end()) mButtonType = iter->second;
			iter = param.find("ButtonSize");
			if (iter != param.end()) mButtonSize = IntSize::parse(iter->second);
			iter = param.find("ButtonOffset");
			if (iter != param.end()) mButtonOffset = IntSize::parse(iter->second);
			iter = param.find("DefaultLayer");
			if (iter != param.end()) mDefaultLayer = iter->second;
			iter = param.find("FadeSkin");
			if (iter != param.end()) mFadeSkin = iter->second;
			iter = param.find("FadeLayer");
			if (iter != param.end()) mFadeLayer = iter->second;
		}

	}

	void Message::setMessage(const Ogre::DisplayString & _message)
	{
		mWidgetText->setCaption(_message);
		updateSize();
	}

	Message::ButtonInfo Message::addButtonName(const Ogre::DisplayString & _name)
	{
		if (mVectorButton.size() >= 7) {
			MYGUI_LOG(Warning, "in message box many buttons, ignored");
			return None;
		}
		// бит, номер кнопки + смещение до Button1
		ButtonInfo info = (ButtonInfo)(FLAG(mVectorButton.size() + mButton1Index));

		// запоминаем кнопки для отмены и подтверждения
		if (mVectorButton.empty()) mInfoOk = info;
		mInfoCancel = info;

		WidgetPtr button = createWidgetT(mButtonType, mButtonSkin, IntCoord(), ALIGN_LEFT | ALIGN_BOTTOM);
		button->eventMouseButtonClick = newDelegate(this, &Message::notifyButtonClick);
		button->setCaption(_name);
		button->_setInternalData((int)info);
		mVectorButton.push_back(button);

		updateSize();
		return info;
	}

	void Message::setButton(ButtonInfo _info)
	{
		clearButton();
		size_t current = 0;
		size_t info = (size_t)_info;

		while (0 != info) {
			if (0 != (info & 1)) {
				// если бит есть то ставим кнопку
				addButtonName(factory::MessageFactory::_getButtonName(current));
				// корректируем ее номер
				mVectorButton.back()->_setInternalData((int)FLAG(current));
			}
			info >>= 1;
			current ++;
		}

		updateSize();
	}

	void Message::updateSize()
	{
		IntSize size = mWidgetText->getTextSize(mWidgetText->getCaption()) + mOffsetText;
		int width = ((int)mVectorButton.size() * mButtonSize.width) + (((int)mVectorButton.size()+1) * mButtonOffset.width);
		int offset = size.width - width;
		if (size.width < width) {
			size.width = width;
			offset = 0;
		}
		offset += mButtonOffset.width;

		IntSize view((int)Gui::getInstance().getViewWidth(), (int)Gui::getInstance().getViewHeight());
		setPosition((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);

		for (VectorWidgetPtr::iterator iter=mVectorButton.begin(); iter!=mVectorButton.end(); ++iter) {
			(*iter)->setPosition(offset, mCoord.height - mButtonOffset.height, mButtonSize.width, mButtonSize.height);
			offset += mButtonOffset.width + mButtonSize.width;
		}
	}

	void Message::notifyButtonClick(MyGUI::WidgetPtr _sender, bool _double)
	{
		if (false == _double) _destroyMessage((ButtonInfo)_sender->_getInternalData());
	}

	void Message::clearButton()
	{
		for (VectorWidgetPtr::iterator iter=mVectorButton.begin(); iter!=mVectorButton.end(); ++iter) {
			WidgetManager::getInstance().destroyWidget(*iter);
		}
		mVectorButton.clear();
	}

	void Message::_onKeyButtonPressed(int _key, Char _char)
	{
		Window::_onKeyButtonPressed(_key, _char);
		if (_key == OIS::KC_RETURN) _destroyMessage(mInfoOk);
		else if (_key == OIS::KC_ESCAPE) _destroyMessage(mInfoCancel);
	}

	void Message::_destroyMessage(ButtonInfo _result)
	{
		eventMessageBoxEnd(this, _result);
		if (null != mWidgetFade) {
			if (mSmooth) ControllerManager::getInstance().addItem(mWidgetFade,
				new ControllerFadeAlpha(MESSAGE_ALPHA_MIN, MESSAGE_SPEED_COEF, ControllerFadeAlpha::ACTION_DESTROY, false));
			else WidgetManager::getInstance().destroyWidget(mWidgetFade);
		}
		if (mSmooth) destroySmooth();
		else WidgetManager::getInstance().destroyWidget(this);
	}

	void Message::setWindowSmooth(bool _smooth)
	{
		mSmooth = _smooth;
		if (mSmooth) showSmooth(true);
	}

	void Message::setWindowFade(bool _fade)
	{
		return; //пока пропустим

		if (_fade) {
			if (null == mWidgetFade) {
				Gui & gui = Gui::getInstance();
				mWidgetFade = gui.createWidgetT(Widget::getWidgetType(), mFadeSkin, IntCoord(0, 0, gui.getViewWidth(), gui.getViewHeight()), ALIGN_STRETCH, mFadeLayer);
				if (mSmooth) {
					mWidgetFade->hide();
					ControllerManager::getInstance().addItem(mWidgetFade,
						new ControllerFadeAlpha(MESSAGE_ALPHA_MAX, MESSAGE_SPEED_COEF, ControllerFadeAlpha::ACTION_NONE, false));
				}
				else mWidgetFade->setAlpha(MESSAGE_ALPHA_MAX);
			}
		}
		else {
			if (null != mWidgetFade) {
				WidgetManager::getInstance().destroyWidget(mWidgetFade);
				mWidgetFade = null;
			}
		}
	}

	void Message::setMessageImage(size_t _image)
	{
	}

	void Message::_createMessage(const Ogre::DisplayString & _caption, const Ogre::DisplayString & _message, size_t _image, const std::string & _skin, const std::string & _layer, bool _modal, EventMessageEnd * _delegate, ButtonInfo _info, const std::string & _button1, const std::string & _button2, const std::string & _button3, const std::string & _button4, const std::string & _button5, const std::string & _button6, const std::string & _button7)
	{
		Gui * gui = Gui::getInstancePtr();
		if (null == gui) return;

		MessagePtr mess = gui->createWidget<Message>(_skin.empty() ? factory::MessageFactory::_getDefaultSkin() : _skin, IntCoord(), ALIGN_DEFAULT, _layer);
		mess->setWindowSmooth(true);
		mess->setCaption(_caption);
		mess->setMessage(_message);
		mess->setMessageImage(_image);
		if (_modal) mess->setWindowFade(true);
		if (null != _delegate) mess->eventMessageBoxEnd = _delegate;
		if (None != _info) mess->setButton(_info);

		if (false == _button1.empty()) {
			mess->addButtonName(_button1);
			if (false == _button2.empty()) {
				mess->addButtonName(_button2);
				if (false == _button3.empty()) {
					mess->addButtonName(_button3);
					if (false == _button4.empty()) {
						mess->addButtonName(_button4);
						if (false == _button5.empty()) {
							mess->addButtonName(_button5);
							if (false == _button6.empty()) {
								mess->addButtonName(_button6);
								if (false == _button7.empty()) {
									mess->addButtonName(_button7);
								}
							}
						}
					}
				}
			}
		}

		if (_layer.empty()) LayerManager::getInstance().attachItem(mess, mess->getDefaultLayer(), true);
		if (_modal) InputManager::getInstance().addWidgetModal(mess);
	}

} // namespace MyGUI

/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Message.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_WidgetOIS.h"
#include "MyGUI_MessageFactory.h"

namespace MyGUI
{

	Message::Message(const IntCoord& _coord, char _align, const WidgetSkinInfoPtr _info, CroppedRectanglePtr _parent, const Ogre::String & _name) :
		Window(_coord, _align, _info, _parent, _name),
		mWidgetText(null),
		mInfoOk(Ok), mInfoCancel(Ok),
		mButton1Index(0)
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

		setSize(size);

		for (VectorWidgetPtr::iterator iter=mVectorButton.begin(); iter!=mVectorButton.end(); ++iter) {
			(*iter)->setPosition(offset, mCoord.height - mButtonOffset.height, mButtonSize.width, mButtonSize.height);
			offset += mButtonOffset.width + mButtonSize.width;
		}
	}

	void Message::notifyButtonClick(MyGUI::WidgetPtr _sender, bool _double)
	{
		if (false == _double) eventMessageBoxEnd(this, (ButtonInfo)_sender->_getInternalData());
		WidgetManager::getInstance().destroyWidget(this);
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
		if (_key == OIS::KC_RETURN) {
			eventMessageBoxEnd(this, mInfoOk);
			WidgetManager::getInstance().destroyWidget(this);
		}
		else if (_key == OIS::KC_ESCAPE) {
			eventMessageBoxEnd(this, mInfoCancel);
			WidgetManager::getInstance().destroyWidget(this);
		}
	}

} // namespace MyGUI

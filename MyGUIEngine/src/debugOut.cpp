	//------------------------------------------------------------------------------//
	// Окно отладки [my.name (my.work(a)inbox.ru)]
	//------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------//
	#include "debugOut.h"
	#include <OgreFontManager.h>
	//------------------------------------------------------------------------------//
	debugOverlay::debugOverlay & debug = debugOverlay::debugOverlay::Instance(); // единственный экземпляр отладочного оверлея
	//------------------------------------------------------------------------------//
	namespace debugOverlay {
	//------------------------------------------------------------------------------//
	//------------------------------------------------------------------------------//
	// реализация класса debugOverlayInfo
	//------------------------------------------------------------------------------//
	void debugOverlayInfo::createOverlayInfo(const String & name, uint16 x, uint16 y, void * value)
	{
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		debugOverlay & _debug = debugOverlay::Instance();

		m_strName = name;
		m_value = value;

		m_overlayContainerShadow = 0;
		#if __DEBUG_SHOW_SHADOW == 1
			m_overlayContainerShadow = static_cast< OverlayContainer*>(overlayManager.createOverlayElement("TextArea", "__overlayInfoShadow_" + name, false));
			m_overlayContainerShadow->setMetricsMode(GMM_PIXELS);
			m_overlayContainerShadow->setLeft(x+1);
			m_overlayContainerShadow->setTop(y+1);
			m_overlayContainerShadow->setWidth(__DEBUG_OVERLAY_INFO_WIDTH);
			m_overlayContainerShadow->setHeight(__DEBUG_FONT_SIZE + 3);
			m_overlayContainerShadow->setParameter("font_name", _debug.m_strFontName);
			m_overlayContainerShadow->setParameter("char_height", Ogre::StringConverter::toString(__DEBUG_FONT_SIZE) );
			m_overlayContainerShadow->setParameter("colour_top", __DEBUG_FONT_SHADOW_COLOUR);
			m_overlayContainerShadow->setParameter("colour_bottom", __DEBUG_FONT_SHADOW_COLOUR);
			_debug.m_overlay->add2D(m_overlayContainerShadow); // добавляем контейнер на оверлей
		#endif
		m_overlayContainer = static_cast< OverlayContainer*>(overlayManager.createOverlayElement("TextArea", "__overlayInfo_" + name, false));
		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setLeft(x);
		m_overlayContainer->setTop(y);
		m_overlayContainer->setWidth(__DEBUG_OVERLAY_INFO_WIDTH);
		m_overlayContainer->setHeight(__DEBUG_FONT_SIZE + 3);
		m_overlayContainer->setParameter("font_name", _debug.m_strFontName);
		m_overlayContainer->setParameter("char_height", Ogre::StringConverter::toString(__DEBUG_FONT_SIZE) );
		m_overlayContainer->setParameter("colour_top", __DEBUG_FONT_COLOUR);
		m_overlayContainer->setParameter("colour_bottom", __DEBUG_FONT_COLOUR);
		_debug.m_overlay->add2D(m_overlayContainer); // добавляем контейнер на оверлей

	}
	//------------------------------------------------------------------------------//
	void debugOverlayInfo::destroyOverlayInfo()
	{
		debugOverlay & _debug = debugOverlay::Instance();
		_debug.m_overlay->remove2D(m_overlayContainer);
		if (m_overlayContainerShadow) _debug.m_overlay->remove2D(m_overlayContainerShadow);

		OverlayManager & manager = Ogre::OverlayManager::getSingleton();
		manager.destroyOverlayElement(m_overlayContainer);
		if (m_overlayContainerShadow) manager.destroyOverlayElement(m_overlayContainerShadow);
	}
	//------------------------------------------------------------------------------//
	const String & debugOverlayInfo::getName()
	{
		return m_strName;
	}
	//------------------------------------------------------------------------------//
	// реализация класса debugOverlay
	//------------------------------------------------------------------------------//
	debugOverlay::debugOverlay() {}
	//------------------------------------------------------------------------------//
	debugOverlay & debugOverlay::Instance()
	{
		static debugOverlay instance;
		return instance;
	}
	//------------------------------------------------------------------------------//
	debugOverlay::~debugOverlay()
	{
		// оверлеи не удаляем их должен огр удалить
		for (uint16 pos=0; pos<mOverlayInfo.size(); pos++) {
	//		mOverlayInfo[pos]->destroyOverlayInfo();
			delete mOverlayInfo[pos];
		}
		mOverlayInfo.clear();
	}
	//------------------------------------------------------------------------------//
	void debugOverlay::show(bool _show)
	{
		_show?m_overlay->show():m_overlay->hide();
	}
	//------------------------------------------------------------------------------//
	void debugOverlay::out(char* fmt, ... )
	{
		check(); // проверяем создание

		try {
			if (fmt == "") return;
			#if __DEBUG_SHOW_SHADOW == 1
				if (!m_overlayDebugInfoShadow) return;
			#endif
			if (!m_overlayDebugInfo) return;

			static uint16 num = 0; // порядковый номер сообщения
			static uint16 count = 0; // колличество одинаковых сообщений
			static UTFString stringOld;
			UTFString stringAll, _string;

			va_list args;
			va_start( args, fmt );
			vsprintf( &m_stringDebugBuffer[0], fmt, args ); // форматирование строки
			va_end( args );

			stringAll = m_stringDebugBuffer;
			if (stringAll == stringOld) {// одинаковые сообщения
				count ++;
				::sprintf(m_stringDebugBuffer, "[%d, %d]   ", num, count+1);
			} else {
				count = 0;
				::sprintf(m_stringDebugBuffer, "[%d]   ", num);
			}

			_string = m_stringDebugBuffer;
			stringOld = stringAll; // сохраняем последнюю строчку
			num ++;

			_string = _string + stringAll; // приписываем порядковый номер
			_string = _string + "\n";

			if (count == 0) { // строка изменилась
				// обрезаем верхнюю строчку
				if (m_overlayDebugInfo->getCaption().size() > m_countCharInLine[0]) { // зищита
					stringAll = m_overlayDebugInfo->getCaption().substr(m_countCharInLine[0]);
				} else stringAll = "";
				// циклический сдвиг вверх
				for (uint16 pos=1; pos<__DEBUG_COUNT_LINE; pos++) m_countCharInLine[pos-1] = m_countCharInLine[pos];
				// длинна текущей строки
				m_countCharInLine[__DEBUG_COUNT_LINE-1] = (uint16)_string.size();
			} else { // строка повторяется
				// обрезаем нижнюю строчку
				stringAll = m_overlayDebugInfo->getCaption();
				stringAll.resize(stringAll.size()-m_countCharInLine[__DEBUG_COUNT_LINE-1]);
				// длинна текущей строки
				m_countCharInLine[__DEBUG_COUNT_LINE-1] = (uint16)_string.size();
			}

			stringAll = stringAll + _string;
			#if __DEBUG_SHOW_SHADOW == 1
				m_overlayDebugInfoShadow->setCaption(stringAll); // и выводим
			#endif
			m_overlayDebugInfo->setCaption(stringAll); // и выводим

		} catch (...) {}
	}
	//------------------------------------------------------------------------------//
	// для упрощеной регистрации
	#define debugOverlay_add(_type) \
	class debugOverlayInfo_##_type : public debugOverlayInfo \
	{ \
	public: \
		bool update() \
		{ \
			try { \
				if ( ::IsBadReadPtr( (const void*)m_value, sizeof(_type) ) ) return false; \
			} catch (...) { return false;} \
			String strOut = m_strName + " = " + StringConverter::toString( *((_type*)m_value) ); \
			if (m_overlayContainer->getCaption() != strOut) { \
				m_overlayContainer->setCaption(strOut); \
				if (m_overlayContainerShadow) m_overlayContainerShadow->setCaption(strOut); \
			} \
			return true; \
		}; \
	}; \
	void debugOverlay::add(const String & name, uint16 x, uint16 y, _type & value) \
	{ \
		check(); \
		remove(name); \
		debugOverlayInfo * info = new debugOverlayInfo_##_type(); \
		info->createOverlayInfo(name, x, y, (void*)&value); \
		mOverlayInfo.push_back(info); \
	}
	//------------------------------------------------------------------------------//
	debugOverlay_add(bool);
	debugOverlay_add(char);
	debugOverlay_add(uint8);
	debugOverlay_add(short);
	debugOverlay_add(uint16);
	debugOverlay_add(int);
	debugOverlay_add(uint32);
	debugOverlay_add(Real);
	debugOverlay_add(Vector2);
	debugOverlay_add(Vector3);
	debugOverlay_add(Vector4);
	debugOverlay_add(Quaternion);
	debugOverlay_add(Matrix3);
	debugOverlay_add(Matrix4);
	debugOverlay_add(Radian);
	debugOverlay_add(Degree);
	debugOverlay_add(ColourValue);
	//------------------------------------------------------------------------------//
	void debugOverlay::remove(const String & name)
	{
		for (uint16 pos=0; pos<mOverlayInfo.size(); pos++) {
			if (mOverlayInfo[pos]->getName() == name) {

				mOverlayInfo[pos]->destroyOverlayInfo();
				delete mOverlayInfo[pos];
				mOverlayInfo[pos] = mOverlayInfo[mOverlayInfo.size()-1];
				mOverlayInfo.pop_back();

				return;
			}
		}
	}
	//------------------------------------------------------------------------------//
	bool debugOverlay::frameStarted(const Ogre::FrameEvent& evt)
	{
		// просто обновляем все поля
		for (uint16 pos=0; pos<mOverlayInfo.size(); pos++) {
			try {
				if ( ! mOverlayInfo[pos]->update()) {
					// если вышли неудачно, то удаляем это поле
					mOverlayInfo[pos]->destroyOverlayInfo();
					delete mOverlayInfo[pos];
					mOverlayInfo[pos] = mOverlayInfo[mOverlayInfo.size()-1];
					mOverlayInfo.pop_back();

					return true; // лучше выйти
				}
			} catch (...) {}
		}
		return true;
	}
	//------------------------------------------------------------------------------//
	bool debugOverlay::frameEnded(const Ogre::FrameEvent& evt)
	{
		return true;
	}
	//------------------------------------------------------------------------------//
	void debugOverlay::check()
	{
		static bool bIsCheck = false;
		if (bIsCheck) return;

		// подписываемся на обновления
		Ogre::Root::getSingleton().addFrameListener(this);

		// ищем шрифт
		Ogre::ResourceManager::ResourceMapIterator fonts = Ogre::FontManager::getSingleton().getResourceIterator();
		assert(fonts.hasMoreElements()); // шрифтов чета нет =(

		// первый шрифт
		m_strFontName = fonts.getNext().getPointer()->getName();
		// ищем дефолтный, если нет то будет первый
		while (fonts.hasMoreElements()) {
			const String & name = fonts.getNext().getPointer()->getName();
			if (name == __DEBUG_FONT_DEFAULT) m_strFontName = __DEBUG_FONT_DEFAULT;
		}

		::memset((void*)m_countCharInLine, 0, __DEBUG_COUNT_LINE*sizeof(uint16));
		// создаем оверлей, если не был создан
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlay = overlayManager.create(__DEBUG_OVERLAY_NAME);
		m_overlay->setZOrder(__DEBUG_OVERLAY_ZORDER);
		RenderWindow * win = Root::getSingleton().getAutoCreatedWindow();

		#if __DEBUG_SHOW_SHADOW == 1
			// текстовое поле,  тень
			m_overlayDebugInfoShadow = static_cast< OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_SHADOW_NAME, false));
			m_overlayDebugInfoShadow->setMetricsMode(GMM_PIXELS);
			m_overlayDebugInfoShadow->setLeft(__DEBUG_OVERLAY_OFFSET + 1);
			m_overlayDebugInfoShadow->setTop(__DEBUG_OVERLAY_OFFSET + 1);
			m_overlayDebugInfoShadow->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET-1);
			m_overlayDebugInfoShadow->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET-1);
			m_overlayDebugInfoShadow->setParameter("font_name", m_strFontName);
			m_overlayDebugInfoShadow->setParameter("char_height", Ogre::StringConverter::toString(__DEBUG_FONT_SIZE) );
			m_overlayDebugInfoShadow->setParameter("colour_top", __DEBUG_FONT_SHADOW_COLOUR);
			m_overlayDebugInfoShadow->setParameter("colour_bottom", __DEBUG_FONT_SHADOW_COLOUR);
			m_overlay->add2D(m_overlayDebugInfoShadow); // добавляем контейнер на оверлей
		#endif
		// текстовое поле
		m_overlayDebugInfo = static_cast< OverlayContainer*>(overlayManager.createOverlayElement("TextArea", __DEBUG_OVERLAY_ELEMENT_NAME, false));
		m_overlayDebugInfo->setMetricsMode(GMM_PIXELS);
		m_overlayDebugInfo->setLeft(__DEBUG_OVERLAY_OFFSET);
		m_overlayDebugInfo->setTop(__DEBUG_OVERLAY_OFFSET);
		m_overlayDebugInfo->setWidth(win->getWidth()-__DEBUG_OVERLAY_OFFSET);
		m_overlayDebugInfo->setHeight(win->getHeight()-__DEBUG_OVERLAY_OFFSET);
		m_overlayDebugInfo->setParameter("font_name", m_strFontName);
		m_overlayDebugInfo->setParameter("char_height", Ogre::StringConverter::toString(__DEBUG_FONT_SIZE) );
		m_overlayDebugInfo->setParameter("colour_top", __DEBUG_FONT_COLOUR);
		m_overlayDebugInfo->setParameter("colour_bottom", __DEBUG_FONT_COLOUR);

		m_overlay->add2D(m_overlayDebugInfo); // добавляем контейнер на оверлей
		m_overlay->show();

		bIsCheck = true;
	}
	//------------------------------------------------------------------------------//
	} // namespace debugOverlay 
	//------------------------------------------------------------------------------//

#include "MyGUI_Window.h"
#include "MyGUI_OIS.h"
#include "MyGUI_GUI.h"
#include "MyGUI_AssetManager.h"

#include <OgreStringConverter.h>
#include <OgreOverlayManager.h>
#include <OgreTechnique.h>
#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>

using namespace Ogre;
using namespace std;
using namespace OIS;

namespace MyGUI {
	GUI *GUI::Initialize(uint16 uWidth, uint16 uHeight, EventCallback *pEventCallback)
	{
		m_uWidth = (uWidth);
		m_uHeight = (uHeight);
		m_uMaxZOrder = (__GUI_ZORDER_OVERLAPPED); // начало дл€ перекрывающихс€ окон
		m_uOverlappedStart = (0);
		m_uOverlappedEnd = (0);
		m_pEventCallback = (pEventCallback);
		m_bFadeState = (FADE_END);
		m_currentFadeWindow = (0);
		m_pEventCallbackFade = (0);
		
		_LOG("create MyGui object (%p) ,  %d x %d\r\n", this, uWidth, uHeight);

		m_overlayGUI[OVERLAY_FADE] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_FADE), __GUI_ZORDER_FADE, false);
		m_overlayGUI[OVERLAY_MOUSE] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_MOUSE), __GUI_ZORDER_MOUSE);
		m_overlayGUI[OVERLAY_POPUP] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_POPUP), __GUI_ZORDER_POPUP);
		m_overlayGUI[OVERLAY_MAIN] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_MAIN), __GUI_ZORDER_MAIN);
		m_overlayGUI[OVERLAY_BACK] = createOverlay("MyGUI_"+StringConverter::toString(OVERLAY_BACK), __GUI_ZORDER_BACK);

		AssetManager::getSingleton()->loadAssets();

		createMousePointer();
		setMousePointer(POINTER_DEFAULT);
		
		return this;
	}
	
	void GUI::Shutdown()
	{
		AssetManager::getSingleton()->unloadAssets();
		while (mRootWindows.size() > 0)
		    destroyWindow(mRootWindows[0]);
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayGUI[OVERLAY_MOUSE]->remove2D(m_overlayContainerMouse);
		overlayManager.destroyOverlayElement(m_overlayContainerMouse);
		for (uint16 pos=0; pos<__OVERLAY_COUNT; pos++) overlayManager.destroy(m_overlayGUI[pos]);

		_LOG("\r\ndestroy MyGui object (%p)", this);
		
	}

	void GUI::destroyWindow(__MYGUI_OVERLAYS overlay) // уничтожает окно и удал€ет из списка
	{
		if (overlay == OVERLAY_DESTROY_ALL)
			while (mRootWindows.size() > 0)
			    destroyWindow(mRootWindows[0]);
		else if (overlay == OVERLAY_OVERLAPPED)
			while (m_uOverlappedStart < m_uOverlappedEnd)
			    destroyWindow(mRootWindows[m_uOverlappedEnd-1]);
		else if ((overlay < __OVERLAY_COUNT) && (overlay != OVERLAY_MOUSE))
			for(size_t pos = 0, size = mRootWindows.size(); pos < size; ++pos)
				if (mRootWindows[pos]->m_overlay == m_overlayGUI[overlay])
				{
					destroyWindow(mRootWindows[pos]);
					pos--;
					size --;
				}
	}

	void GUI::destroyWindow(MyGUI::Window * pWindow) // уничтожает окно и удал€ет из списка
	{
	    /****************************
	    Code from the destructor for a window
	    
	    // удал€ем дитешек
		while(!mChildWindows.empty())
		{
		    delete mChildWindows.back();
		    mChildWindows.pop_back();
		}

		_LOG("destroy window (%p)", this);

		// а теперь и сами
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		if (m_overlayCaption) {
			m_overlayContainer->removeChild(m_overlayCaption->getName());
			overlayManager.destroyOverlayElement(m_overlayCaption);
		}

		if (m_overlay) {
			m_overlay->remove2D(m_overlayContainer);
			overlayManager.destroyOverlayElement(m_overlayContainer);
			if (m_bIsOverlapped) overlayManager.destroy(m_overlay); // перекрывающеес€ окно
		} else {
			__ASSERT(m_pWindowParent != 0); // во как
			m_pWindowParent->m_overlayContainer->removeChild(m_overlayContainer->getName()); // дочернее окно
			overlayManager.destroyOverlayElement(m_overlayContainer);
		}
		
		***********************************/
		if (!pWindow)
		    return;

		// сброс всех активных окон, в конце функции все восстановитс€ кроме захвата мыши
		ResetFocusedWindows();
		
		size_t size;
		if (pWindow->m_pWindowParent) { // есть отец
			for(Window::ChildWindowsIterator x = pWindow->m_pWindowParent->mChildWindows.begin();
                x < pWindow->m_pWindowParent->mChildWindows.end(); ++x)
            {
			    if(*x == pWindow)
			        pWindow->m_pWindowParent->mChildWindows.erase(x);
			}
						        
			delete pWindow;
		} else  { // ребенок гуи
			upZOrder(pWindow); // поднимаем его
			
			size = mRootWindows.size();
			for (size_t i = 0; i < size; i++) { // ищем в детском саду
				if (pWindow != mRootWindows[i])
				    continue;
				    
				//mChildWindows[i] is now the current window
				if (i != 0) { // [we shall try to inform the previous window on that that it now above all overlapped]
					Window * pWindowBack = mRootWindows[i-1];
					if (pWindowBack->m_bIsOverlapped) pWindowBack->_OnUpZOrder();
				}

				if (pWindow->m_bIsOverlapped) { // есть свой оверлей
					m_uMaxZOrder -= __GUI_ZORDER_OVERLAPPED_STEP; // уменьшаем оверлеи
					m_uOverlappedEnd --;
				} else { // принадлежит оверле€м гуи
					__ASSERT(pWindow->m_overlay != 0); // как так
					if (pWindow->m_overlay->getZOrder() < __GUI_ZORDER_OVERLAPPED) { // слои ниже перекрывающихс€
						m_uOverlappedStart --;
						m_uOverlappedEnd --;
					}
				}

				for (size_t pos = i; pos < (size-1); pos++)
				    mRootWindows[pos] = mRootWindows[pos+1];
				mRootWindows.pop_back();

				i = size; // выходим
			}
		}
		
		delete pWindow;

		// the miscalculation of activity of windows, for correctness of display
        // eventMouseMove (m_overlayContainerMouse-> getLeft ()/m_uWidth, 0.0, m_overlayContainerMouse-> getTop ()/m_uHeight, false);
        // it is sent at and * 22 
		
		if (m_pEventCallback)
		    m_pEventCallback->onWarningEvent(pWindow, WE_WARNING_CHILD_DELETE);
	}

	void GUI::createMousePointer()
	{
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		m_overlayContainerMouse = static_cast<OverlayContainer*>(overlayManager.createOverlayElement("Panel", "MyGUI_overlayContainerMouse"));
		m_overlayContainerMouse->setMetricsMode(GMM_PIXELS);
		m_overlayContainerMouse->setPosition(0.0, 0.0);
		m_overlayContainerMouse->setDimensions(
		    AssetManager::getSingleton()->Pointers()->getDefinition(POINTER_DEFAULT)->uSizeX,
            AssetManager::getSingleton()->Pointers()->getDefinition(POINTER_DEFAULT)->uSizeY);
		
		m_overlayContainerMouse->show();
		m_overlayGUI[OVERLAY_MOUSE]->add2D(m_overlayContainerMouse);
		m_overlayGUI[OVERLAY_MOUSE]->show();

	}

	void GUI::setMousePointer(const String &TypePointer) // изменить указатель
	{
		if (AssetManager::getSingleton()->Pointers()->getDefinition(TypePointer)->strMaterialName == "")
		    return;
		m_overlayContainerMouse->setMaterialName(
		    AssetManager::getSingleton()->Pointers()->getDefinition(TypePointer)->strMaterialName);
		m_iCurrentOffsetCursorX = AssetManager::getSingleton()->Pointers()->getDefinition(TypePointer)->iOffsetX;
		m_iCurrentOffsetCursorY = AssetManager::getSingleton()->Pointers()->getDefinition(TypePointer)->iOffsetY;
	}

	void GUI::upZOrder(Window *pWindow)
	{
		//No need to change Z order since it isn't being overlapped
		if (!pWindow->m_bIsOverlapped)
		    return;
		
		//It is being overlapped by other windows
		for (size_t pos = m_uOverlappedStart; pos < m_uOverlappedEnd - 1; pos++) {
			if (pWindow == mRootWindows[pos]) { // если это наше окно, то мен€ем его с верхним
				mRootWindows[pos] = mRootWindows[pos+1];
				mRootWindows[pos+1] = pWindow;
				uint16 uZOrder = pWindow->m_overlay->getZOrder();
				pWindow->m_overlay->setZOrder(mRootWindows[pos]->m_overlay->getZOrder());
				mRootWindows[pos]->m_overlay->setZOrder(uZOrder);
			}
		}
	}

	void GUI::eventWindowResize(const unsigned int uWidth, const unsigned int uHeight) // изменилс€ размер главного окна
	{
		m_uWidth = uWidth;
		m_uHeight = uHeight;
		
		//The fade effect needs to cover the entire viewable area, so we need to resize it
		if (m_currentFadeWindow) m_currentFadeWindow->size(uWidth, uHeight);
		
		//Resize all the overlay sheets the manager is using
		Overlay::Overlay2DElementsIterator iter = m_overlayGUI[MyGUI::OVERLAY_FADE]->get2DElementsIterator();
		if (iter.hasMoreElements()) {
			iter.getNext()->setDimensions(uWidth, uHeight); // может еще и данные размеров класса Window помен€ть
		}
	}

	void GUI::eventUpdateAnimation(Real fTimeLastFrame) // обновл€ет анимацию гуи
	{
		if (m_bFadeState != FADE_END) {
			if (m_bFadeState == FADE_UP) { // рассвет
				m_fCurrentFadeAlpha -= fTimeLastFrame;
				if (m_fCurrentFadeAlpha <= 0.0) {
					m_bFadeState = FADE_END;
					m_overlayGUI[OVERLAY_FADE]->hide(); // скрываем оверлей файдинга
					destroyWindow(m_currentFadeWindow); // уничтожаем окно затемнени€
					m_currentFadeWindow = 0;
					if (m_pEventCallbackFade) m_pEventCallbackFade->onFadeEnd(false, m_uFadeID);
					else if (m_pEventCallback) m_pEventCallback->onFadeEnd(false, m_uFadeID);
					// просчет активности окон, дл€ правильности отображени€
//					eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
				} else { // покажем изменени€
					MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
					TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
					texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);
				}
			} else { // закат
				m_fCurrentFadeAlpha += fTimeLastFrame;
				if (m_fCurrentFadeAlpha >= 1.0) {
					m_bFadeState = FADE_END;
					m_overlayGUI[OVERLAY_FADE]->hide(); // скрываем оверлей файдинга
					destroyWindow(m_currentFadeWindow); // уничтожаем окно затемнени€
					m_currentFadeWindow = 0;
					if (m_pEventCallbackFade) m_pEventCallbackFade->onFadeEnd(true, m_uFadeID);
					else if (m_pEventCallback) m_pEventCallback->onFadeEnd(true, m_uFadeID);
					// просчет активности окон, дл€ правильности отображени€
//					eventMouseMove(m_overlayContainerMouse->getLeft()/m_uWidth, 0.0, m_overlayContainerMouse->getTop()/m_uHeight, false);
				} else {  // покажем изменени€
					MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
					TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
					texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);
				}
			}
		}

	}

	void GUI::fadeScreen(bool bIsFade, FADE_STATES uFadeID, EventCallback *pEventCallback) // затен€ем экран
	{
		__ASSERT(m_currentFadeWindow == 0);
		
		m_pEventCallbackFade = pEventCallback; // дл€ вызова функции
		if (bIsFade) { // закат
			m_bFadeState = FADE_DOWN;
			m_fCurrentFadeAlpha = 0.0;
		} else { //APPEAR - по€витьс€
			m_bFadeState = FADE_UP;
			m_fCurrentFadeAlpha = 1.0;
		}
		
		//Be sure that the fade screen is visible at the appropriate level
		m_overlayGUI[OVERLAY_FADE]->show(); // показываем оверлей файдинга
		m_overlayGUI[OVERLAY_FADE]->setZOrder(__GUI_ZORDER_FADE); // поднимает вверх
		
		m_currentFadeWindow = create<Window>(0, 0, m_uWidth, m_uHeight, OVERLAY_FADE, SKIN_FADE);

		MaterialPtr Mat = m_currentFadeWindow->m_overlayContainer->getMaterial();
		TextureUnitState* texunit = Mat->getTechnique(0)->getPass(0)->getTextureUnitState(0);
		texunit->setAlphaOperation(LBX_MODULATE, LBS_TEXTURE, LBS_MANUAL, 1.0, m_fCurrentFadeAlpha);
	}

	Overlay * GUI::createOverlay(String strName, uint16 zOrder, bool bIsShow) // создает оверлей
	{
		_LOG("create Overlay '%s' \t zOrder = %d", strName.c_str(), zOrder);

		static String old_name;
		OverlayManager &overlayManager = OverlayManager::getSingleton();
		Overlay * pOverlay = overlayManager.create(strName);
		pOverlay->setZOrder(zOrder);
		if (bIsShow) pOverlay->show();

		// !!! To not change a code
		// напоминаем старому окну его место
		Overlay * pOldOverlay = 0;
		try {pOldOverlay = overlayManager.getByName(old_name);} catch (...) {}
		if (pOldOverlay) {
			pOldOverlay->setZOrder(pOldOverlay->getZOrder());
		}
		old_name = strName;
		return pOverlay;
	}

}
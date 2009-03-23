#ifndef __BASIS_STATE_H__
#define __BASIS_STATE_H__

#include <OIS/OIS.h>

//=====================================================================================
// базовый класс для всех стейтов
//===================================================================================
class BasisState
{
public:
	BasisState() {};
	virtual ~BasisState() {};

	virtual void enter(bool bIsChangeState) = 0; // обязательно, в этом методе вы должны создать все свои данные для начала стейта
	virtual void exit() = 0; // обязательно, в этом методе вы должны уничтожить все что было созданно в методе enter()
	virtual void pause() {}; // не обязательно, здесь вы должны спрятать или уничтожить все объекты
	virtual void resume() {}; // не обязательно, здесь вы должны показать или создать все что вы изменили в методе pause()

	virtual bool frameStarted(const Ogre::FrameEvent& evt) {return true;}
	virtual bool frameEnded(const Ogre::FrameEvent& evt) {return true;}
	virtual bool keyPressed( const OIS::KeyEvent &arg ) {return true;}
	virtual bool keyReleased( const OIS::KeyEvent &arg ) {return true;}
	virtual bool mouseMoved( const OIS::MouseEvent &arg ) {return true;}
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}

	virtual void windowResize() {}; // уведомление об изменении размеров окна рендера
};
//===================================================================================

#endif // __BASIS_STATE_H__

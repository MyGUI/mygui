#ifndef __BASIS_STATE_H__
#define __BASIS_STATE_H__

#include <OIS/OIS.h>

//=====================================================================================
// ������� ����� ��� ���� �������
//===================================================================================
class BasisState
{
public:
	BasisState() {};
	virtual ~BasisState() {};

	virtual void enter(bool bIsChangeState) = 0; // �����������, � ���� ������ �� ������ ������� ��� ���� ������ ��� ������ ������
	virtual void exit() = 0; // �����������, � ���� ������ �� ������ ���������� ��� ��� ���� �������� � ������ enter()
	virtual void pause() {}; // �� �����������, ����� �� ������ �������� ��� ���������� ��� �������
	virtual void resume() {}; // �� �����������, ����� �� ������ �������� ��� ������� ��� ��� �� �������� � ������ pause()

	virtual bool frameStarted(const Ogre::FrameEvent& evt) {return true;}
	virtual bool frameEnded(const Ogre::FrameEvent& evt) {return true;}
	virtual bool keyPressed( const OIS::KeyEvent &arg ) {return true;}
	virtual bool keyReleased( const OIS::KeyEvent &arg ) {return true;}
	virtual bool mouseMoved( const OIS::MouseEvent &arg ) {return true;}
	virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}
	virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ) {return true;}

	virtual void windowResize() {}; // ����������� �� ��������� �������� ���� �������
};
//===================================================================================

#endif // __BASIS_STATE_H__


#ifndef _DEBUGOUT_H_
#define _DEBUGOUT_H_

#include "Prerequest.h"

	//------------------------------------------------------------------------------//
	// ���� ������� [my.name (my.work(a)inbox.ru)]
	//------------------------------------------------------------------------------//
	#pragma once
	//------------------------------------------------------------------------------//
	#include <Ogre.h>
	//------------------------------------------------------------------------------//
	using namespace Ogre;
	//------------------------------------------------------------------------------//
	// ����������� �����
	#define __DEBUG_COUNT_LINE 30
	// ���������� �� ���� ������
	#define __DEBUG_SHOW_SHADOW  1
	// ����� �� ���������
	#define __DEBUG_FONT_DEFAULT "BlueHighway"
	// ������ ������
	#define __DEBUG_FONT_SIZE 16
	// ���� ������
	#define __DEBUG_FONT_COLOUR "1.0 1.0 1.0"
	// ���� ������
	#define __DEBUG_FONT_SHADOW_COLOUR "0.0 0.0 0.0"
	// ������ ������� �������
	#define __DEBUG_OVERLAY_ZORDER 649
	// ��� ������� �������
	#define __DEBUG_OVERLAY_NAME "__debugOverlay"
	// ��� ��������
	#define __DEBUG_OVERLAY_ELEMENT_NAME "__debugOverlayElement"
	// ��� �������� ����
	#define __DEBUG_OVERLAY_ELEMENT_SHADOW_NAME "__debugOverlayElementShadow"
	// �������� �� ����
	#define __DEBUG_OVERLAY_OFFSET 10
	// ������ ���� ������
	#define __DEBUG_OVERLAY_INFO_WIDTH 500
	//------------------------------------------------------------------------------//
	namespace debugOverlay {
	//------------------------------------------------------------------------------//
	// ����� ������������� �������������� �����
	//------------------------------------------------------------------------------//
	class debugOverlayInfo
	{
	public:
		void createOverlayInfo(const String & name, uint16 x, uint16 y, void * value);
		void destroyOverlayInfo();

		virtual bool update() = 0;
		inline const String & getName();

	protected:
		OverlayContainer * m_overlayContainer;
		OverlayContainer * m_overlayContainerShadow; // ��� ���� ������
		String m_strName; // ��� ���� ������
		void * m_value; // ��������� �� ���������� ������
	};
	//------------------------------------------------------------------------------//
	// �������� ����� ��� ������ ����������
	//------------------------------------------------------------------------------//
	class debugOverlay : public Ogre::FrameListener
	{
	public: 
		// ������������ ���������
		static debugOverlay & Instance();
		// ����� ��� ������ ����������
		void out(char* fmt, ... );
		// ������ ��� ���������� ����� ������
		void add(const String & name, uint16 x, uint16 y, bool & value);
		void add(const String & name, uint16 x, uint16 y, char & value);
		void add(const String & name, uint16 x, uint16 y, uint8 & value);
		void add(const String & name, uint16 x, uint16 y, short & value);
		void add(const String & name, uint16 x, uint16 y, uint16 & value);
		void add(const String & name, uint16 x, uint16 y, int & value);
		void add(const String & name, uint16 x, uint16 y, uint32 & value);
		void add(const String & name, uint16 x, uint16 y, Real & value);
		void add(const String & name, uint16 x, uint16 y, Vector2 & value);
		void add(const String & name, uint16 x, uint16 y, Vector3 & value);
		void add(const String & name, uint16 x, uint16 y, Vector4 & value);
		void add(const String & name, uint16 x, uint16 y, Quaternion & value);
		void add(const String & name, uint16 x, uint16 y, Matrix3 & value);
		void add(const String & name, uint16 x, uint16 y, Matrix4 & value);
		void add(const String & name, uint16 x, uint16 y, Radian & value);
		void add(const String & name, uint16 x, uint16 y, Degree & value);
		void add(const String & name, uint16 x, uint16 y, ColourValue & value);
		// ����� ��� �������� ���� ������
		void remove(const String & name);
		// ����� ��� ������� ������ �������
		void show(bool _show = true);

		~debugOverlay();
		// ��� ���������� ������
		bool frameStarted(const Ogre::FrameEvent& evt);
		bool frameEnded(const Ogre::FrameEvent& evt);

	private:
		// ��� �������
		friend class debugOverlayInfo;

		debugOverlay();
		void check(); // ��������� ��� �� ��������, ���� ���, �� �������

	private:
		std::vector<debugOverlayInfo*> mOverlayInfo; // ������ ���� ����� ������
		Overlay * m_overlay; // ������� ��� ���� ����� , ���� ����� =)
		String m_strFontName; // ��� ������
		uint16 m_countCharInLine[__DEBUG_COUNT_LINE]; // ����������� �������� � �������
		char m_stringDebugBuffer[1024]; // ����� ��� �������������� ������
		OverlayContainer* m_overlayDebugInfo;
		#if __DEBUG_SHOW_SHADOW == 1
			OverlayContainer* m_overlayDebugInfoShadow; // ��� ���� ������
		#endif
	};
	//------------------------------------------------------------------------------//
	} // namespace debugOverlay 
	//------------------------------------------------------------------------------//
	extern debugOverlay::debugOverlay & debug;
	//------------------------------------------------------------------------------//


#endif
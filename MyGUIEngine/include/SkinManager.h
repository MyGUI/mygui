#ifndef _SKINMANAGER_H_
#define _SKINMANAGER_H_


#include "Prerequest.h"
#include "Instance.h"
#include <string>
#include "WidgetDefines.h"


namespace MyGUI
{

	class _MyGUIExport SkinManager
	{
		INSTANCE_HEADER(SkinManager);

	private:
		SkinManager();
		~SkinManager();

	public:

		char parseAlign(const std::string & _value);
		WidgetSkinInfo * getSkin(const Ogre::String & _name);

		//	��� ������� �������� �����
		WidgetSkinInfo * create(const Ogre::String & _name);

		void load(const std::string & _file);

		static FloatSize getMaterialSize(const std::string & _material);
		// ������������ �� ���������� ��������� � ����������, � Rect ������ ������ � ������
		static FloatRect convertMaterialCoord(const FloatRect & _source, const FloatSize & _materialSize);

	private:
		void initialise();
		void createDefault();

	private:
		SkinInfo m_skins;
		MapFlags m_mapAlign;

	}; // class SkinManager

} // namespace MyGUI

#endif

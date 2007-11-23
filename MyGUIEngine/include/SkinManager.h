#ifndef __SKIN_MANAGER_H__
#define __SKIN_MANAGER_H__

#include "Prerequest.h"
#include "Instance.h"
#include "WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport SkinManager
	{
		INSTANCE_HEADER(SkinManager);

	public:
		void initialise();
		void shutdown();

		Align parseAlign(const std::string & _value);
		WidgetSkinInfo * getSkin(const Ogre::String & _name);

		//	��� ������� �������� �����
		WidgetSkinInfo * create(const Ogre::String & _name);

		void load(const std::string & _file);

		static FloatSize getMaterialSize(const std::string & _material);
		// ������������ �� ���������� ��������� � ����������, � Rect ������ ������ � ������
		static FloatRect convertMaterialCoord(const FloatRect & _source, const FloatSize & _materialSize);


	private:
		void createDefault();

	private:
		MapWidgetSkinInfoPtr m_skins;
		MapFlags m_mapAlign;

	}; // class SkinManager

} // namespace MyGUI

#endif // __SKIN_MANAGER_H__

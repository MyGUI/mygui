/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_CROPPED_RECTANGLE_MANAGER_H__
#define __MYGUI_CROPPED_RECTANGLE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_BasisWidget.h"
#include "MyGUI_BasisWidgetFactoryInterface.h"

#include "MyGUI_MainSkin.h"
#include "MyGUI_SubSkin.h"
#include "MyGUI_TextSimple.h"
#include "MyGUI_TextEdit.h"

namespace MyGUI
{
	class _MyGUIExport CroppedRectangleManager
	{
		INSTANCE_HEADER(CroppedRectangleManager);

	public:
		void initialise();
		void shutdown();

		// создает сабвиджет использу€ фабрику
		CroppedRectangleBase * createCroppedRectangle(const CroppedRectangleInfo &_info, const Ogre::String & _material, CroppedRectangleBase * _parent, size_t & _id);

		// подходит ли данный тип дл€ объединени€
		bool isSharedOverlay(const CroppedRectangleInfo &_info);

		inline void registerFactory(CroppedRectangleFactoryInterface * _factory)
		{
			mFactoryList.push_back(_factory);
		}

	protected:
		std::list<CroppedRectangleFactoryInterface*> mFactoryList;

		CroppedRectangleFactory<MainSkin> * mFactoryMainSkin;
		CroppedRectangleFactory<SubSkin> * mFactorySubSkin;
		CroppedRectangleFactory<TextSimple> * mFactoryTextSimple;
		CroppedRectangleFactory<TextEdit> * mFactoryTextEdit;

	}; // CroppedRectangleManager

} // namespace MyGUI

#endif // __MYGUI_CROPPED_RECTANGLE_MANAGER_H__
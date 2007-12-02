/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_CROPPED_RECTANGLE_FACTORY_INTERFACE_H__
#define __MYGUI_CROPPED_RECTANGLE_FACTORY_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_BasisWidget.h"

namespace MyGUI
{

	class _MyGUIExport CroppedRectangleFactoryInterface
	{
	public:
		virtual void getNextId(size_t & _id) = 0;
		virtual const Ogre::String & getType() = 0;
		virtual CroppedRectangleInterface * createCroppedRectangle(const CroppedRectangleInfo &_info, const Ogre::String & _material, CroppedRectangleInterface * _parent, size_t & _id) = 0;
	};

	template <class ClassName>
	class CroppedRectangleFactory : public CroppedRectangleFactoryInterface
	{
	public:
		void getNextId(size_t & _id) {if (ClassName::_isSharedOverlay())_id++;}
		const Ogre::String & getType() {return ClassName::_getType();};
		CroppedRectangleInterface * createCroppedRectangle(const CroppedRectangleInfo& _info, const Ogre::String& _material, CroppedRectangleInterface* _parent, size_t & _id)
		{
			ClassName * obj = new ClassName(_info, _material, _parent, _id);
			getNextId(_id);
			return  obj;
		}
	}; // class CroppedRectangleFactory : public CroppedRectangleFactoryInterface

} // namespace MyGUI

#endif // __MYGUI_CROPPED_RECTANGLE_FACTORY_INTERFACE_H__
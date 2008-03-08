/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/
#ifndef __MYGUI_COLOUR_RECT_H__
#define __MYGUI_COLOUR_RECT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport ColourRect : public SubSkin
	{

	public:
		ColourRect(const SubWidgetInfo &_info, CroppedRectanglePtr _parent);
		virtual ~ColourRect();

		virtual void setAlpha(float _alpha);

		virtual void setColourAlign(const Ogre::ColourValue & _colour, Align _align);
		virtual const Ogre::ColourValue & getColourAlign(Align _align);

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex, bool _update);

	private:
		Ogre::ColourValue mColourLeftTop;
		Ogre::ColourValue mColourRightTop;
		Ogre::ColourValue mColourRightBottom;
		Ogre::ColourValue mColourLeftBottom;

		uint32 mRenderColourLeftTop;
		uint32 mRenderColourRightTop;
		uint32 mRenderColourRightBottom;
		uint32 mRenderColourLeftBottom;
	};

} // namespace MyGUI

#endif // __MYGUI_COLOUR_RECT_H__

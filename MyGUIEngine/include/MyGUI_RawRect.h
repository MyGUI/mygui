/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_RAW_RECT_H__
#define __MYGUI_RAW_RECT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_CroppedRectangleInterface.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class RenderItem;

	class _MyGUIExport RawRect : public SubSkin
	{

	public:
		RawRect(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent);
		virtual ~RawRect();

		virtual void setAlpha(float _alpha);

		void setRectColour(const Ogre::ColourValue & _colourLT, const Ogre::ColourValue & _colourRT, const Ogre::ColourValue & _colourLB, const Ogre::ColourValue & _colourRB);

		void serRectTexture(const FloatPoint & _pointLT, const FloatPoint & _pointRT, const FloatPoint & _pointLB, const FloatPoint & _pointRB);

		virtual void _setStateData(StateInfo * _data);

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex, bool _update);

		// метод для генерации данных из описания xml
		static StateInfo * createStateData(xml::xmlNodePtr _node, xml::xmlNodePtr _root);

	private:
		FloatPoint mRectTextureLT;
		FloatPoint mRectTextureRT;
		FloatPoint mRectTextureLB;
		FloatPoint mRectTextureRB;

		Ogre::ColourValue mColourLT;
		Ogre::ColourValue mColourRT;
		Ogre::ColourValue mColourLB;
		Ogre::ColourValue mColourRB;

		uint32 mRenderColourLT;
		uint32 mRenderColourRT;
		uint32 mRenderColourLB;
		uint32 mRenderColourRB;
	};

} // namespace MyGUI

#endif // __MYGUI_RAW_RECT_H__

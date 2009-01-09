/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_RAW_RECT_H__
#define __MYGUI_RAW_RECT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Colour.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Types.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_DrawItem.h"
#include "MyGUI_SubSkin.h"

namespace MyGUI
{

	class RenderItem;

	class MYGUI_EXPORT RawRect : public SubSkin
	{
		MYGUI_RTTI_CHILD_HEADER(RawRect, SubSkin);

	public:
		RawRect(const SubWidgetInfo &_info, ICroppedRectangle * _parent);
		virtual ~RawRect();

		virtual void setAlpha(float _alpha);

		void setRectColour(const Colour& _colourLT, const Colour& _colourRT, const Colour& _colourLB, const Colour& _colourRB);

		void setRectTexture(const FloatPoint & _pointLT, const FloatPoint & _pointRT, const FloatPoint & _pointLB, const FloatPoint & _pointRB);

		virtual void _setStateData(StateInfo * _data);

		// метод для отрисовки себя
		virtual size_t _drawItem(Vertex * _vertex, bool _update);

		// метод для генерации данных из описания xml
		static StateInfo * createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version);

	private:
		FloatPoint mRectTextureLT;
		FloatPoint mRectTextureRT;
		FloatPoint mRectTextureLB;
		FloatPoint mRectTextureRB;

		Colour mColourLT;
		Colour mColourRT;
		Colour mColourLB;
		Colour mColourRB;

		uint32 mRenderColourLT;
		uint32 mRenderColourRT;
		uint32 mRenderColourLB;
		uint32 mRenderColourRB;

		bool mRenderGL;

	};

} // namespace MyGUI

#endif // __MYGUI_RAW_RECT_H__

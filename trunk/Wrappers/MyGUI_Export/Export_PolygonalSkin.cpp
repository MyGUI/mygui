/*!
	@file
	@author		Albert Semenov
	@date		02/2013
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_PolygonalSkin.h"

namespace Export
{

	namespace ScopePolygonalSkin_SetWidth
	{
		MYGUIEXPORT void MYGUICALL ExportPolygonalSkin_SetWidth(
			MyGUI::Widget* _widget,
			Convert<float>::Type _width)
		{
			MyGUI::ISubWidget* main = _widget->getSubWidgetMain();
			if (main != nullptr)
			{
				MyGUI::PolygonalSkin* skin = main->castType<MyGUI::PolygonalSkin>(false);
				if (skin != nullptr)
					skin->setWidth(_width);
			}
		}
	}

	namespace ScopePolygonalSkin_SetPoints
	{
		MYGUIEXPORT void MYGUICALL ExportPolygonalSkin_SetPoints(
			MyGUI::Widget* _widget,
			Convert<void*>::Type _points,
			Convert<int>::Type _count)
		{
			MyGUI::ISubWidget* main = _widget->getSubWidgetMain();
			if (main != nullptr)
			{
				MyGUI::PolygonalSkin* skin = main->castType<MyGUI::PolygonalSkin>(false);
				if (skin != nullptr)
				{
					MyGUI::FloatPoint* points = (MyGUI::FloatPoint*)_points;
					std::vector<MyGUI::FloatPoint> pointsArray(_count);
					for (int index = 0; index < _count; index ++)
						pointsArray[index] = points[index];
					skin->setPoints(pointsArray);
				}
			}
		}
	}

}

/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_DEFINES_H__
#define __MYGUI_WIDGET_DEFINES_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_Types.h"
#include "MyGUI_Align.h"
//#include <OgreColourValue.h>

namespace MyGUI
{

	typedef void * SubWidgetStateInfoPtr;
	/*struct SubWidgetStateInfoPtr
	{
		SubWidgetStateInfoPtr() :
			data(0)
		{
		}

		SubWidgetStateInfoPtr(void * _data) :
			data(_data)
		{
		}

		void * data;
	};*/

	struct WidgetStateInfo
	{
	public:
		inline void resize(size_t _count)
		{
			data.resize(_count);
		}

		inline SubWidgetStateInfoPtr getStateData(size_t _index) const
		{
			return data[_index];
		}

		inline void set(size_t _index, SubWidgetStateInfoPtr _data)
		{
			data.at(_index) = _data;
		}

		inline void clear()
		{
			for (std::vector<SubWidgetStateInfoPtr>::iterator iter=data.begin(); iter!=data.end(); ++iter) {
				delete *iter;
				*iter = 0;
			}
		}

	private:
		// колличество в векторе, равно колличеству сабскинов
		// порядок в векторе, равен порядку сабскинов, вне зависимости от типов
		std::vector<SubWidgetStateInfoPtr> data;
	};

	struct SubWidgetInfo
	{
		SubWidgetInfo(const std::string & _type, const IntCoord& _coord, Align _align) :
			coord(_coord),
			align(_align),
			type(_type)
		{
		}

		IntCoord coord;
		Align align;
		std::string type;
	};

	typedef std::map<std::string, SubWidgetStateInfoPtr> MapSubWidgetStateInfo;
	typedef std::map<std::string, WidgetStateInfo> MapWidgetStateInfo;
	typedef std::vector<SubWidgetInfo> VectorSubWidgetInfo;

	class WidgetSkinInfo;
	typedef WidgetSkinInfo * WidgetSkinInfoPtr;
	typedef std::map<std::string, WidgetSkinInfoPtr> MapWidgetSkinInfoPtr;

	class CroppedRectangleInterface;
	/*typedef CroppedRectangleInterface * CroppedRectangleInterface *;
	typedef std::vector<CroppedRectangleInterface *> VectorCroppedRectanglePtr;*/

	class SubWidgetInterface;
	//typedef SubWidgetInterface * SubWidgetPtr;
	typedef std::vector<SubWidgetInterface*> VectorSubWidget;

	class SubWidgetTextInterface;
	//typedef SubWidgetTextInterface * SubWidgetTextInterfacePtr;

	class Widget;
	typedef Widget * WidgetPtr;
	typedef std::vector<WidgetPtr> VectorWidgetPtr;
	typedef std::map<std::string, WidgetPtr> MapWidgetPtr;
	typedef Enumerator<VectorWidgetPtr> EnumeratorWidgetPtr;

} // namespace MyGUI

#endif // __MYGUI_WIDGET_DEFINES_H__

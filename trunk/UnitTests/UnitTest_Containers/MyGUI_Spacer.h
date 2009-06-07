/*!
	@file
	@author		Alexander Ptakhin
	@date		02/2009
	@module
*/
#ifndef __MYGUI_SPACER_H__
#define __MYGUI_SPACER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Spacer.h"

namespace MyGUI
{
	namespace factory
	{
		class SpacerFactory;
	}

	class Spacer;
	typedef Spacer* SpacerPtr;
		
	/** Spacer
	*/
	class /*MYGUI_EXPORT*/ Spacer : public Widget
	{
	protected:

		friend class factory::SpacerFactory;

		MYGUI_RTTI_DERIVED( Spacer );


		Spacer( WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name );

		WidgetPtr baseCreateWidget( WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Align _align, const std::string& _layer, const std::string& _name );

	public:	

	};
} // namespace MyGUI

#endif // __MYGUI_SPACER_H__

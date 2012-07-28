/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _6561a945_ae72_4279_988c_99c42a82a99a_
#define _6561a945_ae72_4279_988c_99c42a82a99a_

#include "DataListBaseControl.h"

namespace tools
{

	class ImageListControl :
		public DataListBaseControl
	{
	public:
		ImageListControl();
		virtual ~ImageListControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
	};

}

#endif

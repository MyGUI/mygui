/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _6561a945_ae72_4279_988c_99c42a82a99a_
#define _6561a945_ae72_4279_988c_99c42a82a99a_

#include "Control.h"
#include "Data.h"

namespace tools
{
	class ImageListControl :
		public Control
	{
	public:
		ImageListControl();
		virtual ~ImageListControl();

		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		bool checkCommand(bool _result);

		void commandCreateData(const MyGUI::UString& _commandName, bool& _result);
		void commandDestroyData(const MyGUI::UString& _commandName, bool& _result);
		void commandRenameData(const MyGUI::UString& _commandName, bool& _result);

	private:
		size_t mNameIndex;
	};
}

#endif

/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _6561a945_ae72_4279_988c_99c42a82a99a_
#define _6561a945_ae72_4279_988c_99c42a82a99a_

#include "Control.h"
#include "sigslot.h"

namespace tools
{
	class ImageListControl :
		public Control,
		public sigslot::has_slots<>
	{
	public:
		ImageListControl();
		virtual ~ImageListControl();

		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		bool checkCommand();

		void commandCreateData(const MyGUI::UString& _commandName, bool& _result);
		void commandDestroyData(const MyGUI::UString& _commandName, bool& _result);
		void commandRenameData(const MyGUI::UString& _commandName, bool& _result);

		void notifyChangeData();

	private:
		size_t mIndex;
		MyGUI::ListBox* mListBox;
	};
}

#endif

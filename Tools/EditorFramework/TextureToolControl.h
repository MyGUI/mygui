/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _535c1422_4ed5_44f0_a04c_83a1cb68b28b_
#define _535c1422_4ed5_44f0_a04c_83a1cb68b28b_

#include "TextureControl.h"

namespace tools
{

	class MYGUI_EXPORT_DLL TextureToolControl :
		public TextureControl,
		public sigslot::has_slots<>
	{
	public:
		TextureToolControl();
		virtual ~TextureToolControl();

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

		virtual void onMouseWheel(int _rel);

		bool checkCommand();
		bool checkMenuCommand();

	private:
		void notifySettingsChanged(const std::string& _path);

		void CommandChangeNextScale(const MyGUI::UString& _commandName, bool& _result);
		void CommandChangePrevScale(const MyGUI::UString& _commandName, bool& _result);
		void CommandChangeScale(const MyGUI::UString& _commandName, bool& _result);

		bool doPrevScale();
		bool doNextScale();

	private:
		typedef std::vector<size_t> VectorSizeT;
		VectorSizeT mScaleValue;
		size_t mCurrentScaleValue;
		std::string mColourValueName;
	};

}

#endif

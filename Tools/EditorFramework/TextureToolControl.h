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

	class MYGUI_EXPORT_DLL TextureToolControl : public TextureControl, public sigslot::has_slots<>
	{
	public:
		~TextureToolControl() override;

	protected:
		void OnInitialise(Control* _parent, MyGUI::Widget* _place, std::string_view _layoutName) override;

		void onMouseWheel(int _rel) override;

		bool checkCommand();
		bool checkMenuCommand();

	private:
		void notifySettingsChanged(std::string_view _path);

		void CommandChangeNextScale(const MyGUI::UString& _commandName, bool& _result);
		void CommandChangePrevScale(const MyGUI::UString& _commandName, bool& _result);
		void CommandChangeScale(const MyGUI::UString& _commandName, bool& _result);

		bool doPrevScale();
		bool doNextScale();

	private:
		using VectorSizeT = std::vector<size_t>;
		VectorSizeT mScaleValue;
		size_t mCurrentScaleValue{100};
		std::string mColourValueName;
	};

}

#endif

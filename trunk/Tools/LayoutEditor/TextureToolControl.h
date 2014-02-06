/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _17446f2a_aef5_4dfd_a0dd_bff3d3dc18cf_
#define _17446f2a_aef5_4dfd_a0dd_bff3d3dc18cf_

#include "TextureControl.h"
#include "sigslot.h"

namespace tools
{

	class TextureToolControlLE :
		public TextureControlLE,
		public sigslot::has_slots<>
	{
	public:
		TextureToolControlLE(MyGUI::Widget* _parent);
		virtual ~TextureToolControlLE();

		void setActivate(bool _value);
		bool getActivate() const;

	protected:
		virtual void onMouseWheel(int _rel);

		virtual void onChangeActivate();

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

		bool mActivate;
	};

}

#endif

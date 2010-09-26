/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __TEXTURE_TOOL_CONTROL_H__
#define __TEXTURE_TOOL_CONTROL_H__

#include "TextureControl.h"

namespace tools
{

	class TextureToolControl :
		public TextureControl
	{
	public:
		TextureToolControl(MyGUI::Widget* _parent);
		virtual ~TextureToolControl();

	protected:
		virtual void onMouseWheel(int _rel);

	private:
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		void Command_ChangeNextScale(const MyGUI::UString& _commandName, bool& _result);
		void Command_ChangePrevScale(const MyGUI::UString& _commandName, bool& _result);
		void Command_ChangeScale(const MyGUI::UString& _commandName, bool& _result);

		bool checkCommand();
		bool checkMenuCommand();

		bool doPrevScale();
		bool doNextScale();

	private:
		typedef std::vector<size_t> VectorSizeT;
		VectorSizeT mScaleValue;
		size_t mCurrentScaleValue;
	};

} // namespace tools

#endif // __TEXTURE_TOOL_CONTROL_H__

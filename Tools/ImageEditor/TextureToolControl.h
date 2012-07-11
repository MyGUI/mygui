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

		void setActivate(bool _value);
		bool getActivate() const;

	protected:
		virtual void onMouseWheel(int _rel);

		virtual void onChangeActivate();

		bool checkCommand();
		bool checkMenuCommand();

	private:
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

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

} // namespace tools

#endif // __TEXTURE_TOOL_CONTROL_H__

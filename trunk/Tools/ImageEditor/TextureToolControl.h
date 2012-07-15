/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#ifndef _535c1422_4ed5_44f0_a04c_83a1cb68b28b_
#define _535c1422_4ed5_44f0_a04c_83a1cb68b28b_

#include "TextureControl.h"

/*namespace tools
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

}*/

#endif

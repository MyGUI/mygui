/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __BACKGROUND_CONTROL_H__
#define __BACKGROUND_CONTROL_H__

#include "BaseLayout/BaseLayout.h"
#include "Property.h"
#include "ColourPanel.h"

namespace tools
{

	class BackgroundControl :
		public wraps::BaseLayout
	{
	public:
		BackgroundControl(MyGUI::Widget* _parent);
		virtual ~BackgroundControl();

		MyGUI::Widget* getCanvas();

	private:
		void setColour(const MyGUI::Colour& _colour);
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

	private:
		MyGUI::Widget* mBackground;
		MyGUI::Widget* mCanvas;
	};

} // namespace tools

#endif // __BACKGROUND_CONTROL_H__

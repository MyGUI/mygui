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

		MyGUI::Widget* getCanvas()
		{
			return mCanvas;
		}

	private:
		void notifyComboChangePosition(MyGUI::ComboBox* _sender, size_t _index);
		void notifyMouseButtonClick(MyGUI::Widget* _sender);

		void notifyChangePosition();
		void notifyEndDialog(Dialog* _sender, bool _result);

		void fillColours(MyGUI::ComboBox* _combo);
		void updateColourByPresets();

		void updateColours();

	private:
		MyGUI::ComboBox* mBackgroundColour;
		MyGUI::Widget* mBackground;
		MyGUI::Widget* mBackgroundButton;
		MyGUI::Widget* mCanvas;

		ColourPanel* mColourPanel;

		MyGUI::Colour mCurrentColour;
	};

} // namespace tools

#endif // __BACKGROUND_CONTROL_H__

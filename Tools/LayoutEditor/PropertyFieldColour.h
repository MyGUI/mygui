/*!
	@file
	@author		Albert Semenov
	@date		12/2010
*/

#ifndef _c05c3f75_ceee_4525_89ac_a8462ec97634_
#define _c05c3f75_ceee_4525_89ac_a8462ec97634_

#include "EditorToolTip.h"
#include "BaseLayout/BaseLayout.h"
#include "IPropertyField.h"
#include "ColourPanel.h"
#include "sigslot.h"

namespace tools
{

	class PropertyFieldColour :
		public wraps::BaseLayout,
		public IPropertyField,
		public sigslot::has_slots<>
	{
	public:
		PropertyFieldColour(MyGUI::Widget* _parent);
		virtual ~PropertyFieldColour();

		virtual void initialise(const std::string& _type);

		virtual void setTarget(MyGUI::Widget* _currentWidget);
		virtual void setValue(const std::string& _value);
		virtual void setName(const std::string& _value);

		virtual void setVisible(bool _value);
		virtual bool getVisible();

		virtual MyGUI::IntSize getContentSize();
		virtual void setCoord(const MyGUI::IntCoord& _coord);

	private:
		void notifyApplyProperties(MyGUI::Widget* _sender, bool _force);
		void notifyTryApplyProperties(MyGUI::EditBox* _sender);
		void notifyForceApplyProperties(MyGUI::EditBox* _widget);
		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyEndDialog(Dialog* _sender, bool _result);
		void notifyPreviewColour(const MyGUI::Colour& _value);

		void updateColourPlace(bool _success);
		MyGUI::Colour getColour();
		void setColour(const MyGUI::Colour& _color, bool _final);
		void showColourDialog();

	protected:
		virtual bool onCheckValue();
		virtual void onAction(const std::string& _value, bool _final);

	private:
		MyGUI::TextBox* mText;
		MyGUI::EditBox* mField;
		MyGUI::Widget* mColourPlace;
		MyGUI::Widget* mCurrentWidget;
		std::string mType;
		std::string mName;
		ColourPanel* mColourPanel;
		MyGUI::Colour mPreviewColour;
		bool mGoodData;
	};

}

#endif

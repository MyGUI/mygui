/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _695efc39_8196_4b64_bc11_76874525dba1_
#define _695efc39_8196_4b64_bc11_76874525dba1_

#include "BaseLayout/BaseLayout.h"
#include "IFactoryItem.h"

namespace tools
{
	class Control :
		public wraps::BaseLayout,
		public components::IFactoryItem
	{
	public:
		Control();
		virtual ~Control();

		MyGUI::Widget* getRoot();

		void Initialise(const std::string& _layoutName);

		typedef std::vector<Control*> VectorControl;
		const VectorControl& getChilds() const;

		void SendCommand(const std::string& _command);

	protected:
		virtual void OnInitialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);
		virtual void OnCommand(const std::string& _command);

	private:
		void CreateChilds(Control* _parent, MyGUI::Widget* _widget);
		void AdviceWidget(MyGUI::Widget* _widget);

		void notifyMouseButtonClick(MyGUI::Widget* _sender);
		void notifyTabChangeSelect(MyGUI::TabControl* _sender, size_t _index);
		void notifyWindowButtonPressed(MyGUI::Window* _sender, const std::string& _name);
		void notifyEditSelectAccept(MyGUI::EditBox* _sender);

	private:
		VectorControl mChilds;
		Control* mParent;
	};
}

#endif

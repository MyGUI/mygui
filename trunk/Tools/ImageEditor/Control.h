/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _8e62f979_bea3_49b2_a4e0_69158c9b8797_
#define _8e62f979_bea3_49b2_a4e0_69158c9b8797_

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
		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

		typedef std::vector<Control*> VectorControl;
		const VectorControl& getChilds() const;

	protected:
		virtual void OnCommand(const std::string& _commandName, MyGUI::Any _data = MyGUI::Any::Null);

	private:
		void CreateChilds(Control* _parent, MyGUI::Widget* _widget);

		void notifyMouseButtonClick(MyGUI::Widget* _sender);

	private:
		VectorControl mChilds;
		Control* mParent;
	};
}

#endif

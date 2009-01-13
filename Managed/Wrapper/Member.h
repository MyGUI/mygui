/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_H__
#define __MEMBER_H__

#include <MyGUI.h>

namespace wrapper
{

	class Member
	{
	public:
		typedef std::vector<std::string> VectorString;
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;

	public:
		Member() { }

		Member(MyGUI::xml::ElementPtr _element, const std::string& _id)
		{
			mName = getItemName(_element);
			mKind = _element->findAttribute("kind");
			mId = _element->findAttribute("refid");
		}

		const std::string& getKind() { return mKind; }
		const std::string& getName() { return mName; }
		const std::string& getId() { return mId; }

		void insertToTemplates(const VectorPairString & _templates)
		{
			for (VectorPairString::const_iterator item=_templates.begin(); item!=_templates.end(); ++item) {
				insertToTemplate(item->second);
			}
		}

	private:
		std::string getItemName(MyGUI::xml::ElementPtr _element)
		{
			MyGUI::xml::ElementEnumerator child_item = _element->getElementEnumerator();
			while (child_item.next("name"))
				return child_item->getContent();
			return "";
		}

		virtual void insertToTemplate(const std::string& _template) { }


	protected:
		std::string mName;
		std::string mKind;
		std::string mId;
	};

} // namespace wrapper

#endif // __MEMBER_H__

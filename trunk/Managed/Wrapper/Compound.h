/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __COMPOUND_H__
#define __COMPOUND_H__

#include "XmlDocument.h"

namespace wrapper
{

	class Compound
	{
	public:
		typedef std::vector<std::string> VectorString;
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;
		typedef std::vector<Compound*> VectorCompound;
		typedef utility::Enumerator<VectorCompound> Enumerator;

	public:
		Compound() { }

		Compound(xml::ElementPtr _element, const std::string& _type)
		{
			mType = _type;
			mName = getItemName(_element);
			mKind = _element->findAttribute("kind");
			mId = _element->findAttribute("refid");

			xml::ElementEnumerator child_item = _element->getElementEnumerator();
			while (child_item.next())
			{
				if (child_item->getName() == "compound")
					mChilds.push_back(new Compound(child_item.current(), child_item->getName()));
				else if (child_item->getName() == "member")
					mChilds.push_back(new Compound(child_item.current(), child_item->getName()));
			}
		}

		~Compound()
		{
			Enumerator childs = Enumerator(mChilds.begin(), mChilds.end());
			while (childs.next()) delete childs.current();
			mChilds.clear();
		}

		const std::string& getKind() { return mKind; }
		const std::string& getName() { return mName; }
		const std::string& getId() { return mId; }
		const std::string& getType() { return mType; }

		Enumerator getEnumerator() { return Enumerator(mChilds.begin(), mChilds.end()); }

	private:
		std::string getItemName(xml::ElementPtr _element)
		{
			xml::ElementEnumerator child_item = _element->getElementEnumerator();
			while (child_item.next("name"))
				return child_item->getContent();
			return "";
		}

	protected:
		std::string mName;
		std::string mKind;
		std::string mId;
		std::string mType;
		VectorCompound mChilds;
	};

} // namespace wrapper

#endif // __COMPOUND_H__

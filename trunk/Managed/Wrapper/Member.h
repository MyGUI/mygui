/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MEMBER_H__
#define __MEMBER_H__

#include "Compound.h"
#include "ITypeHolder.h"
#include "Utility.h"

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

		Member(xml::ElementPtr _element)
		{
			mName = getItemContentName(_element, "name");
			mType = getItemContentName(_element, "type");
			mKind = _element->findAttribute("kind");
			mId = _element->findAttribute("id");
		}

		const std::string& getName() { return mName; }
		const std::string& getType() { return mType; }
		const std::string& getKind() { return mKind; }
		const std::string& getId() { return mId; }

		void setName(const std::string& _name) { mName = _name; }

		// �������� ���� � ������
		virtual void insertToTemplate(const std::string& _template, ITypeHolder * _holder) { }

		// ��������� ������ ���������, ���� �������� true �� ������� ���������
		virtual bool postProccesing(Member* _member) { return false; }

	private:
		std::string getItemContentName(xml::ElementPtr _element, const std::string& _tag)
		{
			xml::ElementEnumerator child_item = _element->getElementEnumerator();
			while (child_item.next(_tag))
			{
				xml::ElementEnumerator iter = child_item->getElementEnumerator();
				while (iter.next())
				{
					if (child_item->getContent2().empty())
					{
						return iter->getContent() + " " + child_item->getContent();
					}
					else
					{
						return child_item->getContent() + " " + iter->getContent() + " " + child_item->getContent2();
					}
				}
				return child_item->getContent();
			}
			return "";
		}

	protected:
		std::string mName;
		std::string mType;
		std::string mKind;
		std::string mId;
	};

} // namespace wrapper

#endif // __MEMBER_H__

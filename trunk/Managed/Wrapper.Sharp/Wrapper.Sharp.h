/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include "Utility.h"
#include "ClassHolder.h"

namespace wrapper
{

	class Wrapper : public ICommonTypeHolder
	{
	public:
		typedef std::vector<ClassAttribute> VectorClassAttribute;

	public:
		Wrapper() : mRoot(nullptr) { }

		virtual VectorPairString getTypeInfo(const std::string& _type)
		{
			MapVectorPairString::const_iterator type = mPairTypeInfo.find( utility::trim_result(_type) );
			if (type == mPairTypeInfo.end()) return VectorPairString();
			return type->second;
		}

		bool initialise()
		{
			xml::Document doc;
			const std::string filename = "Data/data.xml";
			if ( !doc.open(filename) )
			{
				std::cout << doc.getLastError() << std::endl;
				return false;
			}
		
			xml::ElementEnumerator child_item = doc.getRoot()->getElementEnumerator();
			while (child_item.next())
			{
				if (child_item->getName() == "Item")
				{
					mClassAttribute.push_back(ClassAttribute(child_item.current()));
				}
				else if (child_item->getName() == "TypeHolder")
				{
					xml::ElementEnumerator child = child_item->getElementEnumerator();
					while (child.next("TypeInfo"))
					{
						VectorPairString mParams;
						xml::ElementEnumerator item = child->getElementEnumerator();
						while (item.next())
						{
							mParams.push_back(PairString(item->getName(), item->getContent()));
						}
						mPairTypeInfo[child->findAttribute("name")] = mParams;
					}
				}
			}

			// загружаем индексный файл доксигена
			xml::Document doc_doxygen;
			const std::string filename_doxygen = "doxygen/xml/index.xml";
			if ( !doc_doxygen.open(filename_doxygen) )
			{
				std::cout << doc.getLastError() << std::endl;
				return false;
			}

			mRoot = new wrapper::Compound(doc_doxygen.getRoot(), "doxygenindex");
			return true;
		}

		void wrap()
		{
			//std::string type = wrapper::utility::getFullDefinition("EventInfo_WidgetSizeT", mRoot, "MyGUI");
			//wrapper::utility::getFullDefinition("VectorWidgetPtr", mRoot, "MyGUI");
			//return;

			// очищаем шаблоны для добавления
			for (VectorClassAttribute::iterator item=mClassAttribute.begin(); item!=mClassAttribute.end(); ++item)
			{
				(*item).initialise(mRoot, this);
			}

			for (VectorClassAttribute::iterator item=mClassAttribute.begin(); item!=mClassAttribute.end(); ++item)
			{
				(*item).wrap(mRoot);
			}
		}

	private:
		Compound * mRoot;
		VectorClassAttribute mClassAttribute;
		MapVectorPairString mPairTypeInfo;

	};

} // namespace wrapper

#endif // __WRAPPER_H__

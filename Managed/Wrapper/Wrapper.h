/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __WRAPPER_H__
#define __WRAPPER_H__

#include <MyGUI.h>

#include "Utility.h"
#include "ClassAttribute.h"

namespace wrapper
{

	class Wrapper
	{
	public:
		typedef std::vector<ClassAttribute> VectorClassAttribute;

	public:
		Wrapper() : mRoot(nullptr) { }

		bool initialise()
		{
			MyGUI::xml::Document doc;
			const std::string filename = "Data/data.xml";
			if ( !doc.open(filename) )
			{
				std::cout << doc.getLastError() << std::endl;
				return false;
			}
		
			MyGUI::xml::ElementEnumerator child_item = doc.getRoot()->getElementEnumerator();
			while (child_item.next("Item"))
			{
				mClassAttribute.push_back(ClassAttribute(child_item.current()));
			}

			// загружаем индексный файл доксигена
			MyGUI::xml::Document doc_doxygen;
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

			for (VectorClassAttribute::iterator item=mClassAttribute.begin(); item!=mClassAttribute.end(); ++item)
			{
				(*item).wrap(mRoot);
			}
		}

	private:
		Compound * mRoot;
		VectorClassAttribute mClassAttribute;

	};

} // namespace wrapper

#endif // __WRAPPER_H__

/*!
	@file
	@author		Albert Semenov
	@date		01/2009
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
		virtual ~Wrapper()
		{
			delete mRoot;
			mRoot = 0;
		}

		virtual VectorPairString getTypeInfo(const std::string& _type)
		{
			MapTypeInfo::const_iterator type = mPairTypeInfo.find( utility::trim_result(_type) );
			if (type == mPairTypeInfo.end()) return VectorPairString();
			return type->second.tags;
		}

		virtual std::string getTemplatePrefix(const std::string& _template, const std::string& _rettype, const VectorParam& _params, const std::string& _namespace)
		{
			std::string result;

			result += getCustomParamPrefix(_template, utility::trim_result(getFullDefinition(_rettype, mRoot, _namespace)), 0);

			for (size_t index=0; index<_params.size(); ++index)
			{
				result += getCustomParamPrefix(_template, utility::trim_result(getFullDefinition(_params[index].type, mRoot, _namespace)), index + 1);
			}

			return result;
		}

		bool initialise(const std::string& _filename)
		{
			xml::Document doc;
			const std::string filename = _filename;
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
						TypeInfo info;
						xml::ElementEnumerator item = child->getElementEnumerator();
						while (item.next())
						{
							if (item->getName() == "Custom")
							{
								info.custom.name = item->findAttribute("name");
								xml::ElementEnumerator infos = item->getElementEnumerator();
								while (infos.next())
								{
									if (infos->getName() == "Template")
									{
										info.custom.templates.push_back(infos->getContent());
									}
									else if (infos->getName() == "Param")
									{
										info.custom.params.push_back(utility::parseSizeT(infos->getContent()));
									}
								}
							}
							else
							{
								info.tags.push_back(PairString(item->getName(), item->getContent()));
							}
						}
						mPairTypeInfo[child->findAttribute("name")] = info;
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
			// очищаем шаблоны для добавления
			for (VectorClassAttribute::iterator item=mClassAttribute.begin(); item!=mClassAttribute.end(); ++item)
			{
				(*item).initialise(mRoot, this);
			}

			for (VectorClassAttribute::iterator item=mClassAttribute.begin(); item!=mClassAttribute.end(); ++item)
			{
				(*item).wrap();
			}
		}

		std::string getCustomParamPrefix(const std::string& _template, const std::string& _type, size_t _num)
		{
			std::string result;

			MapTypeInfo::const_iterator type = mPairTypeInfo.find( utility::trim_result(_type) );
			if (type == mPairTypeInfo.end()) return result;

			const CustomTypeInfo& info = type->second.custom;

			bool find = false;
			for (VectorString::const_iterator iter=info.templates.begin(); iter!=info.templates.end(); ++iter)
			{
				if ((*iter) == _template)
				{
					find = true;
					break;
				}
			}

			if (!find) return result;

			for (VectorSizeT::const_iterator iter=info.params.begin(); iter!=info.params.end(); ++iter)
			{
				if ((*iter) == _num)
				{
					result = utility::toString(info.name, *iter, "_");
					break;
				}
			}

			return result;
		}

	private:
		Compound * mRoot;
		VectorClassAttribute mClassAttribute;
		MapTypeInfo mPairTypeInfo;

	};

} // namespace wrapper

#endif // __WRAPPER_H__

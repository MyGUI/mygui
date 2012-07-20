/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SettingsManager2.h"

template <> tools::SettingsManager2* MyGUI::Singleton<tools::SettingsManager2>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::SettingsManager2>::mClassTypeName("SettingsManager2");

namespace tools
{

	SettingsManager2::SettingsManager2() :
		mDocument(nullptr),
		mUserDocument(nullptr)
	{
		mDocument = new pugi::xml_document();
		pugi::xml_node declaration = mDocument->append_child(pugi::node_declaration);
		declaration.append_attribute("version") = "1.0";
		declaration.append_attribute("encoding") = "utf-8";

		mDocument->append_child("Settings");

		mUserDocument = new pugi::xml_document();
		declaration = mUserDocument->append_child(pugi::node_declaration);
		declaration.append_attribute("version") = "1.0";
		declaration.append_attribute("encoding") = "utf-8";

		mUserDocument->append_child("Settings");
	}

	SettingsManager2::~SettingsManager2()
	{
		delete mDocument;
		mDocument = nullptr;

		delete mUserDocument;
		mUserDocument = nullptr;
	}

	bool SettingsManager2::loadSettingsFile(const std::string& _fileName)
	{
		pugi::xml_document doc;
		auto result = doc.load_file(_fileName.c_str());

		if (result)
		{
			if (std::string(doc.first_child().name()) == std::string(mDocument->document_element().name()))
				mergeNodes(mDocument->document_element(), doc.first_child());
		}
		else
		{
			// логировать если это ошибка формата xml
		}

		return result;
	}

	void SettingsManager2::saveSettingsFile(const std::string& _fileName)
	{
		mDocument->save_file(_fileName.c_str());
	}

	bool SettingsManager2::getExistValue(const std::string& _path)
	{
		pugi::xpath_node node = mUserDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
			return true;

		node = mDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
			return true;

		return false;
	}

	std::string SettingsManager2::getValueString(const std::string& _path)
	{
		pugi::xpath_node node = mUserDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
			return node.node().child_value();

		node = mDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
			return node.node().child_value();

		return "";
	}

	void SettingsManager2::setValueString(const std::string& _path, const std::string& _value)
	{
		pugi::xpath_node node = mUserDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
		{
			node.node().first_child().set_value(_value.c_str());
		}
		else
		{
			std::vector<std::string> names;
			std::string delims("/");
			names = MyGUI::utility::split(_path, delims);

			pugi::xml_node currentNode = mUserDocument->document_element();
			for (auto name = names.begin(); name != names.end(); name ++)
			{
				pugi::xml_node childNode = currentNode.child((*name).c_str());
				if (childNode.empty())
					childNode = currentNode.append_child((*name).c_str());

				currentNode = childNode;
			}

			pugi::xml_node textNode = currentNode.first_child();
			if (textNode.empty())
				textNode = currentNode.append_child(pugi::node_pcdata);

			textNode.set_value(_value.c_str());
		}
	}

	SettingsManager2::VectorString SettingsManager2::getValueListString(const std::string& _path)
	{
		SettingsManager2::VectorString result;

		pugi::xpath_node_set nodes = mDocument->document_element().select_nodes(_path.c_str());
		for (auto node = nodes.begin(); node != nodes.end(); node ++)
			result.push_back((*node).node().child_value());

		return result;
	}

	void SettingsManager2::mergeNodes(pugi::xml_node _nodeTarget, pugi::xml_node _nodeSource)
	{
		bool listElement = MyGUI::utility::endWith(_nodeTarget.name(), ".List");

		// затираем текст у цели потому что любое значение текста источника является конечным
		pugi::xml_node targetTextNode = _nodeTarget.first_child();
		if (!targetTextNode.empty() && targetTextNode.type() == pugi::node_pcdata)
			targetTextNode.set_value("");

		pugi::xml_node sourceTextNode = _nodeSource.first_child();
		if (!sourceTextNode.empty() && sourceTextNode.type() == pugi::node_pcdata)
		{
			pugi::xml_node targetTextNode = _nodeTarget.first_child();
			if (targetTextNode.empty())
				targetTextNode = _nodeTarget.append_child(pugi::node_pcdata);
			targetTextNode.set_value(sourceTextNode.value());
		}

		for (auto child = _nodeSource.begin(); child != _nodeSource.end(); child ++)
		{
			if ((*child).type() != pugi::node_element)
				continue;

			pugi::xml_node targetChildNode;

			if (listElement)
			{
				targetChildNode = _nodeTarget.append_child((*child).name());
			}
			else
			{
				targetChildNode = _nodeTarget.child((*child).name());
				if (targetChildNode.empty())
					targetChildNode = _nodeTarget.append_child((*child).name());
			}

			mergeAttributes(targetChildNode, (*child));
			mergeNodes(targetChildNode, (*child));
		}
	}

	void SettingsManager2::mergeAttributes(pugi::xml_node _nodeTarget, pugi::xml_node _nodeSource)
	{
		for (auto attribute = _nodeSource.attributes_begin(); attribute != _nodeSource.attributes_end(); attribute ++)
		{
			auto attributeNode = _nodeTarget.attribute((*attribute).name());
			if (attributeNode.empty())
				attributeNode = _nodeTarget.append_attribute((*attribute).name());
			attributeNode.set_value((*attribute).value());
		}
	}

	bool SettingsManager2::loadUserSettingsFile(const std::string& _fileName)
	{
		mUserSettingsFileName = _fileName;

		pugi::xml_document doc;
		auto result = doc.load_file(_fileName.c_str());

		if (result)
		{
			if (std::string(doc.first_child().name()) == std::string(mUserDocument->document_element().name()))
				mergeNodes(mUserDocument->document_element(), doc.first_child());
		}
		else
		{
			// логировать если это ошибка формата xml
		}

		return result;
	}

	void SettingsManager2::saveUserSettingsFile()
	{
		mUserDocument->save_file(mUserSettingsFileName.c_str());
	}

	void SettingsManager2::setValueListString(const std::string& _path, const VectorString& _values)
	{
	}

}

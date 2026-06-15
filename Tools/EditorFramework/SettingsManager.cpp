/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "SettingsManager.h"

namespace tools
{
	MYGUI_SINGLETON_DEFINITION(SettingsManager);

	SettingsManager::SettingsManager() :
		mSingletonHolder(this)
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

	SettingsManager::~SettingsManager()
	{
		delete mDocument;
		mDocument = nullptr;

		delete mUserDocument;
		mUserDocument = nullptr;
	}

	bool SettingsManager::loadSettingsFile(CString _fileName)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(_fileName.c_str());

		if (result)
		{
			if (std::string_view(doc.first_child().name()) == std::string_view(mDocument->document_element().name()))
				mergeNodes(mDocument->document_element(), doc.first_child());
		}
		else
		{
			// логировать если это ошибка формата xml
		}

		return result;
	}

	void SettingsManager::saveSettingsFile(CString _fileName)
	{
		mDocument->save_file(_fileName.c_str());
	}

	bool SettingsManager::getExistValue(CString _path)
	{
		pugi::xpath_node node = mUserDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
			return true;

		node = mDocument->document_element().select_single_node(_path.c_str());
		return !node.node().empty();
	}

	std::string SettingsManager::getValue(CString _path)
	{
		pugi::xpath_node node = mUserDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
			return node.node().child_value();

		node = mDocument->document_element().select_single_node(_path.c_str());
		if (!node.node().empty())
			return node.node().child_value();

		return {};
	}

	void SettingsManager::setValueImpl(std::string_view _path, CString _value)
	{
		const char* path = _path.empty() ? "" : _path.data();
		pugi::xpath_node node = mUserDocument->document_element().select_single_node(path);
		if (!node.node().empty())
		{
			node.node().text().set(_value.c_str());
		}
		else
		{
			std::vector<std::string> names;
			names = MyGUI::utility::split(_path, "/");

			pugi::xml_node currentNode = mUserDocument->document_element();
			for (const auto& name : names)
			{
				pugi::xml_node childNode = currentNode.child(name.data());
				if (childNode.empty())
					childNode = currentNode.append_child(name.data());

				currentNode = childNode;
			}

			currentNode.text().set(_value.c_str());
		}

		eventSettingsChanged(_path);
	}

	SettingsManager::VectorString SettingsManager::getValueList(std::string_view _path)
	{
		SettingsManager::VectorString result;
		std::string path{_path};
		path += "/Value";

		pugi::xpath_node_set nodes = mUserDocument->document_element().select_nodes(path.data());
		if (!nodes.empty())
		{
			for (const auto& node : nodes)
				result.emplace_back(node.node().child_value());
		}
		else
		{
			nodes = mDocument->document_element().select_nodes(path.data());
			for (const auto& node : nodes)
				result.emplace_back(node.node().child_value());
		}

		return result;
	}

	void SettingsManager::mergeNodes(pugi::xml_node _nodeTarget, pugi::xml_node _nodeSource)
	{
		bool listElement = MyGUI::utility::endWith(_nodeTarget.name(), ".List");

		// затираем текст у цели потому что любое значение текста источника является конечным
		pugi::xml_node targetTextNode = _nodeTarget.first_child();
		if (!targetTextNode.empty() && targetTextNode.type() == pugi::node_pcdata)
			targetTextNode.set_value("");

		pugi::xml_node sourceTextNode = _nodeSource.first_child();
		if (!sourceTextNode.empty() && sourceTextNode.type() == pugi::node_pcdata)
		{
			targetTextNode = _nodeTarget.first_child();
			if (targetTextNode.empty())
				targetTextNode = _nodeTarget.append_child(pugi::node_pcdata);
			targetTextNode.set_value(sourceTextNode.value());
		}

		for (auto& child : _nodeSource)
		{
			if (child.type() != pugi::node_element)
				continue;

			pugi::xml_node targetChildNode;

			if (listElement)
			{
				targetChildNode = _nodeTarget.append_child(child.name());
			}
			else
			{
				targetChildNode = _nodeTarget.child(child.name());
				if (targetChildNode.empty())
					targetChildNode = _nodeTarget.append_child(child.name());
			}

			mergeAttributes(targetChildNode, child);
			mergeNodes(targetChildNode, child);
		}
	}

	void SettingsManager::mergeAttributes(pugi::xml_node _nodeTarget, pugi::xml_node _nodeSource)
	{
		for (pugi::xml_node::attribute_iterator attribute = _nodeSource.attributes_begin();
			 attribute != _nodeSource.attributes_end();
			 attribute++)
		{
			pugi::xml_attribute attributeNode = _nodeTarget.attribute((*attribute).name());
			if (attributeNode.empty())
				attributeNode = _nodeTarget.append_attribute((*attribute).name());
			attributeNode.set_value((*attribute).value());
		}
	}

	bool SettingsManager::loadUserSettingsFile(std::string_view _fileName)
	{
		mUserSettingsFileName = _fileName;

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(mUserSettingsFileName.data());

		if (result)
		{
			if (std::string_view(doc.first_child().name()) ==
				std::string_view(mUserDocument->document_element().name()))
				mergeNodes(mUserDocument->document_element(), doc.first_child());
		}
		else
		{
			// логировать если это ошибка формата xml
		}

		return result;
	}

	void SettingsManager::saveUserSettingsFile()
	{
		if (!mUserSettingsFileName.empty())
			mUserDocument->save_file(mUserSettingsFileName.data());
	}

	void SettingsManager::setValueListImpl(std::string_view _path, const VectorString& _values)
	{
		if (!MyGUI::utility::endWith(_path, ".List"))
			return;

		pugi::xml_node targetNode;

		pugi::xpath_node node = mUserDocument->document_element().select_single_node(_path.data());
		if (!node.node().empty())
		{
			targetNode = node.node();
			while (!targetNode.first_child().empty())
				targetNode.remove_child(targetNode.first_child());
		}
		else
		{
			std::vector<std::string> names;
			names = MyGUI::utility::split(_path, "/");

			pugi::xml_node currentNode = mUserDocument->document_element();
			for (const auto& name : names)
			{
				pugi::xml_node childNode = currentNode.child(name.data());
				if (childNode.empty())
					childNode = currentNode.append_child(name.data());

				currentNode = childNode;
			}

			targetNode = currentNode;
		}

		for (const auto& _value : _values)
			targetNode.append_child("Value").text().set(_value.data());

		eventSettingsChanged(_path);
	}

	pugi::xpath_node_set SettingsManager::getValueNodeList(std::string_view _path)
	{
		std::string path{_path};
		path += "/Value";

		pugi::xpath_node_set nodes = mUserDocument->document_element().select_nodes(path.data());
		if (!nodes.empty())
			return nodes;

		nodes = mDocument->document_element().select_nodes(path.data());
		return nodes;
	}

}

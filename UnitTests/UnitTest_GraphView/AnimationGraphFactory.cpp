#include "AnimationGraphFactory.h"

namespace animation
{

	AnimationGraphFactory::AnimationGraphFactory()
	{
	}

	AnimationGraph* AnimationGraphFactory::createGraph(const std::string& _filename)
	{
		AnimationGraph* result = 0;

		MyGUI::xml::Document doc;
		if (doc.open(_filename))
		{
			MyGUI::xml::Element* root = doc.getRoot();
			if (root)
			{
				result = new AnimationGraph(root->findAttribute("name"));

				MyGUI::xml::ElementEnumerator data = root->getElementEnumerator();
				while (data.next())
				{
					if (data->getName() == "Node")
					{
						IAnimationNode* node = mNodeFactory.createNode(data->findAttribute("type"), data->findAttribute("name"), result);
						result->addNode(node);

						MyGUI::xml::ElementEnumerator prop = data->getElementEnumerator();
						while (prop.next("Property"))
						{
							std::string key = prop->findAttribute("key");
							std::string value = prop->findAttribute("value");

							node->setProperty(key, value);
						}
					}
					else if (data->getName() == "Connection")
					{
						IAnimationNode* from_node = result->getNodeByName(data->findAttribute("from"));
						IAnimationNode* to_node = result->getNodeByName(data->findAttribute("to"));

						MyGUI::xml::ElementEnumerator point = data->getElementEnumerator();
						while (point.next("Point"))
						{
							from_node->addConnection(
								point->findAttribute("from"),
								to_node,
								point->findAttribute("to"));
						}
					}
				}
			}
		}

		return result;
	}

} // namespace animation

/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __GRAPH_NODE_SIMPLE_H__
#define __GRAPH_NODE_SIMPLE_H__

#include <MyGUI.h>
#include "BaseGraphNode.h"
#include "GraphConnectionSimple.h"

namespace demo
{

	class GraphNodeSimple : public wraps::BaseGraphNode
	{
	public:
		GraphNodeSimple(const std::string& _name) :
			BaseGraphNode("NodeSimple.layout"),
			mName(_name),
			mConnection1(nullptr),
			mConnection2(nullptr)
		{
		}

	private:
		virtual void initialise()
		{
			mMainWidget->setCaption(mName);
			assignBase(mConnection1, "Connection1");
			assignBase(mConnection2, "Connection2");
		}

		virtual void shutdown()
		{
		}

	private:
		std::string mName;
		GraphConnectionSimple * mConnection1;
		GraphConnectionSimple * mConnection2;

	};

} // namespace demo

#endif // __GRAPH_NODE_SIMPLE_H__

/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseDemoManager.h"
#include "GraphView.h"
#include "AnimationGraph.h"
#include "AnimationGraphFactory.h"
#include "AnimationNodeFactory.h"
#include "GraphNodeFactory.h"
#include "OpenSaveFileDialog.h"
#include "ContextMenu.h"

namespace demo
{

	class DemoKeeper :
		public base::BaseDemoManager
	{
	public:
		DemoKeeper();

	private:
		virtual void createScene();
		virtual void destroyScene();
		virtual void setupResources();
		void notifyFrameStarted(float _time);

		void createGrapView();
		void notifyNodeClosed(wraps::BaseGraphView* _sender, wraps::BaseGraphNode* _node);
		void notifyConnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to);
		void notifyDisconnectPoint(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to);
		void notifyInvalidateNode(BaseAnimationNode* _sender);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMenuCtrlAccept(wraps::ContextMenu* _sender, const std::string& _id);

		void SaveGraph();
		void LoadGraph();
		void ClearGraph();

		void notifyEndDialog(tools::Dialog* _dialog, bool _result);
		void saveToFile(const std::string& _filename);
		void loadFromFile(const std::string& _filename);

		BaseAnimationNode* createNode(const std::string& _type, const std::string& _name);
		BaseAnimationNode* getNodeByName(const std::string& _name);

		void connectPoints(BaseAnimationNode* _node_from, BaseAnimationNode* _node_to, const std::string& _name_from, const std::string& _name_to);
		void disconnectPoints(BaseAnimationNode* _node_from, BaseAnimationNode* _node_to, const std::string& _name_from, const std::string& _name_to);

	private:
		GraphView* mGraphView;

		animation::AnimationGraphFactory mGraphFactory;
		animation::AnimationNodeFactory mNodeFactory;
		animation::AnimationGraph* mGraph;

		GraphNodeFactory mGraphNodeFactory;

		tools::OpenSaveFileDialog* mFileDialog;
		bool mFileDialogSave;
		wraps::ContextMenu* mContextMenu;

		typedef std::vector<BaseAnimationNode*> VectorBaseAnimationNode;
		VectorBaseAnimationNode mNodes;

		MyGUI::IntPoint mClickPosition;
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__

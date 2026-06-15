/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#ifndef DEMO_KEEPER_H_
#define DEMO_KEEPER_H_

#include "Base/BaseDemoManager.h"
#include "GraphView.h"
#include "AnimationGraph.h"
#include "AnimationNodeFactory.h"
#include "GraphNodeFactory.h"
#include "OpenSaveFileDialog.h"
#include "ContextMenu.h"

namespace demo
{

	class DemoKeeper : public base::BaseDemoManager
	{
	private:
		void createScene() override;
		void destroyScene() override;
		void setupResources() override;
		void notifyFrameStarted(float _time);

		void createGrapView();
		void notifyNodeClosed(wraps::BaseGraphView* _sender, wraps::BaseGraphNode* _node);
		void notifyConnectPoint(
			wraps::BaseGraphView* _sender,
			wraps::BaseGraphConnection* _from,
			wraps::BaseGraphConnection* _to);
		void notifyDisconnectPoint(
			wraps::BaseGraphView* _sender,
			wraps::BaseGraphConnection* _from,
			wraps::BaseGraphConnection* _to);
		void notifyInvalidateNode(BaseAnimationNode* _sender);
		void notifyMouseButtonReleased(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMenuCtrlAccept(wraps::ContextMenu* _sender, std::string_view _id);

		void SaveGraph();
		void LoadGraph();
		void ClearGraph();

		void notifyEndDialog(tools::Dialog* _dialog, bool _result);
		void saveToFile(const std::string& _filename);
		void loadFromFile(const std::string& _filename);

		BaseAnimationNode* createNode(std::string_view _type, std::string_view _name);
		BaseAnimationNode* getNodeByName(std::string_view _name);

		void connectPoints(
			BaseAnimationNode* _node_from,
			BaseAnimationNode* _node_to,
			std::string_view _name_from,
			std::string_view _name_to);
		void disconnectPoints(
			BaseAnimationNode* _node_from,
			BaseAnimationNode* _node_to,
			std::string_view _name_from,
			std::string_view _name_to);

	private:
		GraphView* mGraphView{nullptr};

		animation::AnimationNodeFactory mNodeFactory;
		animation::AnimationGraph* mGraph{nullptr};

		GraphNodeFactory mGraphNodeFactory;

		tools::OpenSaveFileDialog* mFileDialog{nullptr};
		bool mFileDialogSave{false};
		wraps::ContextMenu* mContextMenu{nullptr};

		using VectorBaseAnimationNode = std::vector<BaseAnimationNode*>;
		VectorBaseAnimationNode mNodes;

		MyGUI::IntPoint mClickPosition;
	};

} // namespace demo

#endif // DEMO_KEEPER_H_

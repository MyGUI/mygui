/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __BASE_GRAPH_VIEW_H__
#define __BASE_GRAPH_VIEW_H__

#include <MyGUI.h>
#include "MyGUI_Canvas.h"
#include "BaseGraphNode.h"
#include "ConnectionInfo.h"

namespace wraps
{

	class BaseGraphView :
		public BaseLayout,
		public IGraphController
	{
	public:
		typedef std::vector<BaseGraphNode*> VectorGraphNode;
		typedef MyGUI::Enumerator<VectorGraphNode> EnumeratorNode;

		BaseGraphView(const std::string& _layout, MyGUI::Widget* _parent) :
			BaseLayout(_layout, _parent),
			mCanvas(nullptr),
			mIsDrug(false),
			mConnectionStart(nullptr),
			mInvalidate(false),
			mCurrentIndexConnection(0)
		{
			MyGUI::Gui::getInstance().eventFrameStart += MyGUI::newDelegate(this, &BaseGraphView::notifyFrameStart);
		}

		virtual ~BaseGraphView()
		{
			MyGUI::Gui::getInstance().eventFrameStart -= MyGUI::newDelegate(this, &BaseGraphView::notifyFrameStart);
		}

		void notifyFrameStart(float _time)
		{
			if (mInvalidate)
			{
				mInvalidate = false;
				updateCanvasImpl();
			}
		}

		void addItem(BaseGraphNode* _node)
		{
			mNodes.push_back(_node);
			_node->_initialise(mCanvas, this);

			changePosition(_node);
		}

		void removeItem(BaseGraphNode* _node)
		{
			VectorGraphNode::iterator item = std::find(mNodes.begin(), mNodes.end(), _node);
			MYGUI_ASSERT(item != mNodes.end(), "Item not found");

			removeAllConnections(_node);
			_node->_shutdown();

			mNodes.erase(item);

			changePosition(nullptr);
		}

		void removeAllItems()
		{
			for (VectorGraphNode::iterator item = mNodes.begin(); item != mNodes.end(); ++item)
			{
				removeAllConnections((*item));
				(*item)->_shutdown();
				(*item) = nullptr;
			}
			mNodes.clear();

			changePosition(nullptr);
		}

		EnumeratorNode getNodeEnumerator() const
		{
			return EnumeratorNode(mNodes);
		}

		bool isConnecting(BaseGraphConnection* _from, BaseGraphConnection* _to) const
		{
			EnumeratorConnection conn = _from->getConnectionEnumerator();
			while (conn.next())
			{
				if (conn.current() == _to)
				{
					return true;
				}
			}
			return false;
		}

		MyGUI::Widget* getClient() const
		{
			return mCanvas;
		}

	/*event:*/
		/** Request : Connection point.\n
			signature : void method(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to, bool& _result)
			@param _sender
			@param _from
			@param _to
			@param _result
		*/
		MyGUI::delegates::CDelegate4<BaseGraphView*, BaseGraphConnection*, BaseGraphConnection*, bool&> requestConnectPoint;

		/** Request : Disconnection point.\n
			signature : void method(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to, bool& _result)
			@param _sender
			@param _from
			@param _to
			@param _result
		*/
		MyGUI::delegates::CDelegate4<BaseGraphView*, BaseGraphConnection*, BaseGraphConnection*, bool&> requestDisconnectPoint;

		/** Event : Connection point.\n
			signature : void method(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to)
			@param _sender
			@param _from
			@param _to
		*/
		MyGUI::delegates::CDelegate3<BaseGraphView*, BaseGraphConnection*, BaseGraphConnection*> eventConnectPoint;

		/** Event : Disconnection point.\n
			signature : void method(wraps::BaseGraphView* _sender, wraps::BaseGraphConnection* _from, wraps::BaseGraphConnection* _to)
			@param _sender
			@param _from
			@param _to
		*/
		MyGUI::delegates::CDelegate3<BaseGraphView*, BaseGraphConnection*, BaseGraphConnection*> eventDisconnectPoint;

		/** Event : Change size.\n
			signature : void method(wraps::BaseGraphView* _sender, MyGUI::IntSize _size)
			@param _sender
			@param _size
		*/
		MyGUI::delegates::CDelegate2<BaseGraphView*, MyGUI::IntSize> eventChangeSize;

		/** Event : Node closed.\n
			signature : void method(wraps::BaseGraphView* _sender, wraps::BaseGraphNode* _node)
			@param _sender
			@param _id
		*/
		MyGUI::delegates::CDelegate2<BaseGraphView*, BaseGraphNode*> eventNodeClosed;

	protected:
		void setCanvasWidget(const std::string& _widgetName)
		{
			assignWidget(mCanvas, _widgetName);

			updateCanvas();
		}

	private:
		void removeAllConnections(BaseGraphNode* _node)
		{
			EnumeratorConnection node_conn = _node->getConnectionEnumerator();
			while (node_conn.next())
			{
				// удаляем прямые соединения
				while (node_conn.current()->isAnyConnection())
				{
					EnumeratorConnection conn = node_conn.current()->getConnectionEnumerator();
					while (conn.next())
					{
						eventDisconnectPoint(this, node_conn.current(), conn.current());
						node_conn.current()->removeConnectionPoint(conn.current());
						break;
					}
				}

				// удаляем обратные соединения
				while (node_conn.current()->isAnyReverseConnection())
				{
					EnumeratorConnection conn = node_conn.current()->getReverseConnectionEnumerator();
					while (conn.next())
					{
						eventDisconnectPoint(this, conn.current(), node_conn.current());
						conn.current()->removeConnectionPoint(node_conn.current());
						break;
					}
				}
			}
		}

		virtual void close(BaseGraphNode* _node)
		{
			eventNodeClosed(this, _node);
		}

		virtual void startDrag(BaseGraphConnection* _connection)
		{
			if ( ! mIsDrug )
			{
				bool result = false;
				requestConnectPoint(this, _connection, nullptr, result);

				// тащим новый конект
				if (result)
				{
					mIsDrug = true;

					const MyGUI::IntCoord& coord = _connection->getAbsoluteCoord();

					mDrugLine.colour.set(1, 1, 1, 1);
					mDrugLine.start_offset.clear();
					mDrugLine.end_offset.clear();
					mDrugLine.point_start.set(
						coord.left + (coord.width / 2) - mCanvas->getAbsoluteLeft(),
						coord.top + (coord.height / 2) - mCanvas->getAbsoluteTop()
					);
					mDrugLine.point_end = mDrugLine.point_start;

					mConnectionStart = _connection;

					updateCanvas();
				}
				// разрываем существующий
				else
				{
					BaseGraphConnection* drag_node = nullptr;
					bool disconect = false;
					// прямое сочленение
					if (_connection->isAnyConnection())
					{
						EnumeratorConnection conn = _connection->getConnectionEnumerator();
						while (conn.next())
						{
							result = false;
							requestDisconnectPoint(this, _connection, conn.current(), result);
							if (result)
							{
								drag_node = _connection;
								eventDisconnectPoint(this, _connection, conn.current());
								_connection->removeConnectionPoint(conn.current());
								disconect = true;
							}
							break;
						}
					}
					else
					{
						// обратное сочленение
						EnumeratorConnection conn = _connection->getReverseConnectionEnumerator();
						while (conn.next())
						{
							result = false;
							requestDisconnectPoint(this, conn.current(), _connection, result);
							if (result)
							{
								drag_node = conn.current();
								eventDisconnectPoint(this, conn.current(), _connection);
								conn.current()->removeConnectionPoint(_connection);
								disconect = true;
							}
							break;
						}
					}

					// тащим разорваную связь
					if (disconect)
					{
						mIsDrug = true;

						const MyGUI::IntCoord& coord = drag_node->getAbsoluteCoord();

						mDrugLine.colour.set(1, 1, 1, 1);
						mDrugLine.start_offset.clear();
						mDrugLine.end_offset.clear();
						mDrugLine.point_start.set(
							coord.left + (coord.width / 2) - mCanvas->getAbsoluteLeft(),
							coord.top + (coord.height / 2) - mCanvas->getAbsoluteTop()
						);
						mDrugLine.point_end = mDrugLine.point_start;

						mConnectionStart = drag_node;

						updateCanvas();

						updateDrag(nullptr);
					}

				}
			}
		}

		virtual void stopDrag(BaseGraphConnection* _connection)
		{
			if (mIsDrug)
			{
				// нод откуда тянется не всегда может быть сендером
				_connection = mConnectionStart;

				connectPoint(_connection);

				mIsDrug = false;
				mConnectionStart = nullptr;

				updateCanvas();
			}
		}

		virtual void updateDrag(BaseGraphConnection* _connection)
		{
			if (mIsDrug)
			{
				// нод откуда тянется не всегда может быть сендером
				_connection = mConnectionStart;

				const MyGUI::IntPoint& mouse = MyGUI::InputManager::getInstance().getMousePosition();
				//const MyGUI::IntCoord& coord = _node->getAbsoluteCoord();
				mDrugLine.point_end.set(mouse.left - mCanvas->getAbsoluteLeft(), mouse.top - mCanvas->getAbsoluteTop());

				// устанавлваем длинну загиба от дистанции
				double distance = ((mDrugLine.point_end.left - mDrugLine.point_start.left) * (mDrugLine.point_end.left - mDrugLine.point_start.left)) +
					((mDrugLine.point_end.top - mDrugLine.point_start.top) * (mDrugLine.point_end.top - mDrugLine.point_start.top));
				distance = std::sqrt(distance);

				mDrugLine.start_offset = _connection->getOffset();

				const int offset = 30;
				distance *= 0.5;
				if (distance < 1) distance = 1;
				else if (distance > offset) distance = offset;
				if (mDrugLine.start_offset.height != 0)
				{
					if (mDrugLine.start_offset.height < 0) mDrugLine.start_offset.height = -(int)distance;
					else  mDrugLine.start_offset.height = (int)distance;
				}
				if (mDrugLine.start_offset.width != 0)
				{
					if (mDrugLine.start_offset.width < 0) mDrugLine.start_offset.width = -(int)distance;
					else  mDrugLine.start_offset.width = (int)distance;
				}

				// пикаем виджет под нами
				MyGUI::Widget* widget = MyGUI::LayerManager::getInstance().getWidgetFromPoint(mouse.left, mouse.top);
				if (widget != nullptr)
				{
					BaseGraphConnection** connection = widget->getUserData<BaseGraphConnection*>(false);
					if (connection != nullptr)
					{
						bool accept = requestConnectToPoint(_connection, *connection);
						if (accept)
							mDrugLine.colour = MyGUI::Colour::Green;
						else
							mDrugLine.colour = MyGUI::Colour::Red;
					}
					else
					{
						mDrugLine.colour = MyGUI::Colour::White;
					}
				}
				else
				{
					mDrugLine.colour = MyGUI::Colour::White;
				}

				updateCanvas();
			}
		}

		virtual void changePosition(BaseGraphNode* _node)
		{
			eventChangeSize(this, getViewSize());

			updateCanvas();
		}

		void updateCanvas()
		{
			mInvalidate = true;
		}

		void updateCanvasImpl()
		{
			clearCanvas();

			// проходим по всем нодам и перерисовываем связи
			for (size_t index = 0; index < mNodes.size(); ++index)
			{
				EnumeratorConnection node_point = mNodes[index]->getConnectionEnumerator();
				while (node_point.next())
				{
					const MyGUI::IntCoord& coord_from = node_point->getAbsoluteCoord();
					EnumeratorConnection connect_point = node_point->getConnectionEnumerator();
					while (connect_point.next())
					{
						const MyGUI::IntCoord& coord_to = connect_point->getAbsoluteCoord();

						ConnectionInfo info(
							coord_from.point() - mCanvas->getAbsolutePosition() + MyGUI::IntPoint(coord_from.width / 2, coord_from.height / 2),
							coord_to.point() - mCanvas->getAbsolutePosition() + MyGUI::IntPoint(coord_to.width / 2, coord_to.height / 2),
							MyGUI::Colour::White,
							node_point->getOffset(),
							connect_point->getOffset());

						drawCurve(info);
					}
				}
			}

			// ниточка для драга
			if (mIsDrug)
				drawCurve(mDrugLine);
		}

		void connectPoint(BaseGraphConnection* _connection)
		{
			const MyGUI::IntPoint& mouse = MyGUI::InputManager::getInstance().getMousePosition();

			// пикаем виджет под нами
			MyGUI::Widget* widget = MyGUI::LayerManager::getInstance().getWidgetFromPoint(mouse.left, mouse.top);
			if (widget != nullptr)
			{
				BaseGraphConnection** connection = widget->getUserData<BaseGraphConnection*>(false);
				if (connection != nullptr)
				{
					bool accept = requestConnectToPoint(_connection, *connection);
					if (accept)
					{
						eventConnectToPoint(_connection, *connection);
					}
				}
			}
		}

		bool requestConnectToPoint(BaseGraphConnection* _from, BaseGraphConnection* _to)
		{
			bool result = false;
			requestConnectPoint(this, _from, _to, result);

			return result;
		}

		void eventConnectToPoint(BaseGraphConnection* _from, BaseGraphConnection* _to)
		{
			_from->addConnectionPoint(_to);
			eventConnectPoint(this, _from, _to);
		}

		void clearCanvas()
		{
			for (MyGUI::VectorWidgetPtr::iterator item = mConnections.begin(); item != mConnections.end(); ++item)
				(*item)->setVisible(false);
			mCurrentIndexConnection = 0;
		}

		MyGUI::Widget* getNextWidget()
		{
			MyGUI::Widget* result = nullptr;

			if (mCurrentIndexConnection < mConnections.size())
			{
				result = mConnections[mCurrentIndexConnection];
			}
			else
			{
				result = mCanvas->createWidget<MyGUI::Widget>("PolygonalSkin", mCanvas->getCoord(), MyGUI::Align::Default);
				result->setNeedMouseFocus(false);
				mConnections.push_back(result);
			}

			mCurrentIndexConnection++;

			result->setVisible(true);
			return result;
		}

		void drawSpline(const ConnectionInfo& _info, int _offset, const MyGUI::Colour& _colour)
		{
			MyGUI::Widget* widget = getNextWidget();
			widget->setColour(_colour);

			MyGUI::ISubWidget* main = widget->getSubWidgetMain();
			MyGUI::PolygonalSkin* polygonalSkin = main->castType<MyGUI::PolygonalSkin>();
			polygonalSkin->setWidth(4.0f);

			const size_t PointsNumber = 16;
			std::vector<MyGUI::FloatPoint> basePoints;
			basePoints.push_back(
				MyGUI::FloatPoint((float)_info.point_start.left, (float)_info.point_start.top + _offset));
			basePoints.push_back(
				MyGUI::FloatPoint((float)_info.point_start.left + _info.start_offset.width, (float)_info.point_start.top + _info.start_offset.height + _offset));
			basePoints.push_back(
				MyGUI::FloatPoint((float)_info.point_end.left + _info.end_offset.width, (float)_info.point_end.top + _info.end_offset.height + _offset));
			basePoints.push_back(
				MyGUI::FloatPoint((float)_info.point_end.left, (float)_info.point_end.top + _offset));
			std::vector<MyGUI::FloatPoint> splinePoints;
			splinePoints.reserve(PointsNumber);
			for (size_t i = 0; i < PointsNumber; ++i)
			{
				float t = float(i) / (PointsNumber - 1);
				float left = basePoints[0].left * pow(1 - t, 3) + 3 * basePoints[1].left * pow(1 - t, 2) * t + 3 * basePoints[2].left * (1 - t) * t * t + t * t * t * basePoints[3].left;
				float top = basePoints[0].top * pow(1 - t, 3) + 3 * basePoints[1].top * pow(1 - t, 2) * t + 3 * basePoints[2].top * (1 - t) * t * t + t * t * t * basePoints[3].top;
				splinePoints.push_back(MyGUI::FloatPoint(left, top));
			}
			polygonalSkin->setPoints(splinePoints);
		}

		void drawCurve(const ConnectionInfo& _info)
		{
			drawSpline(_info, 3, MyGUI::Colour(0.3, 0.3, 0.3, 0.8));
			drawSpline(_info, 0, _info.colour);
		}

		MyGUI::IntSize getViewSize()
		{
			MyGUI::IntSize result;
			for (size_t index = 0; index < mNodes.size(); ++index)
			{
				const MyGUI::IntCoord& coord = mNodes[index]->getCoord();
				if (coord.right() > result.width) result.width = coord.right();
				if (coord.bottom() > result.height) result.height = coord.bottom();
			}

			// для соединений справа
			result.width += 10;
			result.height += 10;

			return result;
		}

	private:
		VectorGraphNode mNodes;

		MyGUI::Widget* mCanvas;
		bool mIsDrug;
		ConnectionInfo mDrugLine;
		BaseGraphConnection* mConnectionStart;
		bool mInvalidate;
		MyGUI::VectorWidgetPtr mConnections;
		size_t mCurrentIndexConnection;
	};

} // namespace wraps

#endif // __BASE_GRAPH_VIEW_H__

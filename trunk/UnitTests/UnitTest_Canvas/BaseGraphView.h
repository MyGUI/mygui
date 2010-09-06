/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __BASE_GRAPH_VIEW_H__
#define __BASE_GRAPH_VIEW_H__

#include <MyGUI.h>
#include "MyGUI_Canvas.h"
#include "BaseGraphNode.h"

namespace wraps
{
	struct ConnectionInfo
	{
		ConnectionInfo() { }
		ConnectionInfo(const MyGUI::IntPoint& _point_start, const MyGUI::IntPoint& _point_end, const MyGUI::Colour& _colour, const MyGUI::IntSize& _start_offset, const MyGUI::IntSize& _end_offset) :
			point_start(_point_start), point_end(_point_end), colour(_colour), start_offset(_start_offset), end_offset(_end_offset) { }

		MyGUI::Colour colour;
		MyGUI::IntPoint point_start;
		MyGUI::IntPoint point_end;
		MyGUI::IntSize start_offset;
		MyGUI::IntSize end_offset;
	};

	class BaseGraphView : public BaseLayout, public IGraphController
	{
	public:
		typedef std::vector<BaseGraphNode*> VectorGraphNode;

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

	public:
		BaseGraphView(const std::string& _layout, MyGUI::WidgetPtr _parent) :
			BaseLayout(_layout, _parent),
			mIsDrug(false),
			mConnectionStart(nullptr)
		{
			mCanvas = mMainWidget->castType<MyGUI::Canvas>();
			mCanvas->requestUpdateCanvas = MyGUI::newDelegate( this, &BaseGraphView::requestUpdateCanvas );
			mCanvas->updateTexture();
		}

		void addItem(BaseGraphNode* _node)
		{
			mNodes.push_back(_node);
			_node->_initialise(mMainWidget, this);

			changePosition(_node);
		}

		void removeItem(BaseGraphNode* _node)
		{
			VectorGraphNode::iterator item = std::find(mNodes.begin(), mNodes.end(), _node);
			MYGUI_ASSERT(item != mNodes.end(), "Item not found");
			(*item)->_shutdown();
			mNodes.erase(item);

			changePosition(_node);
		}

		bool isConnecting(BaseGraphConnection* _from, BaseGraphConnection* _to)
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

	private:
		virtual void eraseView()
		{
			mCanvas->updateTexture();
		}

		virtual void startDrag(BaseGraphConnection* _node)
		{
			if ( ! mIsDrug )
			{
				bool result = false;
				requestConnectPoint(this, _node, nullptr, result);

				// тащим новый конект
				if (result)
				{
					mIsDrug = true;

					const MyGUI::IntCoord& coord = _node->getAbsoluteCoord();

					mDrugLine.colour.set(1, 1, 1, 1);
					mDrugLine.start_offset.clear();
					mDrugLine.end_offset.clear();
					mDrugLine.point_start.set(
						coord.left + (coord.width / 2) - mMainWidget->getAbsoluteLeft(),
						coord.top + (coord.height / 2) - mMainWidget->getAbsoluteTop()
					);
					mDrugLine.point_end = mDrugLine.point_start;

					mConnectionStart = _node;
					mCanvas->updateTexture();
				}
				// разрываем существующий
				else
				{
					BaseGraphConnection* drag_node = nullptr;
					bool disconect = false;
					// прямое сочленение
					if (_node->getConnectionCount())
					{
						EnumeratorConnection conn = _node->getConnectionEnumerator();
						while (conn.next())
						{
							result = false;
							requestDisconnectPoint(this, _node, conn.current(), result);
							if (result)
							{
								drag_node = _node;
								eventDisconnectPoint(this, _node, conn.current());
								_node->removeConnectionPoint(conn.current());
								disconect = true;
							}
							break;
						}
					}
					else
					{
						// обратное сочленение
						EnumeratorConnection conn = _node->getReverseConnectionEnumerator();
						while (conn.next())
						{
							result = false;
							requestDisconnectPoint(this, conn.current(), _node, result);
							if (result)
							{
								drag_node = conn.current();
								eventDisconnectPoint(this, conn.current(), _node);
								conn.current()->removeConnectionPoint(_node);
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
							coord.left + (coord.width / 2) - mMainWidget->getAbsoluteLeft(),
							coord.top + (coord.height / 2) - mMainWidget->getAbsoluteTop()
						);
						mDrugLine.point_end = mDrugLine.point_start;

						mConnectionStart = drag_node;
						mCanvas->updateTexture();
					}

				}
			}
		}

		virtual void stopDrag(BaseGraphConnection* _node)
		{
			if (mIsDrug)
			{
				// нод откуда тянется не всегда может быть сендером
				_node = mConnectionStart;

				connectPoint(_node);

				mIsDrug = false;
				mConnectionStart = nullptr;

				mCanvas->updateTexture();
			}
		}

		virtual void updateDrag(BaseGraphConnection* _node)
		{
			if (mIsDrug)
			{
				// нод откуда тянется не всегда может быть сендером
				_node = mConnectionStart;

				const MyGUI::IntPoint& mouse = MyGUI::InputManager::getInstance().getMousePosition();
				//const MyGUI::IntCoord& coord = _node->getAbsoluteCoord();
				mDrugLine.point_end.set(mouse.left - mMainWidget->getAbsoluteLeft(), mouse.top - mMainWidget->getAbsoluteTop());

				// устанавлваем длинну загиба от дистанции
				double distance = ((mDrugLine.point_end.left - mDrugLine.point_start.left) * (mDrugLine.point_end.left - mDrugLine.point_start.left)) +
					((mDrugLine.point_end.top - mDrugLine.point_start.top) * (mDrugLine.point_end.top - mDrugLine.point_start.top));
				distance = std::sqrt(distance);

				mDrugLine.start_offset = _node->getOffset();

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
				MyGUI::WidgetPtr widget = MyGUI::LayerManager::getInstance().getWidgetFromPoint(mouse.left, mouse.top);
				if (widget)
				{
					BaseGraphConnection** connection = widget->getUserData<BaseGraphConnection*>(false);
					if (connection != nullptr)
					{
						bool accept = requestConnectToPoint(_node, *connection);
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

				mCanvas->updateTexture();
			}
		}

		virtual void changePosition(BaseGraphNode* _node)
		{
			eventChangeSize(this, getViewSize());
		}

		void requestUpdateCanvas(MyGUI::Canvas* _canvas, MyGUI::Canvas::Event _event)
		{
			if ( ! _event.textureChanged && ! _event.requested ) return;

			clearCanvas(_canvas);

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
							coord_from.point() - mMainWidget->getAbsolutePosition() + MyGUI::IntPoint(coord_from.width / 2, coord_from.height / 2),
							coord_to.point() - mMainWidget->getAbsolutePosition() + MyGUI::IntPoint(coord_to.width / 2, coord_to.height / 2),
							MyGUI::Colour::White,
							node_point->getOffset(),
							connect_point->getOffset());

						drawCurve(_canvas, info);
					}
				}
			}

			// ниточка для драга
			if (mIsDrug)
				drawCurve(_canvas, mDrugLine);
		}

		void connectPoint(BaseGraphConnection* _node)
		{
			const MyGUI::IntPoint& mouse = MyGUI::InputManager::getInstance().getMousePosition();

			// пикаем виджет под нами
			MyGUI::WidgetPtr widget = MyGUI::LayerManager::getInstance().getWidgetFromPoint(mouse.left, mouse.top);
			BaseGraphConnection** connection = widget->getUserData<BaseGraphConnection*>(false);
			if (connection != nullptr)
			{
				bool accept = requestConnectToPoint(_node, *connection);
				if (accept)
				{
					eventConnectToPoint(_node, *connection);
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

		void clearCanvas(MyGUI::Canvas* _canvas)
		{
			size_t i = 0;
			while (i < _canvas->getChildCount())
			{
				if (_canvas->getChildAt(i)->isType<MyGUI::StaticImage>())
				{
					MyGUI::WidgetManager::getInstance().destroyWidget(_canvas->getChildAt(i));
				}
				else
				{
					++i;
				}
			}
		}

		void drawSpline(MyGUI::Canvas* _canvas, const ConnectionInfo& _info, int _offset, const MyGUI::Colour& _colour)
		{
			MyGUI::Widget* widget = _canvas->createWidget<MyGUI::StaticImage>("PolygonalSkin", _canvas->getCoord(), MyGUI::Align::Default);

			MyGUI::ISubWidget* main = widget->getSubWidgetMain();
			MyGUI::PolygonalSkin* polygonalSkin = main->castType<MyGUI::PolygonalSkin>();
			polygonalSkin->setWidth(4.0f);
			widget->setColour(_colour);

			const int PointsNumber = 16;
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

		void drawCurve(MyGUI::Canvas* _canvas, const ConnectionInfo& _info)
		{
			drawSpline(_canvas, _info, 3, MyGUI::Colour(0.3, 0.3, 0.3, 0.8));
			drawSpline(_canvas, _info, 0, _info.colour);
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
		MyGUI::CanvasPtr mCanvas;
		VectorGraphNode mNodes;
		bool mIsDrug;
		ConnectionInfo mDrugLine;
		BaseGraphConnection* mConnectionStart;
	};

} // namespace wraps

#endif // __BASE_GRAPH_VIEW_H__

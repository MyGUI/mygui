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
#include "ConnectionInfo.h"

#include "agg_scanline_p.h"
#include "agg_renderer_scanline.h"
#include "agg_pixfmt_rgba.h"

#include "agg_scanline_u.h"
#include "agg_rasterizer_scanline_aa.h"
#include "agg_pixfmt_rgb.h"
#include "agg_path_storage.h"
#include "agg_curves.h"
#include "agg_conv_stroke.h"

namespace wraps
{

	class BaseGraphView : public BaseLayout, public IGraphController
	{
	public:
		BaseGraphView(const std::string& _layout, MyGUI::WidgetPtr _parent) :
	  		BaseLayout(_layout, _parent),
			mIsDrug(false),
			mConnectionStart(nullptr),
			mCanvas(nullptr)
		{
			if (mMainWidget->isType<MyGUI::Canvas>())
			{
				wrapCanvas(mMainWidget->castType<MyGUI::Canvas>());
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
			removeAllConnections(_node);

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
		void wrapCanvas(MyGUI::CanvasPtr _canvas)
		{
			mCanvas = _canvas;
			mCanvas->requestUpdateCanvas = MyGUI::newDelegate( this, &BaseGraphView::requestUpdateCanvas );
			mCanvas->createTexture( MyGUI::Canvas::TRM_PT_VIEW_REQUESTED);
			mCanvas->updateTexture();
		}

	private:
		void removeAllConnections(BaseGraphNode* _node)
		{
			EnumeratorConnection node_conn = _node->getConnectionEnumerator();
			while (node_conn.next())
			{
				// удаляем прямые соединения
				while (node_conn.current()->getConnectionCount())
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
				while (node_conn.current()->getReverseConnectionCount())
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
					mCanvas->updateTexture();
				}
				// разрываем существующий
				else
				{
					BaseGraphConnection* drag_node = nullptr;
					bool disconect = false;
					// прямое сочленение
					if (_connection->getConnectionCount())
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
						mCanvas->updateTexture();

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

				mCanvas->updateTexture();
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
					if (mDrugLine.start_offset.height < 0) mDrugLine.start_offset.height = distance * -1;
					else  mDrugLine.start_offset.height = distance;
				}
				if (mDrugLine.start_offset.width != 0)
				{
					if (mDrugLine.start_offset.width < 0) mDrugLine.start_offset.width = distance * -1;
					else  mDrugLine.start_offset.width = distance;
				}

				// пикаем виджет под нами
				MyGUI::WidgetPtr widget = MyGUI::LayerManager::getInstance().getWidgetFromPoint(mouse.left, mouse.top);
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

				mCanvas->updateTexture();
			}
		}

		virtual void changePosition(BaseGraphNode* _node)
		{
			eventChangeSize(this, getViewSize());
			mCanvas->updateTexture();
		}

		void requestUpdateCanvas(MyGUI::CanvasPtr _canvas, MyGUI::Canvas::Event _event)
		{
			if ( ! _event.textureChanged && ! _event.requested ) return;

			unsigned char * data = (unsigned char*)_canvas->lock();

			int width = _canvas->getTextureRealWidth();
			int height = _canvas->getTextureRealHeight();

			clearCanvas((unsigned char*)data, width, height);

			// проходим по всем нодам и перерисовываем связи
			for (size_t index=0; index<mNodes.size(); ++index)
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

						drawCurve((unsigned char*)data, width, height, info);
					}
				}
			}

			// ниточка для драга
			if (mIsDrug)
				drawCurve((unsigned char*)data, width, height, mDrugLine);
			
			_canvas->unlock();
		}

		void connectPoint(BaseGraphConnection* _connection)
		{
			const MyGUI::IntPoint& mouse = MyGUI::InputManager::getInstance().getMousePosition();

			// пикаем виджет под нами
			MyGUI::WidgetPtr widget = MyGUI::LayerManager::getInstance().getWidgetFromPoint(mouse.left, mouse.top);
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

		void clearCanvas(unsigned char* _data, int _width, int _height)
		{
			agg::rendering_buffer rbuf;
			rbuf.attach(_data, _width, _height, _width*4);

			// Pixel format and basic primitives renderer
			agg::pixfmt_bgra32 pixf(rbuf);
			agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

			renb.clear(agg::rgba8(106, 147, 221, 0));
		}

		void drawCurve(unsigned char* _data, int _width, int _height, const ConnectionInfo& _info)
		{
			//============================================================ 
			// AGG
			agg::rendering_buffer rbuf;
			rbuf.attach(_data, _width, _height, _width*4);

			// Pixel format and basic primitives renderer
			agg::pixfmt_bgra32 pixf(rbuf);
			agg::renderer_base<agg::pixfmt_bgra32> renb(pixf);

			// Scanline renderer for solid filling.
			agg::renderer_scanline_aa_solid<agg::renderer_base<agg::pixfmt_bgra32> > ren(renb);

			// Rasterizer & scanline
			agg::rasterizer_scanline_aa<> ras;
			agg::scanline_p8 sl;

			// хранилище всех путей
			agg::path_storage path;

			// кривая безье которая строится по 4 точкам
			agg::curve4 curve;
			curve.approximation_method(agg::curve_approximation_method_e(agg::curve_inc)); // метод апроксимации, curve_inc - быстрый но много точек
			curve.approximation_scale(0.7); //масштаб апроксимации
			curve.angle_tolerance(agg::deg2rad(0));
			curve.cusp_limit(agg::deg2rad(0));
			const int offset = 3;
			curve.init(
				_info.point_start.left,
				_info.point_start.top + offset,
				_info.point_start.left + _info.start_offset.width,
				_info.point_start.top + _info.start_offset.height + offset,

				_info.point_end.left + _info.end_offset.width,
				_info.point_end.top + _info.end_offset.height + offset,
				_info.point_end.left,
				_info.point_end.top + offset);

			// добавляем путь безье
			path.concat_path(curve);

			// сам путь который рисуется, растерезатор
			agg::conv_stroke<agg::path_storage> stroke(path);
			stroke.width(2); // ширина линии
			stroke.line_join(agg::line_join_e(agg::bevel_join)); // хз че такое
			stroke.line_cap(agg::line_cap_e(agg::butt_cap)); //обрезка концов
			stroke.inner_join(agg::inner_join_e(agg::inner_miter)); // соединения внутри линии точек
			stroke.inner_miter_limit(1.01);

			ras.add_path(stroke);

			// Setting the attrribute (color) & Rendering
			ren.color(agg::rgba8(80, 80, 80, 200));
			agg::render_scanlines(ras, sl, ren);


			//============================================================ 
			// хранилище всех путей
			agg::path_storage path2;

			// кривая безье которая строится по 4 точкам
			agg::curve4 curve2;
			curve2.approximation_method(agg::curve_approximation_method_e(agg::curve_inc)); // метод апроксимации, curve_inc - быстрый но много точек
			curve2.approximation_scale(0.7); //масштаб апроксимации
			curve2.angle_tolerance(agg::deg2rad(0));
			curve2.cusp_limit(agg::deg2rad(0));
			curve2.init(
				_info.point_start.left,
				_info.point_start.top,
				_info.point_start.left + _info.start_offset.width,
				_info.point_start.top + _info.start_offset.height,

				_info.point_end.left + _info.end_offset.width,
				_info.point_end.top + _info.end_offset.height,
				_info.point_end.left,
				_info.point_end.top);

			// добавляем путь безье
			path2.concat_path(curve2);

			// сам путь который рисуется, растерезатор
			agg::conv_stroke<agg::path_storage> stroke2(path2);
			stroke2.width(2); // ширина линии
			stroke2.line_join(agg::line_join_e(agg::bevel_join)); // хз че такое
			stroke2.line_cap(agg::line_cap_e(agg::butt_cap)); //обрезка концов
			stroke2.inner_join(agg::inner_join_e(agg::inner_miter)); // соединения внутри линии точек
			stroke2.inner_miter_limit(1.01);

			ras.add_path(stroke2);

			// Setting the attrribute (color) & Rendering
			ren.color(agg::rgba8(_info.colour.red * 255, _info.colour.green * 255, _info.colour.blue * 255, 255));
			agg::render_scanlines(ras, sl, ren);
			//============================================================ 
		}

		MyGUI::IntSize getViewSize()
		{
			MyGUI::IntSize result;
			for (size_t index=0; index<mNodes.size(); ++index)
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
		typedef std::vector<BaseGraphNode*> VectorGraphNode;
		VectorGraphNode mNodes;

		MyGUI::CanvasPtr mCanvas;
		bool mIsDrug;
		ConnectionInfo mDrugLine;
		BaseGraphConnection* mConnectionStart;
	};

} // namespace wraps

#endif // __BASE_GRAPH_VIEW_H__

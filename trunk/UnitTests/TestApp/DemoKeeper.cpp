/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"
#include "MyGUI_RTTLayerFactory.h"
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_NodeAnimation.h"

namespace demo
{

	class CustomNodeAnimation : public MyGUI::NodeAnimation
	{
	public:
		virtual size_t animate(bool _update, MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, const MyGUI::RenderTargetInfo& _info, const MyGUI::IntCoord& _coord, const MyGUI::IntSize& _size)
		{
			if (true/*_update*/)
			{
				const int count_w = 3;
				const int count_h = 2;
				const int count = count_w * count_h * MyGUI::VertexQuad::VertexCount;

				// запрашивам нужный размер вершин
				_buffer->setVertextCount(count);

				MyGUI::VertexQuad* quad = (MyGUI::VertexQuad*)_buffer->lock();

				float vertex_z = _info.maximumDepth;

				float vertex_left = ((_info.pixScaleX * (float)(_coord.left) + _info.hOffset) * 2) - 1;
				float vertex_right = vertex_left + (_info.pixScaleX * (float)_size.width * 2);
				float vertex_top = -(((_info.pixScaleY * (float)(_coord.top) + _info.vOffset) * 2) - 1);
				float vertex_bottom = vertex_top - (_info.pixScaleY * (float)_size.height * 2);

				draw(
					vertex_left,
					vertex_top,

					vertex_right,
					vertex_top,

					vertex_left,
					vertex_bottom,

					vertex_right,
					vertex_bottom,

					vertex_z,
					
					0, 0, 1, 1,

					0xFFFFFFFF,

					quad,

					count_w,
					count_h
					);

				/*if (_info.rttFlipY)
				{
					quad->vertex[MyGUI::VertexQuad::CornerLT].v = quad->vertex[MyGUI::VertexQuad::CornerLB].v;
					quad->vertex[MyGUI::VertexQuad::CornerLB].v = quad->vertex[MyGUI::VertexQuad::CornerRT].v;

					quad->vertex[MyGUI::VertexQuad::CornerRT].v = quad->vertex[MyGUI::VertexQuad::CornerLT].v;
					quad->vertex[MyGUI::VertexQuad::CornerRT2].v = quad->vertex[MyGUI::VertexQuad::CornerLT].v;

					quad->vertex[MyGUI::VertexQuad::CornerRB].v = quad->vertex[MyGUI::VertexQuad::CornerLB].v;
					quad->vertex[MyGUI::VertexQuad::CornerLB2].v = quad->vertex[MyGUI::VertexQuad::CornerLB].v;
				}*/

				_buffer->unlock();

				return count;
			}

		}

		void draw(float _1l, float _1t, float _2r, float _2t, float _3l, float _3b, float _4r, float _4b, float _z, float _u1, float _v1, float _u2, float _v2, unsigned int _colour, MyGUI::VertexQuad* _quad, int _w, int _h)
		{
			_quad->set(
				_1l, _1t, _4r, _4b, _z,
				_u1, _v1, _u2, _v2, _colour
				);
		}
	};

	CustomNodeAnimation gCustomNodeAnimation;

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

    void DemoKeeper::createScene()
    {

		MyGUI::LayerManager::getInstance().addLayerFactory("RTTLayer", new MyGUI::RTTLayerFactory());

		this->addResourceLocation("../../Media/UnitTests/TestApp");

		mGUI->load("test_layer.xml");

		MyGUI::WindowPtr widget = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(56, 16, 512, 512), MyGUI::Align::Default, "RTT_Test");
		MyGUI::WidgetPtr widget2 = widget->createWidget<MyGUI::Widget>("EditStretch", MyGUI::IntCoord(116, 116, 164, 164), MyGUI::Align::Default, "RTT_Test");

	}

    void DemoKeeper::destroyScene()
    {
    }

	void setCornerData(MyGUI::VertexQuad& _quad, MyGUI::VertexQuad::Enum _corner, float _x, float _y, float _z, float _u, float _v, unsigned int _colour, bool _flipY)
	{
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		MyGUI::EnumeratorLayer layer = MyGUI::LayerManager::getInstance().getEnumerator();
		while(layer.next())
		{
			if (layer->getName() == "RTT_Test")
			{
				MyGUI::EnumeratorILayerNode node = layer->getEnumerator();
				while(node.next())
				{
					MyGUI::RTTLayerNode* rttnode = node->castType<MyGUI::RTTLayerNode>(false);
					if (rttnode != nullptr)
					{
						rttnode->setCacheUsing(!rttnode->getCacheUsing());

						if (rttnode->getCacheUsing())
						{
							if (rttnode->getNodeAnimation() != nullptr)
							{
								rttnode->setNodeAnimation(nullptr);
							}
							else
							{
								rttnode->setNodeAnimation(&gCustomNodeAnimation);
							}
						}
					}
				}
			}
		}

		return BaseManager::keyPressed( arg );
	}

} // namespace demo

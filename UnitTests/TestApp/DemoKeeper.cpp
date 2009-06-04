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

#include "Ogre.h"

namespace demo
{
	using namespace MyGUI;


	class CustomNodeAnimation : public MyGUI::NodeAnimation
	{
	public:
		virtual size_t animate(bool _update, MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, const MyGUI::RenderTargetInfo& _info, const MyGUI::IntCoord& _coord, const MyGUI::IntSize& _size)
		{
			if (true/*_update*/)
			{
				const int count_w = 8;
				const int count_h = 8;
				const int count = count_w * count_h * MyGUI::VertexQuad::VertexCount;

				// запрашивам нужный размер вершин
				_buffer->setVertextCount(count);

				MyGUI::VertexQuad* quad = (MyGUI::VertexQuad*)_buffer->lock();

				float vertex_z = _info.maximumDepth;

				float vertex_left = ((_info.pixScaleX * (float)(_coord.left) + _info.hOffset) * 2) - 1;
				float vertex_right = vertex_left + (_info.pixScaleX * (float)_size.width * 2);
				float vertex_top = -(((_info.pixScaleY * (float)(_coord.top) + _info.vOffset) * 2) - 1);
				float vertex_bottom = vertex_top - (_info.pixScaleY * (float)_size.height * 2);

				tesselateQuad(
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

		void tesselateQuad(float _x1, float _y1, float _x2, float _y2, float _x3, float _y3, float _x4, float _y4, float _z, float _u1, float _v1, float _u2, float _v2, uint32 _colour, MyGUI::VertexQuad * _quads, int _tesX, int _tesY)
		{
			Ogre::Vector2 LT(_x1, _y1);
			Ogre::Vector2 RT(_x2, _y2);
			Ogre::Vector2 RB(_x4, _y4);
			Ogre::Vector2 LB(_x3, _y3);
			Ogre::Vector2 baseDYLeft = (LB - LT)/_tesY;
			Ogre::Vector2 baseDYRight = (RB - RT)/_tesY;

			Ogre::Vector2 LTUV(_x1, _y1);

			float dXUV = (_u2 - _u1)/_tesX;
			float dYUV = (_v2 - _v1)/_tesY;

			for (int i = 0; i <= _tesX; ++i)
			{
				for (int j = 0; j <= _tesY; ++j)
				{
					Ogre::Vector2 point1 = LT + baseDYLeft * j;
					Ogre::Vector2 point2 = RT + baseDYRight * j;
					Ogre::Vector2 point = point1 + (point2 - point1)*i/_tesX;

					float u = _u1 + dXUV * i;
					float v = _v1 + dYUV * j;

					if (i < _tesX && j < _tesY)
					{
						_quads[i + j*_tesX].vertex[VertexQuad::CornerLT].x = point.x;
						_quads[i + j*_tesX].vertex[VertexQuad::CornerLT].y = point.y;
						_quads[i + j*_tesX].vertex[VertexQuad::CornerLT].u = u;
						_quads[i + j*_tesX].vertex[VertexQuad::CornerLT].v = v;
					}

					if (i > 0 && j > 0)
					{
						_quads[(i-1) + (j-1)*_tesX].vertex[VertexQuad::CornerRB].x = point.x;
						_quads[(i-1) + (j-1)*_tesX].vertex[VertexQuad::CornerRB].y = point.y;
						_quads[(i-1) + (j-1)*_tesX].vertex[VertexQuad::CornerRB].u = u;
						_quads[(i-1) + (j-1)*_tesX].vertex[VertexQuad::CornerRB].v = v;
					}

					if (i > 0 && j < _tesY)
					{
						_quads[(i-1) + j*_tesX].vertex[VertexQuad::CornerRT].x = point.x;
						_quads[(i-1) + j*_tesX].vertex[VertexQuad::CornerRT].y = point.y;
						_quads[(i-1) + j*_tesX].vertex[VertexQuad::CornerRT].u = u;
						_quads[(i-1) + j*_tesX].vertex[VertexQuad::CornerRT].v = v;
					}

					if (i < _tesX && j > 0)
					{
						_quads[i + (j-1)*_tesX].vertex[VertexQuad::CornerLB].x = point.x;
						_quads[i + (j-1)*_tesX].vertex[VertexQuad::CornerLB].y = point.y;
						_quads[i + (j-1)*_tesX].vertex[VertexQuad::CornerLB].u = u;
						_quads[i + (j-1)*_tesX].vertex[VertexQuad::CornerLB].v = v;
					}
				}
			}

			for (int i = 0; i < _tesX; ++i)
			{
				for (int j = 0; j < _tesY; ++j)
				{		
					for (int k = 0; k < 6; ++k)
					{
						_quads[i + j*_tesX].vertex[k].z = _z;
						_quads[i + j*_tesX].vertex[k].colour = _colour;

						_quads[i + j*_tesX].vertex[k].z = _z;
						_quads[i + j*_tesX].vertex[k].colour = _colour;
					}

					_quads[i + j*_tesX].vertex[VertexQuad::CornerRT2] = _quads[i + j*_tesX].vertex[VertexQuad::CornerRT];
					_quads[i + j*_tesX].vertex[VertexQuad::CornerLB2] = _quads[i + j*_tesX].vertex[VertexQuad::CornerLB];
				}
			}
		}
	};

	CustomNodeAnimation gCustomNodeAnimation;

	DemoKeeper::DemoKeeper() :
		base::BaseManager()
	{
	}

	MyGUI::WindowPtr widget = nullptr;

    void DemoKeeper::createScene()
    {

		MyGUI::LayerManager::getInstance().addLayerFactory("RTTLayer", new MyGUI::RTTLayerFactory());

		this->addResourceLocation("../../Media/UnitTests/TestApp");

		mGUI->load("test_layer.xml");

		widget = mGUI->createWidget<MyGUI::Window>("WindowCSX", MyGUI::IntCoord(56, 16, 512, 512), MyGUI::Align::Default, "RTT_Test");
		widget->setCaption("RTT mode");

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
		if (arg.key == OIS::KC_1)
		{
			mCamera->setPolygonMode(Ogre::PM_SOLID);
		}
		else if (arg.key == OIS::KC_2)
		{
			mCamera->setPolygonMode(Ogre::PM_WIREFRAME);
		}
		else if (arg.key == OIS::KC_3)
		{
			mCamera->setPolygonMode(Ogre::PM_POINTS);
		}
		else if (arg.key == OIS::KC_SPACE)
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
									widget->setCaption("RTT mode");
								}
								else
								{
									rttnode->setNodeAnimation(&gCustomNodeAnimation);
									widget->setCaption("Abstract mode");
								}
							}
							else
							{
								widget->setCaption("Vertext mode");
							}

						}
					}
				}
			}
		}
		return BaseManager::keyPressed( arg );
	}

} // namespace demo

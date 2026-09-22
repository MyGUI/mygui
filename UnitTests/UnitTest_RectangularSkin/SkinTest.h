#ifndef MYGUI_UNITTEST_RECTANGULAR_SKIN_TEST_H_
#define MYGUI_UNITTEST_RECTANGULAR_SKIN_TEST_H_

#include "SkinTestContext.h"
#include "TestRunner.h"
#include "MyGUI_LayerNode.h"

namespace unittest::rectangularskin
{

	struct ExpectedQuad
	{
		MyGUI::IntCoord coord;
		MyGUI::FloatRect uv;
	};

	class Renderer : public unittest::SkinRenderManager
	{
	public:
		explicit Renderer(MyGUI::VertexColourType _format) :
			mFormat(_format),
			info(unittest::SkinRenderManager::getInfo())
		{
			info.maximumDepth = 0.25f;
		}

		MyGUI::VertexColourType getVertexFormat() const override
		{
			return mFormat;
		}

		const MyGUI::RenderTargetInfo& getInfo() const override
		{
			return info;
		}

		void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			unittest::SkinRenderManager::doRender(_buffer, _texture, _count);
			const auto& source = static_cast<unittest::SkinVertexBuffer*>(_buffer)->vertices;
			vertices.insert(vertices.end(), source.begin(), source.begin() + _count);
		}

	private:
		MyGUI::VertexColourType mFormat;

	public:
		MyGUI::RenderTargetInfo info;
		std::vector<MyGUI::Vertex> vertices;
	};

	class ParentRectangle : public MyGUI::ICroppedRectangle
	{
	public:
		ParentRectangle()
		{
			setSize({100, 80});
			mAbsolutePosition = {100, 60};
		}

		void setAbsolutePosition(const MyGUI::IntPoint& _position)
		{
			mAbsolutePosition = _position;
		}

		void setMargin(const MyGUI::IntRect& _margin)
		{
			mMargin = _margin;
		}
	};

	template<typename Skin>
	class Fixture
	{
	public:
		explicit Fixture(MyGUI::VertexColourType _format = MyGUI::VertexColourType::ColourARGB) :
			renderer(_format)
		{
			skin._setCroppedParent(&parent);
			skin.setAlign(MyGUI::Align::Default);
			setCoord({10, 20, 40, 30});
			skin._setUVSet({0.2f, 0.1f, 0.8f, 0.9f});
			attach();
		}

		~Fixture()
		{
			if (mAttached)
				skin.destroyDrawItem();
		}

		void attach()
		{
			skin.createDrawItem(renderer.getTexture("MyGUI_BlueWhiteSkins.png"), &node);
			mAttached = true;
		}

		void detach()
		{
			skin.destroyDrawItem();
			mAttached = false;
		}

		void setCoord(const MyGUI::IntCoord& _coord)
		{
			skin.setCoord(_coord);
			// As in Widget, alignment initializes the current rectangle before updating its clipping.
			skin._setAlign(parent.getSize());
		}

		void draw()
		{
			renderer.vertices.clear();
			// Exercise invalidation: a forced rebuild would conceal stale geometry after a setter.
			node.renderToTarget(&renderer, false);
		}

		void expectQuads(const std::vector<ExpectedQuad>& _quads, MyGUI::uint32 _colour = 0xFFFFFFFF)
		{
			draw();
			require(renderer.vertices.size() == _quads.size() * 6, "Unexpected number of rectangle vertices");
			for (size_t index = 0; index < _quads.size(); ++index)
				checkQuad(index, _quads[index].coord, _quads[index].uv, _colour);
		}

		void expectQuad(const MyGUI::IntCoord& _coord, const MyGUI::FloatRect& _uv, MyGUI::uint32 _colour = 0xFFFFFFFF)
		{
			expectQuads({{_coord, _uv}}, _colour);
		}

	private:
		void checkQuad(size_t _quad, const MyGUI::IntCoord& _coord, const MyGUI::FloatRect& _uv, MyGUI::uint32 _colour)
		{
			const bool right[] = {false, true, false, false, true, true};
			const bool bottom[] = {false, false, true, true, false, true};
			for (size_t index = 0; index < 6; ++index)
			{
				const auto& vertex = renderer.vertices[_quad * 6 + index];
				// Convert back to screen pixels to check the public geometry contract.
				const auto& info = renderer.info;
				const float x = ((vertex.x + 1) / 2 - info.hOffset) / info.pixScaleX + info.leftOffset;
				const float y = ((1 - vertex.y) / 2 - info.vOffset) / info.pixScaleY + info.topOffset;
				require(
					std::abs(x - (right[index] ? _coord.right() : _coord.left)) < 0.001f &&
						std::abs(y - (bottom[index] ? _coord.bottom() : _coord.top)) < 0.001f,
					"Skin triangle positions or winding differ from the expected rectangle");
				require(
					std::abs(vertex.u - (right[index] ? _uv.right : _uv.left)) < 0.00001f &&
						std::abs(vertex.v - (bottom[index] ? _uv.bottom : _uv.top)) < 0.00001f,
					"Skin UVs differ from the expected texture rectangle");
				require(vertex.z == info.maximumDepth, "Skin must use the layer node depth");
				require(vertex.colour == _colour, "Skin packed colour or alpha is incorrect");
			}
		}

	public:
		Renderer renderer;
		ParentRectangle parent;
		MyGUI::LayerNode node{nullptr};
		Skin skin;

	private:
		bool mAttached{false};
	};

	using Tests = std::vector<TestCase>;
	void addSubSkinTests(Tests& _tests);
	void addTileRectTests(Tests& _tests);

}

#endif // MYGUI_UNITTEST_RECTANGULAR_SKIN_TEST_H_

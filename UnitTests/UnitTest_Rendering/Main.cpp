#include "TestSupport.h"
#include "TestRunner.h"
#include "SkinTestContext.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_TextureUtility.h"
#include "TextRendering.h"
#include <cmath>

namespace
{

	using unittest::require;

	class RecordingBuffer : public unittest::SkinVertexBuffer
	{
	public:
		MyGUI::Vertex* lock() override
		{
			++locks;
			return failLock ? nullptr : SkinVertexBuffer::lock();
		}
		void unlock() override
		{
			++unlocks;
		}
		int locks{0};
		int unlocks{0};
		bool failLock{false};
	};

	class RecordingRenderer : public unittest::SkinRenderManager
	{
	public:
		MyGUI::IVertexBuffer* createVertexBuffer() override
		{
			++created;
			lastBuffer = new RecordingBuffer();
			return lastBuffer;
		}
		void destroyVertexBuffer(MyGUI::IVertexBuffer* _buffer) override
		{
			++destroyed;
			delete _buffer;
		}
		void doRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			require(_count > 0 && _count <= _buffer->getVertexCount(), "Draw must use populated buffer storage");
			auto* buffer = static_cast<RecordingBuffer*>(_buffer);
			std::vector<MyGUI::uint32> colours;
			for (size_t index = 0; index < _count; ++index)
				colours.push_back(buffer->vertices[index].colour);
			draws.push_back({_texture, colours});
		}
		struct Draw
		{
			MyGUI::ITexture* texture;
			std::vector<MyGUI::uint32> colours;
		};
		std::vector<Draw> draws;
		RecordingBuffer* lastBuffer{nullptr};
		int created{0};
		int destroyed{0};
	};

	// Emits identifiable vertices so assertions observe submission order and actual counts.
	class DrawItem : public MyGUI::ISubWidget
	{
	public:
		DrawItem(MyGUI::RenderItem* _batch, MyGUI::uint32 _colour, size_t _reserved = 6) :
			batch(_batch),
			colour(_colour)
		{
			batch->addDrawItem(this, _reserved);
		}
		~DrawItem() override
		{
			batch->removeDrawItem(this);
		}
		void createDrawItem(MyGUI::ITexture*, MyGUI::ILayerNode*) override
		{
		}
		void destroyDrawItem() override
		{
		}
		void doRender() override
		{
			++renders;
			update = batch->getCurrentUpdate();
			target = batch->getRenderTarget();
			auto* vertices = batch->getCurrentVertexBuffer();
			for (size_t index = 0; index < count; ++index)
			{
				vertices[index] = {};
				vertices[index].colour = colour;
			}
			batch->setLastVertexCount(count);
		}
		void doManualRender(MyGUI::IVertexBuffer* _buffer, MyGUI::ITexture* _texture, size_t _count) override
		{
			++manualRenders;
			manualBuffer = _buffer;
			manualTexture = _texture;
			manualCount = _count;
		}
		MyGUI::RenderItem* batch;
		MyGUI::uint32 colour;
		size_t count{3};
		int renders{0};
		bool update{false};
		MyGUI::IRenderTarget* target{nullptr};
		int manualRenders{0};
		MyGUI::IVertexBuffer* manualBuffer{nullptr};
		MyGUI::ITexture* manualTexture{nullptr};
		size_t manualCount{0};
	};

	void testBufferUpdates()
	{
		RecordingRenderer renderer;
		auto* texture = renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		{
			MyGUI::RenderItem batch;
			batch.setTexture(texture);
			DrawItem first(&batch, 1);
			{
				DrawItem second(&batch, 2);
				require(batch.getNeedVertexCount() == 12, "Reservations must accumulate");
				batch.renderToTarget(&renderer, false);
				require(renderer.lastBuffer->getVertexCount() == 12, "Buffer must fit all reservations");
				require(batch.getVertexCount() == 6, "Submission must use emitted vertices, not reserved capacity");
				require(renderer.draws.back().texture == texture, "Submission must retain the batch texture");
				require(
					renderer.draws.back().colours == std::vector<MyGUI::uint32>({1, 1, 1, 2, 2, 2}),
					"Partially filled reservations must pack without gaps");
				require(
					first.target == &renderer && !first.update,
					"Draw callbacks must receive target and update state");
				batch.renderToTarget(&renderer, false);
				require(
					first.renders == 1 && second.renders == 1 && renderer.lastBuffer->locks == 1,
					"A clean frame must submit cached geometry without locking or regenerating it");
				batch.outOfDate();
				batch.renderToTarget(&renderer, false);
				require(first.renders == 2 && !batch.isOutOfDate(), "Dirty geometry must rebuild once");
				batch.renderToTarget(&renderer, true);
				require(first.renders == 3 && first.update, "Forced updates must rebuild clean geometry");
				batch.reallockDrawItem(&first, 3);
				require(
					batch.getNeedVertexCount() == 12 && !batch.isOutOfDate(),
					"Smaller requests must preserve capacity without invalidating geometry");
				batch.reallockDrawItem(&first, 9);
				first.count = 9;
				require(
					batch.getNeedVertexCount() == 15 && batch.isOutOfDate(),
					"Growth must reserve space and dirty geometry");
				batch.renderToTarget(&renderer, false);
				require(
					batch.getVertexCount() == 12 && renderer.draws.back().colours.back() == 2,
					"Growing one item must preserve subsequent geometry");
			}
			batch.renderToTarget(&renderer, false);
			require(
				batch.getNeedVertexCount() == 9 && renderer.draws.back().colours == std::vector<MyGUI::uint32>(9, 1),
				"Removing an item must remove its reservation and submitted vertices");
			require(
				renderer.lastBuffer->locks == renderer.lastBuffer->unlocks,
				"Every successful lock must be unlocked");
		}
		require(renderer.created == 1 && renderer.destroyed == 1, "A render item must release its vertex buffer");
	}

	void testEmptyAndFailedDraws()
	{
		RecordingRenderer renderer;
		MyGUI::RenderItem batch;
		batch.renderToTarget(&renderer, true);
		require(
			renderer.lastBuffer->locks == 0 && renderer.draws.empty(),
			"Textureless batches must skip buffer access");
		batch.setTexture(renderer.getTexture("MyGUI_BlueWhiteSkins.png"));
		{
			DrawItem item(&batch, 1);
			item.count = 0;
			batch.renderToTarget(&renderer, false);
			require(renderer.draws.empty(), "Zero emitted vertices must not submit a draw");
			item.count = 3;
			batch.outOfDate();
			batch.renderToTarget(&renderer, false);
			require(renderer.draws.size() == 1, "Geometry must resume after an empty frame");
			renderer.draws.clear();
			renderer.lastBuffer->failLock = true;
			batch.renderToTarget(&renderer, true);
			require(
				item.renders == 2 && renderer.lastBuffer->unlocks == 2 && renderer.draws.empty(),
				"Failed locks must skip callbacks, unlock, and stale geometry submission");
			renderer.lastBuffer->failLock = false;
			batch.outOfDate();
			batch.renderToTarget(&renderer, false);
			require(renderer.draws.size() == 1, "Invalidation must allow recovery after a failed lock");
		}
		require(
			batch.getTexture() == nullptr && batch.getNeedCompression() && batch.getNeedVertexCount() == 0,
			"Removing the last item must release the texture and mark the batch for compression");
		renderer.draws.clear();
		batch.renderToTarget(&renderer, true);
		require(renderer.draws.empty(), "An emptied batch must not submit stale geometry");
	}

	void testManualRendering()
	{
		RecordingRenderer renderer;
		MyGUI::RenderItem batch;
		auto* texture = renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		batch.setTexture(texture);
		batch.setManualRender(true);
		DrawItem item(&batch, 1);
		batch.renderToTarget(&renderer, false);
		batch.renderToTarget(&renderer, false);
		require(
			renderer.draws.empty() && item.renders == 1 && item.manualRenders == 2,
			"Manual callbacks must run each frame while reusing clean geometry");
		require(
			item.manualBuffer == renderer.lastBuffer && item.manualTexture == texture && item.manualCount == 3,
			"Manual rendering must receive the buffer, texture, and actual vertex count");
		item.count = 0;
		batch.outOfDate();
		batch.renderToTarget(&renderer, false);
		require(item.manualRenders == 2, "Empty manual batches must skip callbacks");
	}

	void testBatchQueues()
	{
		RecordingRenderer renderer;
		unittest::AtlasTexture otherTexture;
		auto* texture = renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		{
			MyGUI::LayerNode node(nullptr);
			DrawItem first(node.addToRenderItem(texture, true, false), 1);
			DrawItem adjacent(node.addToRenderItem(texture, true, false), 2);
			DrawItem different(node.addToRenderItem(&otherTexture, true, false), 3);
			DrawItem later(node.addToRenderItem(texture, true, false), 4);
			require(
				first.batch == adjacent.batch && first.batch != later.batch,
				"First queue may batch adjacent textures but must preserve order across texture changes");
			DrawItem text(node.addToRenderItem(texture, false, false), 5);
			DrawItem otherText(node.addToRenderItem(&otherTexture, false, false), 6);
			DrawItem moreText(node.addToRenderItem(texture, false, false), 7);
			require(
				text.batch == moreText.batch && text.batch != first.batch,
				"Second queue must batch matching textures independently of insertion order and first queue");
			auto* child = node.createChildItemNode();
			DrawItem childItem(child->addToRenderItem(texture, true, false), 8);
			node.renderToTarget(&renderer, false);
			require(renderer.draws.size() == 6, "Queues and child nodes must produce six batches");
			const std::vector<std::vector<MyGUI::uint32>>
				expected{{1, 1, 1, 2, 2, 2}, {3, 3, 3}, {4, 4, 4}, {5, 5, 5, 7, 7, 7}, {6, 6, 6}, {8, 8, 8}};
			for (size_t index = 0; index < expected.size(); ++index)
				require(
					renderer.draws[index].colours == expected[index],
					"Draw order must be first queue, second queue, children");
			require(!node.isOutOfDate(), "Rendering must clear node and batch invalidation");
			child->outOfDate(childItem.batch);
			require(node.isOutOfDate(), "Child batch invalidation must propagate to the parent");
			node.renderToTarget(&renderer, false);
			require(childItem.renders == 2 && first.renders == 1, "Child invalidation must rebuild only its geometry");
		}
		require(renderer.created == renderer.destroyed, "Node destruction must release both queues and child buffers");
	}

	void testBatchReuseAndManualIsolation()
	{
		RecordingRenderer renderer;
		unittest::AtlasTexture otherTexture;
		auto* texture = renderer.getTexture("MyGUI_BlueWhiteSkins.png");
		MyGUI::LayerNode node(nullptr);
		auto* empty = node.addToRenderItem(texture, true, false);
		{
			DrawItem removed(empty, 1);
		}
		DrawItem retained(node.addToRenderItem(&otherTexture, true, false), 2);
		node.renderToTarget(&renderer, false);
		DrawItem reused(node.addToRenderItem(texture, true, false), 3);
		require(
			reused.batch == empty && renderer.created == 2,
			"Compression must make empty first-queue buffers reusable");
		DrawItem manual(node.addToRenderItem(texture, true, true), 4);
		DrawItem afterManual(node.addToRenderItem(texture, true, false), 5);
		require(
			manual.batch != reused.batch && manual.batch != afterManual.batch,
			"Manual first-queue items must remain isolated from ordinary batches");
		auto* secondEmpty = node.addToRenderItem(texture, false, false);
		{
			DrawItem removed(secondEmpty, 6);
		}
		DrawItem secondReuse(node.addToRenderItem(&otherTexture, false, false), 7);
		require(secondReuse.batch == secondEmpty, "Second queue must reuse an empty buffer for a different texture");
		renderer.draws.clear();
		node.renderToTarget(&renderer, false);
		require(
			renderer.draws.size() == 4 && manual.manualRenders == 1,
			"Compressed queues must render each surviving batch once");
		const std::vector<MyGUI::uint32> expected{2, 3, 5, 7};
		for (size_t index = 0; index < expected.size(); ++index)
			require(
				renderer.draws[index].colours == std::vector<MyGUI::uint32>(3, expected[index]),
				"Buffer reuse must preserve surviving draw order and replace removed geometry");
	}

	void testColourPacking()
	{
		const MyGUI::Colour colour(0x12 / 255.0f, 0x34 / 255.0f, 0x56 / 255.0f, 0x78 / 255.0f);
		require(
			MyGUI::texture_utility::toNativeColour(colour, MyGUI::VertexColourType::ColourARGB) == 0x78123456,
			"ARGB packing must preserve alpha and RGB channels");
		require(
			MyGUI::texture_utility::toNativeColour(colour, MyGUI::VertexColourType::ColourABGR) == 0x78563412,
			"ABGR packing must swap red and blue");
		MyGUI::uint32 packed = 0x78123456;
		MyGUI::texture_utility::convertColour(packed, MyGUI::VertexColourType::ColourARGB);
		require(packed == 0x78123456, "ARGB conversion must preserve the packed value");
		MyGUI::texture_utility::convertColour(packed, MyGUI::VertexColourType::ColourABGR);
		require(packed == 0x78563412, "ABGR conversion must preserve alpha and green while swapping red and blue");
	}

	class RotationTarget : public MyGUI::IRenderTarget
	{
	public:
		MyGUI::RenderTargetInfo info;
		std::vector<MyGUI::Vertex> vertices;
		size_t draws{0};

		void begin() override
		{
		}
		void end() override
		{
		}
		const MyGUI::RenderTargetInfo& getInfo() const override
		{
			return info;
		}

		void doRender(MyGUI::IVertexBuffer* buffer, MyGUI::ITexture*, size_t count) override
		{
			require(count <= buffer->getVertexCount() && count % 3 == 0, "Invalid triangle buffer");
			++draws;
			const auto& data = static_cast<unittest::SkinVertexBuffer*>(buffer)->vertices;
			vertices.insert(vertices.end(), data.begin(), data.begin() + count);
		}

		void render()
		{
			vertices.clear();
			draws = 0;
			// Setters must invalidate cached geometry without a forced update.
			MyGUI::LayerManager::getInstance().renderToTarget(this, false);
		}

		MyGUI::FloatPoint pixel(const MyGUI::Vertex& v) const
		{
			return MyGUI::FloatPoint(
				((v.x + 1) * 0.5f - info.hOffset) / info.pixScaleX + info.leftOffset,
				((1 - v.y) * 0.5f - info.vOffset) / info.pixScaleY + info.topOffset);
		}

		float triangleArea(size_t index) const
		{
			const auto a = pixel(vertices[index]);
			const auto b = pixel(vertices[index + 1]);
			const auto c = pixel(vertices[index + 2]);
			return std::abs((b.left - a.left) * (c.top - a.top) - (b.top - a.top) * (c.left - a.left)) * 0.5f;
		}

		void expectRectangle(const MyGUI::FloatRect& expected) const
		{
			require(!vertices.empty(), "Expected rectangle must remain visible");
			for (const auto& v : vertices)
			{
				const auto p = pixel(v);
				require(
					p.left >= expected.left - 0.01f && p.left <= expected.right + 0.01f &&
						p.top >= expected.top - 0.01f && p.top <= expected.bottom + 0.01f,
					"Rendered vertices must stay inside the expected rectangle");
			}
			float area = 0;
			for (size_t i = 0; i < vertices.size(); i += 3)
				area += triangleArea(i);
			require(
				std::abs(area - (expected.right - expected.left) * (expected.bottom - expected.top)) < 0.1f,
				"Rendered triangles must cover the entire expected rectangle");
		}
	};

	void testRotatedSubSkinClipping()
	{
		unittest::SkinTestContext context;
		context.loadSkins();
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);

		auto* parent = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(
			"Default",
			MyGUI::IntCoord(100, 100, 100, 100),
			MyGUI::Align::Default,
			"Main");
		auto* child =
			parent->createWidget<MyGUI::Widget>("WhiteSkin", MyGUI::IntCoord(120, 20, 60, 40), MyGUI::Align::Default);
		child->setRotationCenter(MyGUI::FloatPoint(0, 0));
		child->setRotation(1.5707963268f);

		RotationTarget target;
		target.info = context.renderer().getInfo();
		target.render();
		require(!target.vertices.empty(), "Rotation must bring the initially clipped child into view");
		// Rotated bounds: [180, 220] x [120, 180]. Parent clips the right edge to 200.
		target.expectRectangle({180, 120, 200, 180});
	}

	void testRotationRenderInvalidation()
	{
		unittest::SkinTestContext context;
		context.loadSkins();
		auto& gui = MyGUI::Gui::getInstance();
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		auto* parent = gui.createWidget<MyGUI::Widget>("Default", {100, 100, 200, 200}, MyGUI::Align::Default, "Main");
		auto* child = parent->createWidget<MyGUI::Widget>("WhiteSkin", {40, 50, 60, 40}, MyGUI::Align::Default);
		RotationTarget target;
		target.info = context.renderer().getInfo();
		auto check = [&](const MyGUI::FloatRect& expected)
		{
			target.render();
			target.expectRectangle(expected);
		};
		check({140, 150, 200, 190});
		parent->setRotation(1.5707963268f);
		check({210, 140, 250, 200});
		parent->setPosition(120, 110);
		check({230, 150, 270, 210});
		parent->setSize(220, 240);
		check({260, 160, 300, 220});
		parent->setRotationCenter({0, 0});
		check({30, 150, 70, 210});
		parent->setRotation(0);
		check({160, 160, 220, 200});
		auto* other = gui.createWidget<MyGUI::Widget>("Default", {400, 100, 200, 200}, MyGUI::Align::Default, "Main");
		other->setRotation(1.5707963268f);
		child->attachToWidget(other);
		check({510, 140, 550, 200});
	}

	void testMixedRotatedBatchClipping()
	{
		unittest::SkinTestContext context;
		context.loadSkins();
		auto& gui = MyGUI::Gui::getInstance();
		MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
		auto* parent = gui.createWidget<MyGUI::Widget>("Default", {100, 100, 200, 200}, MyGUI::Align::Default, "Main");
		auto* rotated = parent->createWidget<MyGUI::Widget>("WhiteSkin", {0, 0, 200, 200}, MyGUI::Align::Default);
		rotated->setColour(MyGUI::Colour::Red);
		rotated->getSubWidgetMain()->_setUVSet({0, 0, 1, 1});
		rotated->setRotation(0.7853981634f);
		auto* plain = parent->createWidget<MyGUI::Widget>("WhiteSkin", {20, 20, 40, 30}, MyGUI::Align::Default);
		plain->setColour(MyGUI::Colour::Blue);
		RotationTarget target;
		target.info = context.renderer().getInfo();
		target.render();
		require(target.draws == 1, "Both skins must exercise the same rotated render batch");
		require(target.vertices.size() > 12, "Clipping the rotated square must grow the original two-quad buffer");
		const auto red =
			MyGUI::texture_utility::toNativeColour(MyGUI::Colour::Red, context.renderer().getVertexFormat());
		const auto blue =
			MyGUI::texture_utility::toNativeColour(MyGUI::Colour::Blue, context.renderer().getVertexFormat());
		float redArea = 0, blueArea = 0;
		for (const auto& v : target.vertices)
		{
			const auto p = target.pixel(v);
			const float x = p.left, y = p.top;
			if (v.colour == red)
			{
				require(
					x >= 99.99f && x <= 300.01f && y >= 99.99f && y <= 300.01f,
					"Rotated triangles must be clipped to the parent bounds");
				// Invert the known 45-degree rotation analytically, independently of Widget helpers.
				const float u = (100 + ((x - 200) + (y - 200)) / std::sqrt(2.0f)) / 200;
				const float w = (100 + ((y - 200) - (x - 200)) / std::sqrt(2.0f)) / 200;
				require(
					std::abs(v.u - u) < 0.0001f && std::abs(v.v - w) < 0.0001f,
					"Clipping must interpolate the rotated texture coordinates");
			}
			else
			{
				require(
					v.colour == blue && x >= 119.99f && x <= 160.01f && y >= 119.99f && y <= 150.01f,
					"Unrotated geometry in the same batch must keep its coordinates and colour");
			}
		}
		for (size_t i = 0; i < target.vertices.size(); i += 3)
		{
			const auto& a = target.vertices[i];
			const auto& b = target.vertices[i + 1];
			const auto& c = target.vertices[i + 2];
			require(a.colour == b.colour && b.colour == c.colour, "Triangles must not mix the two skins");
			const float area = target.triangleArea(i);
			(a.colour == red ? redArea : blueArea) += area;
		}
		const float cut = 200 - 100 * std::sqrt(2.0f);
		require(std::abs(redArea - (40000 - 2 * cut * cut)) < 0.2f, "Clipped square must cover the expected octagon");
		require(std::abs(blueArea - 1200) < 0.1f, "Unrotated rectangle must retain its full area");
	}

}

int main()
{
	return unittest::runTests({
		{"Text glyphs, colour tags, shadows and font loading", testTextGeometry},
		{"Text clipping and UV interpolation", testTextClipping},
		{"Multiline selection and cursor rendering", testTextSelectionAndCursor},
		{"Explicit text layout and render invalidation", testTextLayoutInvalidation},
		{"Text alignment updates layout before rendering", testTextAlignmentInvalidation},
		{"Render buffer allocation, packing, and invalidation", testBufferUpdates},
		{"Empty geometry and failed buffer locks", testEmptyAndFailedDraws},
		{"Manual render dispatch", testManualRendering},
		{"Batch queues and child invalidation", testBatchQueues},
		{"Batch compression, reuse, and manual isolation", testBatchReuseAndManualIsolation},
		{"Native vertex colour packing", testColourPacking},
		{"Rotated subskin clipping", testRotatedSubSkinClipping},
		{"Mixed rotated batch with clipping", testMixedRotatedBatchClipping},
		{"Rotation render-cache invalidation", testRotationRenderInvalidation},
	});
}

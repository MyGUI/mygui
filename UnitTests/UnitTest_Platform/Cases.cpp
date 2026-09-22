#include "PlatformFixture.h"
#include "MyGUI_DataStream.h"
#include "MyGUI_MaskPickInfo.h"
#include "MyGUI_FileSystemUtility.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <memory>

namespace platformtest
{

	namespace
	{

		constexpr Pixel white{255, 255, 255, 255};
		constexpr Pixel black{0, 0, 0, 255};
		constexpr Pixel red{255, 0, 0, 255};
		constexpr Pixel green{0, 255, 0, 255};
		constexpr Pixel blue{0, 0, 255, 255};
		const std::array<Pixel, 8> pngPixels{
			{black,
			 {255, 255, 255, 0},
			 {173, 49, 231, 128},
			 black,
			 {10, 20, 30, 255},
			 {40, 50, 60, 0},
			 {70, 80, 90, 64},
			 {100, 110, 120, 255}}};

		std::vector<unsigned char> pngBytes()
		{
			std::vector<unsigned char> result;
			for (auto pixel : pngPixels)
				result.insert(result.end(), {pixel[2], pixel[1], pixel[0], pixel[3]});
			return result;
		}

		void show(Fixture& f, MyGUI::ITexture* texture)
		{
			f.scene([&f, texture](MyGUI::IRenderTarget* target) { f.quad(target, texture); });
			f.capture();
		}

		void verifyPng(Fixture& f, MyGUI::ITexture* texture)
		{
			require(texture->getWidth() == 4 && texture->getHeight() == 2, "PNG must preserve dimensions");
			require(
				texture->getFormat() == MyGUI::PixelFormat::R8G8B8A8 && texture->getNumElemBytes() == 4,
				"PNG must preserve RGBA format");
			// Constant UVs sample exact texel centres; no filtering/edge ambiguity.
			f.scene(
				[&f, texture](MyGUI::IRenderTarget* target)
				{
					for (int y = 0; y < 2; ++y)
						for (int x = 0; x < 4; ++x)
						{
							const float u = (float(x) + 0.5f) / 4, v = (float(y) + 0.5f) / 2;
							f.quad(target, texture, white, {x * 32, y * 64, 32, 64}, {u, v, u, v});
						}
				});
			f.capture();
			for (int y = 0; y < 2; ++y)
				for (int x = 0; x < 4; ++x)
				{
					auto pixel = pngPixels[size_t(y * 4 + x)];
					for (size_t c = 0; c < 3; ++c)
						pixel[c] = static_cast<unsigned char>((int(pixel[c]) * pixel[3] + 127) / 255);
					f.expect(x * 32 + 16, y * 64 + 32, pixel);
				}
		}

		template<MyGUI::PixelFormat::Enum format, MyGUI::TextureUsage::Enum hint>
		void pattern(Fixture& f)
		{
			const auto usage = hint | MyGUI::TextureUsage::Write;
			requireSupport(f, format, usage);
			auto* texture = f.texture();
			texture->createManual(3, 2, usage, format);
			require(texture->getWidth() == 3 && texture->getHeight() == 2, "Odd-width texture dimensions");
			require(
				texture->getFormat() == format &&
					texture->getNumElemBytes() == size_t(MyGUI::PixelFormat(format).getBytesPerPixel()),
				"Texture format metadata");
			std::vector<unsigned char> bytes;
			std::array<Pixel, 6> expected{};
			for (size_t i = 0; i < expected.size(); ++i)
			{
				auto& pixel = expected[i];
				const auto luminance = static_cast<unsigned char>(17 + i * 31);
				pixel =
					{luminance, static_cast<unsigned char>(223 - i * 17), static_cast<unsigned char>(37 + i * 19), 255};
				if (format == MyGUI::PixelFormat::L8 || format == MyGUI::PixelFormat::L8A8)
				{
					pixel = {luminance, luminance, luminance, 255};
					bytes.push_back(luminance);
				}
				else
					bytes.insert(bytes.end(), {pixel[2], pixel[1], pixel[0]});
				if (format == MyGUI::PixelFormat::L8A8 || format == MyGUI::PixelFormat::R8G8B8A8)
				{
					pixel[3] = static_cast<unsigned char>(32 + i * 43);
					bytes.push_back(pixel[3]);
				}
				for (size_t c = 0; c < 3; ++c)
					pixel[c] = static_cast<unsigned char>((int(pixel[c]) * pixel[3] + 127) / 255);
			}
			f.scene(
				[&f, texture](MyGUI::IRenderTarget* target)
				{
					for (int y = 0; y < 2; ++y)
						for (int x = 0; x < 3; ++x)
						{
							const float u = (float(x) + 0.5f) / 3, v = (float(y) + 0.5f) / 2;
							f.quad(target, texture, white, {x * 40, y * 64, 40, 64}, {u, v, u, v});
						}
				});
			for (int update = 0; update < 3; ++update)
			{
				upload(texture, bytes);
				f.capture();
				for (int y = 0; y < 2; ++y)
					for (int x = 0; x < 3; ++x)
						f.expect(x * 40 + 20, y * 64 + 32, expected[size_t(y * 3 + x)]);
				std::rotate(bytes.begin(), bytes.begin() + texture->getNumElemBytes(), bytes.end());
				std::rotate(expected.begin(), expected.begin() + 1, expected.end());
			}
		}

		template<MyGUI::PixelFormat::Enum format, MyGUI::TextureUsage::Enum hint>
		void readWritePattern(Fixture& f)
		{
			const auto usage = hint | MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write;
			requireSupport(f, format, usage);
			for (auto size : {MyGUI::IntSize(1, 3), MyGUI::IntSize(3, 3), MyGUI::IntSize(5, 3), MyGUI::IntSize(4, 2)})
			{
				auto* texture = f.texture();
				texture->createManual(size.width, size.height, usage, format);
				std::vector<unsigned char> expected(size_t(size.width * size.height) * texture->getNumElemBytes());
				for (size_t i = 0; i < expected.size(); ++i)
					expected[i] = static_cast<unsigned char>(17 + i * 7);
				if (format == MyGUI::PixelFormat::R8G8B8A8 && size == MyGUI::IntSize(4, 2))
					expected = pngBytes();
				upload(texture, expected);
				require(read(texture) == expected, "Readback must reproduce tightly packed bytes");
				for (size_t edit : {expected.size() / 4, expected.size() - 1})
				{
					auto* bytes = static_cast<unsigned char*>(
						texture->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
					require(bytes != nullptr, "Read/write lock must succeed");
					const bool preserved = std::equal(expected.begin(), expected.end(), bytes);
					expected[edit] ^= 0x5a;
					bytes[edit] = expected[edit];
					texture->unlock();
					require(
						preserved && read(texture) == expected,
						"Partial update must preserve every other byte across padded rows");
				}
			}
		}

		MyGUI::ITexture* renderTexture(Fixture& f, int width, int height)
		{
			requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, MyGUI::TextureUsage::RenderTarget);
			auto* texture = f.texture();
			texture->createManual(width, height, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
			require(texture->getWidth() == width && texture->getHeight() == height, "RTT texture dimensions");
			auto* target = texture->getRenderTarget();
			require(target != nullptr, "Advertised render-target support must supply a target");
			const auto& info = target->getInfo();
			require(
				std::isfinite(info.pixScaleX) && std::isfinite(info.pixScaleY) && std::isfinite(info.aspectCoef),
				"RTT metadata must be finite");
			require(
				std::abs(info.pixScaleX * float(width) - 1) < 0.0001f &&
					std::abs(info.pixScaleY * float(height) - 1) < 0.0001f,
				"RTT pixel scale must match target dimensions");
			return texture;
		}

		void redraw(Fixture& f, MyGUI::ITexture* destination, MyGUI::ITexture* source)
		{
			auto* target = destination->getRenderTarget();
			target->begin();
			f.quad(target, source, white, {0, 0, destination->getWidth(), destination->getHeight()});
			target->end();
		}

		void widgets(Fixture& f)
		{
			auto& gui = MyGUI::Gui::getInstance();
			auto* parent =
				gui.createWidget<MyGUI::Widget>("PlatformSolid", {16, 16, 80, 80}, MyGUI::Align::Default, "Platform");
			parent->setColour(MyGUI::Colour(1, 0, 0));
			auto* child = parent->createWidget<MyGUI::Widget>("PlatformSolid", {48, 16, 64, 32}, MyGUI::Align::Default);
			child->setColour(MyGUI::Colour(0, 1, 0));
			f.capture();
			f.expect(24, 24, red);
			f.expect(72, 40, green);
			f.expect(104, 40, black);
			f.expect(8, 8, black);
			child->setVisible(false);
			f.capture();
			f.expect(72, 40, red);
			gui.destroyWidget(parent);
			f.capture();
			f.expectCorners(black);
		}

		void resourceStream(Fixture&)
		{
			auto& data = MyGUI::DataManager::getInstance();
			require(
				data.isDataExist("Known.txt") && !data.isDataExist("PlatformMissing.resource"),
				"Existing/missing resource query");
			auto stream = data.getDataHolder("Known.txt");
			require(bool(stream), "Known resource must open");
			const char expected[] = "MyGUI platform stream\n\0exact bytes\xff\n";
			require(stream->size() == sizeof(expected) - 1, "Exact binary stream size");
			std::array<char, 5> prefix{};
			require(stream->read(prefix.data(), prefix.size()) == prefix.size(), "Partial stream read");
			require(std::equal(prefix.begin(), prefix.end(), expected), "Stream prefix bytes");
			require(stream->size() == sizeof(expected) - 1, "Size must preserve stream position");
			require(
				stream->readAllText() == std::string(expected + 5, sizeof(expected) - 6),
				"Stream remainder including NUL and 0xff");
		}

		void textureLifecycle(Fixture& f)
		{
			auto* texture = f.texture("ReusableTexture");
			require(f.render().getTexture("ReusableTexture") == texture, "Texture lookup after creation");
			for (const auto size : {MyGUI::IntSize(3, 2), MyGUI::IntSize(7, 5), MyGUI::IntSize(1, 1)})
			{
				texture->createManual(
					size.width,
					size.height,
					MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
					MyGUI::PixelFormat::R8G8B8A8);
				upload(texture, std::vector<unsigned char>(size_t(size.width * size.height) * 4, 255));
				show(f, texture);
				f.expectCorners(white);
				f.scene({});
				f.capture();
				texture->destroy();
			}
			f.removeTexture(texture);
			require(f.render().getTexture("ReusableTexture") == nullptr, "Destroyed texture must leave registry");
			texture = f.texture("ReusableTexture");
			texture->loadFromFile("PlatformWhite.png");
			show(f, texture);
			f.expectCorners(white);
		}

		void duplicateTextureName(Fixture& f)
		{
			auto* original = f.texture("DuplicateTexture");
			bool rejected = false;
			MyGUI::ITexture* duplicate = nullptr;
			try
			{
				duplicate = f.render().createTexture("DuplicateTexture");
			}
			catch (const MyGUI::Exception&)
			{
				rejected = true;
			}
			// Registry corruption can make normal fixture ownership unsafe.
			if (f.render().getTexture("DuplicateTexture") != original || (duplicate && duplicate != original))
				throw Fatal("Duplicate creation replaced the registered texture");
			require(rejected, "Duplicate texture names must be rejected");
		}

		void unknownFormat(Fixture& f)
		{
			for (auto usage :
				 {MyGUI::TextureUsage::Default,
				  MyGUI::TextureUsage::Write,
				  MyGUI::TextureUsage::Read,
				  MyGUI::TextureUsage::RenderTarget})
				require(
					!f.render().isFormatSupported(MyGUI::PixelFormat::Unknow, usage),
					"Unknown pixel format must not be advertised");
		}

		void interleavedTextureLocks(Fixture& f)
		{
			checkInterleavedLocks(f);
		}

		void reloadRenderTargetFromFile(Fixture& f)
		{
			auto* texture = renderTexture(f, 3, 2);
			texture->loadFromFile("TransparentRgb.png");
			verifyPng(f, texture);
			upload(texture, std::vector<unsigned char>(32, 255));
			show(f, texture);
			f.expectCorners(white);
		}

		void sharedFileTextureLifetime(Fixture& f)
		{
			auto* owner = f.texture();
			owner->loadFromFile("TransparentRgb.png");
			auto* borrower = f.texture();
			borrower->loadFromFile("TransparentRgb.png");
			verifyPng(f, borrower);
			f.scene({});
			f.capture();
			f.removeTexture(borrower);
			verifyPng(f, owner);
			auto* replacement = f.texture();
			replacement->loadFromFile("TransparentRgb.png");
			verifyPng(f, replacement);
		}

		void loadedTextureWrite(Fixture& f)
		{
			auto* texture = f.texture();
			texture->loadFromFile("TransparentRgb.png");
			requireSupport(f, texture->getFormat(), texture->getUsage() | MyGUI::TextureUsage::Write);
			upload(texture, std::vector<unsigned char>(32, 255));
			show(f, texture);
			f.expectCorners(white);
		}

		void pngFidelity(Fixture& f)
		{
			auto* texture = f.texture();
			texture->loadFromFile("TransparentRgb.png");
			verifyPng(f, texture);
		}

		void pngReadableBytes(Fixture& f)
		{
			requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read);
			auto* texture = f.texture();
			texture->loadFromFile("TransparentRgb.png");
			require(read(texture) == pngBytes(), "PNG must preserve hidden RGB and top-down BGR(A) bytes");
		}

		void pngRoundtrip(Fixture& f)
		{
			unittest::TemporaryDirectory temporary("mygui-platform-");
			auto* source = f.texture();
			source->loadFromFile("TransparentRgb.png");
			source->saveToFile(MyGUI::utility::toUtf8(temporary.path() / "RoundTrip.PNG"));
			require(
				std::filesystem::exists(temporary.path() / "RoundTrip.PNG"),
				"PNG saving must create the requested file");
			verifyPng(f, source);
			f.addResourceLocation(temporary.path());
			auto* reloaded = f.texture();
			reloaded->loadFromFile("RoundTrip.PNG");
			verifyPng(f, reloaded);
			if (f.render().isFormatSupported(
					MyGUI::PixelFormat::R8G8B8A8,
					MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read))
				require(
					read(source) == pngBytes() && read(reloaded) == pngBytes(),
					"Roundtrip must independently preserve source and decoded bytes");
		}

		void filterLoaded(Fixture& f)
		{
			auto* texture = f.texture();
			texture->loadFromFile("TransparentRgb.png");
			f.scene([&f, texture](MyGUI::IRenderTarget* target)
					{ f.quad(target, texture, white, {0, 0, 128, 128}, {0.25f, 0.25f, 0.25f, 0.25f}); });
			f.capture();
			f.expectCorners({64, 64, 64, 255});
		}

		void filterUploaded(Fixture& f)
		{
			auto* texture = f.texture();
			texture->createManual(
				2,
				1,
				MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
				MyGUI::PixelFormat::R8G8B8A8);
			upload(texture, {0, 0, 0, 255, 255, 255, 255, 0});
			f.scene([&f, texture](MyGUI::IRenderTarget* target)
					{ f.quad(target, texture, white, {0, 0, 128, 128}, {0.5f, 0.5f, 0.5f, 0.5f}); });
			f.capture();
			f.expectCorners({64, 64, 64, 255});
		}

		void maskPicking(Fixture& f)
		{
			requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read);
			MyGUI::MaskPickInfo mask;
			require(mask.load("Mask.png") && !mask.empty(), "Readable PNG mask must load");
			require(
				mask.pick({16, 16}, {0, 0, 64, 64}) && mask.pick({48, 48}, {0, 0, 64, 64}),
				"White mask coordinates must be accepted");
			require(
				!mask.pick({48, 16}, {0, 0, 64, 64}) && !mask.pick({16, 48}, {0, 0, 64, 64}),
				"Nonwhite mask coordinates must be rejected");
		}

		void captureCurrentFrame(Fixture& f)
		{
			auto* texture = solid(f, white);
			Pixel colour = red;
			f.scene([&](MyGUI::IRenderTarget* target) { f.quad(target, texture, colour); });
			for (auto value : {red, blue, green, red})
			{
				colour = value;
				f.capture();
				f.expectCorners(value);
			}
		}

		void tintOverlapBlend(Fixture& f)
		{
			auto* texture = solid(f, white);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					f.quad(target, texture, blue);
					f.quad(target, texture, {255, 0, 0, 128}, {16, 16, 80, 80});
					f.quad(target, texture, {0, 255, 0, 255}, {64, 64, 32, 32});
				});
			f.capture();
			f.expect(8, 8, blue);
			f.expect(32, 32, {128, 0, 127, 255});
			f.expect(80, 80, green);
		}

		void streamGrowShrink(Fixture& f)
		{
			auto* texture = solid(f, white);
			auto* buffer = f.buffer();
			size_t capacity = 6;
			Pixel colour = red;
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					const size_t count = capacity / 6 * 6;
					const auto first = f.quadVertices(target->getInfo(), {0, 0, 40, 128}, colour);
					const auto middle = f.quadVertices(target->getInfo(), {44, 0, 40, 128}, blue);
					const auto last = f.quadVertices(target->getInfo(), {88, 0, 40, 128}, colour == red ? green : red);
					buffer->setVertexCount(capacity);
					require(buffer->getVertexCount() == capacity, "Vertex buffer must report requested count");
					auto* vertices = buffer->lock();
					require(vertices != nullptr, "Vertex write lock must succeed");
					std::fill_n(vertices, capacity, MyGUI::Vertex{});
					std::copy(first.begin(), first.end(), vertices);
					if (count > 6)
					{
						std::copy(middle.begin(), middle.end(), vertices + count / 12 * 6);
						std::copy(last.begin(), last.end(), vertices + count - 6);
					}
					buffer->unlock();
					target->doRender(buffer, texture, count);
				});
			for (int repeat = 0; repeat < 3; ++repeat)
				for (size_t count : {4096u, 6u, 2048u, 6u})
				{
					capacity = count;
					colour = colour == red ? green : red;
					f.capture();
					for (int y : {8, 64, 119})
					{
						f.expect(20, y, colour);
						f.expect(64, y, capacity > 6 ? blue : black);
						f.expect(108, y, capacity > 6 ? (colour == red ? green : red) : black);
					}
				}
		}

		Pixel queuedColour(int frame)
		{
			return {
				static_cast<unsigned char>(31 + frame * 11),
				static_cast<unsigned char>(239 - frame * 7),
				static_cast<unsigned char>(17 + frame * 13),
				255};
		}

		void queuedUpdates(Fixture& f)
		{
			// Upload once before the burst: texture updates can drain the GPU queue.
			auto* texture = solid(f, white);
			auto* buffer = f.buffer();
			int frame = 0;
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					const auto colour = queuedColour(frame++ % 16);
					f.fill(buffer, frame % 2 == 0 ? 4096u : 6u, target->getInfo(), {0, 0, 128, 128}, colour);
					target->doRender(buffer, texture, 6);
				});
			for (int burst : {8, 9, 16})
			{
				const int expectedFrame = frame + burst;
				for (int i = 1; i < burst; ++i)
					f.drawOneFrame();
				f.capture();
				require(frame == expectedFrame, "Capture must not inject additional rendered frames");
				f.expectCorners(queuedColour((frame - 1) % 16));
			}
		}

		void queuedRttUpdates(Fixture& f)
		{
			// Keep every frame's output observable without capturing between updates.
			std::array<MyGUI::ITexture*, 16> outputs{};
			for (auto& output : outputs)
				output = renderTexture(f, 8, 8);
			auto* texture = solid(f, white);
			auto* buffer = f.buffer();
			int frame = 0;
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					auto* output = outputs[size_t(frame)]->getRenderTarget();
					output->begin();
					f.fill(buffer, frame % 2 == 0 ? 4096u : 6u, output->getInfo(), {0, 0, 8, 8}, queuedColour(frame));
					output->doRender(buffer, texture, 6);
					output->end();
					// Sampling also schedules the producer on deferred scene-graph backends.
					f.quad(target, outputs[size_t(frame)]);
					++frame;
				});
			for (size_t i = 0; i < outputs.size(); ++i)
				f.drawOneFrame();
			require(frame == int(outputs.size()), "Each queued frame must render exactly once");
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					for (int i = 0; i < int(outputs.size()); ++i)
						f.quad(target, outputs[size_t(i)], white, {i % 4 * 32, i / 4 * 32, 32, 32});
				});
			f.capture();
			for (int i = 0; i < int(outputs.size()); ++i)
				f.expect(i % 4 * 32 + 16, i / 4 * 32 + 16, queuedColour(i));
		}

		void sameFrameVertexUpdates(Fixture& f)
		{
			auto* texture = solid(f, white);
			auto* buffer = f.buffer();
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					f.fill(buffer, 6, target->getInfo(), {0, 0, 64, 128}, red);
					target->doRender(buffer, texture, 6);
					f.fill(buffer, 6, target->getInfo(), {64, 0, 64, 128}, blue);
					target->doRender(buffer, texture, 6);
				});
			f.capture();
			f.expect(32, 64, red);
			f.expect(96, 64, blue);
		}

		void sameFrameTextureUpdates(Fixture& f)
		{
			auto* texture = solid(f, red);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					upload(texture, {0, 0, 255, 255});
					f.quad(target, texture, white, {0, 0, 64, 128});
					upload(texture, {255, 0, 0, 255});
					f.quad(target, texture, white, {64, 0, 64, 128});
				});
			f.capture();
			f.expect(32, 64, red);
			f.expect(96, 64, blue);
		}

		void sameFrameResourceDestruction(Fixture& f)
		{
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					auto* source = solid(f, red);
					auto release = [&](MyGUI::IVertexBuffer* buffer)
					{
						f.render().destroyVertexBuffer(buffer);
					};
					std::unique_ptr<MyGUI::IVertexBuffer, decltype(release)> buffer(
						f.render().createVertexBuffer(),
						release);
					f.fill(buffer.get(), 6, target->getInfo(), {0, 0, 128, 128}, white);
					target->doRender(buffer.get(), source, 6);
					f.removeTexture(source);
				});
			// Both wrappers disappear before the host submits/presents each frame.
			for (int frame = 0; frame < 8; ++frame)
			{
				f.capture();
				f.expectCorners(red);
			}
		}

		void sameFrameTextureRecreation(Fixture& f)
		{
			auto* source = solid(f, red);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					upload(source, {0, 0, 255, 255});
					f.quad(target, source, white, {0, 0, 64, 128});
					source->destroy();
					source->createManual(
						1,
						1,
						MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
						MyGUI::PixelFormat::R8G8B8A8);
					upload(source, {255, 0, 0, 255});
					f.quad(target, source, white, {64, 0, 64, 128});
				});
			f.capture();
			f.expect(32, 64, red);
			f.expect(96, 64, blue);
		}

		void sameFrameUpdateBurst(Fixture& f)
		{
			auto* texture = solid(f, white);
			auto* buffer = f.buffer();
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					// Exceed a small rotating buffer pool before any draw can retire.
					for (int i = 0; i < 16; ++i)
					{
						const auto colour = i % 2 == 0 ? red : blue;
						upload(texture, {colour[2], colour[1], colour[0], colour[3]});
						f.fill(buffer, i % 2 == 0 ? 6 : 4096, target->getInfo(), {i * 8, 0, 8, 128}, white);
						target->doRender(buffer, texture, 6);
					}
				});
			f.capture();
			for (int i = 0; i < 16; ++i)
				f.expect(i * 8 + 4, 64, i % 2 == 0 ? red : blue);
		}

		void manyDraws(Fixture& f)
		{
			auto* texture = solid(f, white);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					for (int y = 0; y < 8; ++y)
						for (int x = 0; x < 16; ++x)
							f.quad(target, texture, (x + y) % 2 == 0 ? red : blue, {x * 8, y * 16, 8, 16});
				});
			for (int frame = 0; frame < 4; ++frame)
			{
				f.capture();
				for (int y = 0; y < 8; ++y)
					for (int x = 0; x < 16; ++x)
						f.expect(x * 8 + 4, y * 16 + 8, (x + y) % 2 == 0 ? red : blue);
			}
		}

		template<bool offscreen>
		void shaderSelection(Fixture& f)
		{
			const auto files = shaderFiles();
			if (files.first.empty())
				throw Skip("This renderer has no programmable shader fixture");
			// Separate names avoid requiring shader replacement support between cases.
			const std::string name = offscreen ? "PlatformSwapChannelsRTT" : "PlatformSwapChannels";
			auto* output = offscreen ? renderTexture(f, 128, 128) : nullptr;
			f.render().registerShader(name, files.first, files.second);
			auto* custom = solid(f, red);
			auto* normal = solid(f, red);
			custom->setShader(name);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					auto* drawTarget = offscreen ? output->getRenderTarget() : target;
					if (offscreen)
						drawTarget->begin();
					f.quad(drawTarget, custom, white, {0, 0, 64, 128});
					f.quad(drawTarget, normal, white, {64, 0, 64, 128});
					if (offscreen)
					{
						drawTarget->end();
						f.quad(target, output);
					}
				});
			f.capture();
			f.expect(32, 64, blue);
			f.expect(96, 64, red);
			custom->setShader("Default");
			f.capture();
			f.expectCorners(red);
		}

		void sceneDepthPreservation(Fixture& f)
		{
			if (!f.setSceneDepthProbe(true))
				throw Skip("Scene depth capture is unavailable in this native harness");
			try
			{
				auto checkDepth = [&]
				{
					const auto& depth = f.getFrameCapture().depth;
					require(depth.size() == 5, "Depth probe must capture all interior samples");
					for (float sample : depth)
						require(
							std::isfinite(sample) && std::abs(sample - 0.25f) < 0.001f,
							"GUI must preserve the scene depth buffer");
				};
				f.capture();
				checkDepth();
				auto* texture = solid(f, white);
				show(f, texture);
				f.expectCorners(white);
				checkDepth();
			}
			catch (...)
			{
				f.setSceneDepthProbe(false);
				throw;
			}
			f.setSceneDepthProbe(false);
		}

		void emptySubmissionRemoval(Fixture& f)
		{
			auto* texture = solid(f, white);
			auto* buffer = f.buffer();
			size_t count = 6;
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					f.fill(buffer, 6, target->getInfo(), {0, 0, 128, 128}, red);
					target->doRender(buffer, texture, count);
				});
			f.capture();
			f.expectCorners(red);
			count = 0;
			f.capture();
			f.expectCorners(black);
			f.scene({});
			for (int i = 0; i < 3; ++i)
			{
				f.capture();
				f.expectCorners(black);
			}
		}

		void resize(Fixture& f)
		{
			auto* texture = solid(f, white);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					auto size = f.render().getViewSize();
					f.quad(target, texture, green, {0, 0, size.width, size.height});
				});
			for (const auto size : {MyGUI::IntSize(96, 80), MyGUI::IntSize(160, 112), MyGUI::IntSize(128, 128)})
			{
				f.resizeWindow(size.width, size.height);
				require(f.render().getViewSize() == size, "Resize must update logical view size");
				f.capture();
				f.expectCorners(green);
				auto& gui = MyGUI::Gui::getInstance();
				auto* parent = gui.createWidget<MyGUI::Widget>(
					"PlatformSolid",
					{size.width - 48, size.height - 48, 32, 32},
					MyGUI::Align::Default,
					"Platform");
				parent->setColour(MyGUI::Colour(1, 0, 0));
				auto* child =
					parent->createWidget<MyGUI::Widget>("PlatformSolid", {16, 0, 32, 32}, MyGUI::Align::Default);
				child->setColour(MyGUI::Colour(0, 0, 1));
				f.capture();
				f.expect(size.width - 40, size.height - 32, red);
				f.expect(size.width - 24, size.height - 32, blue);
				f.expect(size.width - 8, size.height - 32, green);
				gui.destroyWidget(parent);
			}
		}

		void hostState(Fixture& f)
		{
			if (!f.setHostileRenderState(true))
				throw Skip("Backend has no hostile render state hook");
			try
			{
				auto* texture = solid(f, white);
				show(f, texture);
				f.expectCorners(white);
			}
			catch (...)
			{
				f.setHostileRenderState(false);
				throw;
			}
			f.setHostileRenderState(false);
		}

		void rttPersistenceOrientation(Fixture& f)
		{
			auto* texture = solid(f, white);
			auto* output = renderTexture(f, 32, 16);
			bool update = true;
			bool changed = false;
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					if (update)
					{
						auto* rtt = output->getRenderTarget();
						rtt->begin();
						f.quad(rtt, texture, changed ? green : red, {0, 0, 32, 8});
						f.quad(rtt, texture, blue, {0, 8, 32, 8});
						rtt->end();
						update = false;
					}
					f.quad(target, output);
				});
			for (int i = 0; i < 3; ++i)
			{
				f.capture();
				f.expect(32, 24, red);
				f.expect(96, 104, blue);
			}
			update = true;
			changed = true;
			f.capture();
			f.expect(32, 24, green);
			f.expect(96, 104, blue);
		}

		void rttEmptyClear(Fixture& f)
		{
			auto* texture = solid(f, white);
			auto* output = renderTexture(f, 16, 8);
			bool empty = false;
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					auto* rtt = output->getRenderTarget();
					rtt->begin();
					if (!empty)
						f.quad(rtt, texture, red, {0, 0, 16, 8});
					rtt->end();
					f.quad(target, texture, green);
					f.quad(target, output);
				});
			f.capture();
			f.expectCorners(red);
			empty = true;
			f.capture();
			f.expectCorners(green);
		}

		void rttChain(Fixture& f)
		{
			auto* source = solid(f, red);
			auto* first = renderTexture(f, 32, 16);
			auto* second = renderTexture(f, 16, 32);
			// Exercise both creation orders and change dependencies on live targets.
			for (bool reverse : {false, true})
			{
				auto* a = reverse ? second : first;
				auto* b = reverse ? first : second;
				upload(source, {0, 0, 255, 255});
				f.scene(
					[&, a, b](MyGUI::IRenderTarget* target)
					{
						redraw(f, a, source);
						redraw(f, b, a);
						f.quad(target, b);
					});
				f.capture();
				f.expectCorners(red);
				upload(source, {0, 255, 0, 255});
				f.capture();
				f.expectCorners(green);
			}
		}

		void rttReadback(Fixture& f)
		{
			const auto usage = MyGUI::TextureUsage::RenderTarget | MyGUI::TextureUsage::Read;
			requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, usage);
			auto* output = f.texture();
			output->createManual(3, 2, usage, MyGUI::PixelFormat::R8G8B8A8);
			auto* source = solid(f, white);
			Pixel colour = red;
			f.scene(
				[&](MyGUI::IRenderTarget*)
				{
					auto* rtt = output->getRenderTarget();
					rtt->begin();
					f.quad(rtt, source, colour, {0, 0, 3, 1});
					f.quad(rtt, source, green, {0, 1, 3, 1});
					rtt->end();
				});
			for (auto value : {red, blue})
			{
				colour = value;
				// Verify GPU rendering independently of window sampling and screenshot capture.
				f.drawOneFrame();
				std::vector<unsigned char> expected;
				for (auto row : {colour, green})
					for (int x = 0; x < 3; ++x)
						expected.insert(expected.end(), {row[2], row[1], row[0], row[3]});
				require(read(output) == expected, "RTT readback must return current GPU-rendered rows");
			}
		}

		void rttReadWrite(Fixture& f)
		{
			const auto usage =
				MyGUI::TextureUsage::RenderTarget | MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write;
			requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, usage);
			auto* output = f.texture();
			output->createManual(3, 2, usage, MyGUI::PixelFormat::R8G8B8A8);
			require(output->getRenderTarget() != nullptr, "Combined access flags must create a render target");
			upload(output, std::vector<unsigned char>(24, 0));
			auto* source = solid(f, green);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					redraw(f, output, source);
					f.quad(target, output);
				});
			f.capture();
			f.expectCorners(green);
			f.scene([&](MyGUI::IRenderTarget* target) { f.quad(target, output); });
			std::vector<unsigned char> expected;
			for (int i = 0; i < 6; ++i)
				expected.insert(expected.end(), {0, 255, 0, 255});
			require(read(output) == expected, "Read-only RTT locks must return current GPU pixels");
			auto* bytes =
				static_cast<unsigned char*>(output->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
			require(bytes != nullptr, "RTT read/write lock must succeed");
			const bool current = std::equal(expected.begin(), expected.end(), bytes);
			bytes[0] = 255;
			expected[0] = 255;
			output->unlock();
			require(current && read(output) == expected, "RTT read/write must preserve GPU-produced pixels");
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					f.quad(target, output, white, {0, 0, 64, 128}, {1.0f / 6, 0.25f, 1.0f / 6, 0.25f});
					f.quad(target, output, white, {64, 0, 64, 128}, {0.5f, 0.75f, 0.5f, 0.75f});
				});
			f.capture();
			f.expect(32, 64, {0, 255, 255, 255});
			f.expect(96, 64, green);
			// Render again after a CPU edit: staging must not retain stale GPU contents.
			upload(source, {255, 0, 0, 255});
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					redraw(f, output, source);
					f.quad(target, output);
				});
			f.capture();
			f.expectCorners(blue);
			for (size_t i = 0; i < expected.size(); i += 4)
			{
				expected[i] = 255;
				expected[i + 1] = 0;
				expected[i + 2] = 0;
			}
			require(read(output) == expected, "RTT readback must refresh staging after subsequent rendering");
		}

		void rttCachedTargetAfterUpload(Fixture& f)
		{
			const auto usage = MyGUI::TextureUsage::RenderTarget | MyGUI::TextureUsage::Write;
			requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, usage);
			auto* source = f.texture();
			source->createManual(1, 1, usage, MyGUI::PixelFormat::R8G8B8A8);
			auto* cachedTarget = source->getRenderTarget();
			require(cachedTarget != nullptr, "Combined flags must supply a render target");
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					upload(source, {0, 255, 0, 255});
					f.quad(target, source, white, {0, 0, 64, 128});
					upload(source, {255, 255, 0, 255});
					require(source->getRenderTarget() == cachedTarget, "Upload must preserve cached target pointers");
					f.quad(target, source, white, {64, 0, 64, 128});
				});
			f.capture();
			f.expect(32, 64, green);
			f.expect(96, 64, {0, 255, 255, 255});
			auto* whiteTexture = solid(f, white);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					cachedTarget->begin();
					f.quad(cachedTarget, whiteTexture, red, {0, 0, 1, 1});
					cachedTarget->end();
					f.quad(target, source);
				});
			f.capture();
			f.expectCorners(red);
		}

		void rttNested(Fixture& f)
		{
			auto* source = solid(f, white);
			auto* a = renderTexture(f, 32, 16);
			auto* b = renderTexture(f, 16, 32);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					auto* outer = a->getRenderTarget();
					auto* inner = b->getRenderTarget();
					outer->begin();
					f.quad(outer, source, red, {0, 0, 32, 16});
					inner->begin();
					f.quad(inner, source, blue, {0, 0, 16, 32});
					inner->end();
					f.quad(outer, source, green, {16, 0, 16, 16});
					outer->end();
					f.quad(target, a);
				});
			f.capture();
			f.expect(8, 64, red);
			f.expect(119, 64, green);
			show(f, b);
			f.expectCorners(blue);
		}

		void rttStateRestoration(Fixture& f)
		{
			auto* texture = solid(f, white);
			auto* a = renderTexture(f, 16, 32);
			auto* b = renderTexture(f, 32, 8);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					f.quad(target, texture, red);
					redraw(f, a, texture);
					redraw(f, b, texture);
					f.quad(target, texture, green, {64, 0, 64, 128});
				});
			f.capture();
			f.expect(8, 8, red);
			f.expect(8, 119, red);
			f.expect(119, 8, green);
			f.expect(119, 119, green);
		}

		void rttDestroyBeforeFrame(Fixture& f)
		{
			auto* source = solid(f, red);
			auto* output = renderTexture(f, 32, 16);
			// No draw/capture may flush a deferred target before destruction.
			output->destroy();
			output->createManual(16, 32, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
			require(
				output->getWidth() == 16 && output->getHeight() == 32,
				"Recreated target must report its new dimensions");
			require(output->getRenderTarget() != nullptr, "Recreated texture must supply a render target");
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					redraw(f, output, source);
					f.quad(target, output);
				});
			f.capture();
			f.expectCorners(red);
		}

		void rttDestruction(Fixture& f)
		{
			auto* source = solid(f, red);
			auto* output = renderTexture(f, 32, 16);
			f.scene(
				[&](MyGUI::IRenderTarget* target)
				{
					redraw(f, output, source);
					f.quad(target, output);
				});
			f.capture();
			f.expectCorners(red);
			f.scene({});
			f.capture();
			f.removeTexture(output);
			show(f, source);
			f.expectCorners(red);
		}

		void freshFixture(Fixture& f)
		{
			auto* texture = solid(f, green);
			show(f, texture);
			f.expectCorners(green);
		}

		void replacementFixture(Fixture& f)
		{
			auto* texture = solid(f, red);
			show(f, texture);
			f.expectCorners(red);
		}

		template<MyGUI::TextureUsage::Enum hint>
		void addTextureCases(std::vector<PlatformTestCase>& result, const std::string& suffix)
		{
			result.insert(
				result.end(),
				{
					{"resources", "format-rgba" + suffix, pattern<MyGUI::PixelFormat::R8G8B8A8, hint>},
					{"resources", "format-rgb" + suffix, pattern<MyGUI::PixelFormat::R8G8B8, hint>},
					{"resources", "format-l8" + suffix, pattern<MyGUI::PixelFormat::L8, hint>},
					{"resources", "format-l8a8" + suffix, pattern<MyGUI::PixelFormat::L8A8, hint>},
					{"resources", "rgba-read-write" + suffix, readWritePattern<MyGUI::PixelFormat::R8G8B8A8, hint>},
					{"resources", "rgb-read-write" + suffix, readWritePattern<MyGUI::PixelFormat::R8G8B8, hint>},
					{"resources", "l8-read-write" + suffix, readWritePattern<MyGUI::PixelFormat::L8, hint>},
					{"resources", "l8a8-read-write" + suffix, readWritePattern<MyGUI::PixelFormat::L8A8, hint>},
				});
		}

	}

	std::vector<PlatformTestCase> cases()
	{
		std::vector<PlatformTestCase> result{
			{"resources", "resource-stream", resourceStream},
			{"resources", "texture-lifecycle", textureLifecycle},
			{"resources", "duplicate-texture-name", duplicateTextureName},
			{"resources", "unknown-format", unknownFormat},
			{"resources", "interleaved-texture-locks", interleavedTextureLocks},
			{"resources", "reload-render-target-from-file", reloadRenderTargetFromFile},
			{"resources", "shared-file-texture-lifetime", sharedFileTextureLifetime},
			{"resources", "loaded-texture-write", loadedTextureWrite},
			{"resources", "png-fidelity", pngFidelity},
			{"resources", "png-readable-bytes", pngReadableBytes},
			{"resources", "png-roundtrip", pngRoundtrip},
			{"resources", "filter-loaded", filterLoaded},
			{"resources", "filter-uploaded", filterUploaded},
			{"resources", "mask-picking", maskPicking},

			{"rendering", "capture-current-frame", captureCurrentFrame},
			{"rendering", "tint-overlap-blend", tintOverlapBlend},
			{"rendering", "widgets-clipping-removal", widgets},
			{"rendering", "stream-grow-shrink", streamGrowShrink},
			{"rendering", "queued-updates", queuedUpdates},
			{"rendering", "queued-rtt-updates", queuedRttUpdates},
			{"rendering", "same-frame-vertex-updates", sameFrameVertexUpdates},
			{"rendering", "same-frame-texture-updates", sameFrameTextureUpdates},
			{"rendering", "same-frame-resource-destruction", sameFrameResourceDestruction},
			{"rendering", "same-frame-texture-recreation", sameFrameTextureRecreation},
			{"rendering", "same-frame-update-burst", sameFrameUpdateBurst},
			{"rendering", "many-draws", manyDraws},
			{"rendering", "shader-selection", shaderSelection<false>},
			{"rendering", "rtt-shader-selection", shaderSelection<true>},
			{"rendering", "scene-depth-preservation", sceneDepthPreservation},
			{"rendering", "empty-submission-removal", emptySubmissionRemoval},
			{"rendering", "resize", resize},
			{"rendering", "host-state", hostState},
			{"rendering", "rtt-persistence-orientation", rttPersistenceOrientation},
			{"rendering", "rtt-empty-clear", rttEmptyClear},
			{"rendering", "rtt-chain", rttChain},
			{"rendering", "rtt-readback", rttReadback},
			{"rendering", "rtt-read-write", rttReadWrite},
			{"rendering", "rtt-cached-target-after-upload", rttCachedTargetAfterUpload},
			{"rendering", "rtt-nested", rttNested},
			{"rendering", "rtt-state-restoration", rttStateRestoration},
			{"rendering", "rtt-destroy-before-frame", rttDestroyBeforeFrame},
			{"rendering", "rtt-destruction", rttDestruction},

			{"lifecycle", "fresh-fixture", freshFixture},
			{"lifecycle", "replacement-fixture", replacementFixture}};

		addTextureCases<MyGUI::TextureUsage::Static>(result, "");
		addTextureCases<MyGUI::TextureUsage::Dynamic>(result, "-dynamic");
		addTextureCases<MyGUI::TextureUsage::Stream>(result, "-stream");
		return result;
	}

}

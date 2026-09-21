#include "PlatformFixture.h"
#include "MyGUI_DataStream.h"
#include "MyGUI_MaskPickInfo.h"
#include "MyGUI_FileSystemUtility.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstring>
#include <fstream>

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

		void pattern(Fixture& f, MyGUI::PixelFormat format)
		{
			const auto usage = MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write;
			requireSupport(f, format, usage);
			auto* texture = f.texture();
			texture->createManual(3, 2, usage, format);
			require(texture->getWidth() == 3 && texture->getHeight() == 2, "Odd-width texture dimensions");
			require(
				texture->getFormat() == format && texture->getNumElemBytes() == size_t(format.getBytesPerPixel()),
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
			upload(texture, bytes);
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
			f.capture();
			for (int y = 0; y < 2; ++y)
				for (int x = 0; x < 3; ++x)
					f.expect(x * 40 + 20, y * 64 + 32, expected[size_t(y * 3 + x)]);
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

	}

	std::vector<Case> cases()
	{
		return {
			{"resources",
			 "resource-stream",
			 [](Fixture&)
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
			 }},
			{"resources",
			 "texture-lifecycle",
			 [](Fixture& f)
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
			 }},
			{"resources",
			 "duplicate-texture-name",
			 [](Fixture& f)
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
			 }},
			{"resources",
			 "unknown-format",
			 [](Fixture& f)
			 {
				 for (auto usage :
					  {MyGUI::TextureUsage::Default,
					   MyGUI::TextureUsage::Write,
					   MyGUI::TextureUsage::Read,
					   MyGUI::TextureUsage::RenderTarget})
					 require(
						 !f.render().isFormatSupported(MyGUI::PixelFormat::Unknow, usage),
						 "Unknown pixel format must not be advertised");
			 }},
			{"resources", "format-rgba", [](Fixture& f) { pattern(f, MyGUI::PixelFormat::R8G8B8A8); }},
			{"resources", "format-rgb", [](Fixture& f) { pattern(f, MyGUI::PixelFormat::R8G8B8); }},
			{"resources", "format-l8", [](Fixture& f) { pattern(f, MyGUI::PixelFormat::L8); }},
			{"resources", "format-l8a8", [](Fixture& f) { pattern(f, MyGUI::PixelFormat::L8A8); }},
			{"resources",
			 "read-write-preservation",
			 [](Fixture& f)
			 {
				 const auto usage =
					 MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write;
				 requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, usage);
				 auto* texture = f.texture();
				 texture->createManual(4, 2, usage, MyGUI::PixelFormat::R8G8B8A8);
				 auto expected = pngBytes();
				 upload(texture, expected);
				 require(read(texture) == expected, "Read lock must reproduce written bytes exactly");
				 auto* bytes =
					 static_cast<unsigned char*>(texture->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
				 require(bytes != nullptr, "Read/write lock must succeed");
				 const bool preserved = std::equal(expected.begin(), expected.end(), bytes);
				 bytes[8] = 13;
				 expected[8] = 13;
				 texture->unlock();
				 require(
					 preserved && read(texture) == expected,
					 "Partial read/write update must preserve untouched bytes");
			 }},
			{"resources",
			 "rgb-read-write",
			 [](Fixture& f)
			 {
				 const auto usage =
					 MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write;
				 requireSupport(f, MyGUI::PixelFormat::R8G8B8, usage);
				 for (int width : {1, 3, 5})
				 {
					 auto* texture = f.texture();
					 texture->createManual(width, 3, usage, MyGUI::PixelFormat::R8G8B8);
					 std::vector<unsigned char> expected(size_t(width) * 3 * 3);
					 for (size_t i = 0; i < expected.size(); ++i)
						 expected[i] = static_cast<unsigned char>(17 + i * 7);
					 upload(texture, expected);
					 require(read(texture) == expected, "Odd-width RGB readback must be tightly packed");
					 auto* bytes = static_cast<unsigned char*>(
						 texture->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
					 require(bytes != nullptr, "RGB read/write lock must succeed");
					 const bool preserved = std::equal(expected.begin(), expected.end(), bytes);
					 bytes[expected.size() - 1] = 93;
					 expected.back() = 93;
					 texture->unlock();
					 require(
						 preserved && read(texture) == expected,
						 "RGB partial update must preserve every other byte");
				 }
			 }},
			{"resources",
			 "loaded-texture-write",
			 [](Fixture& f)
			 {
				 auto* texture = f.texture();
				 texture->loadFromFile("TransparentRgb.png");
				 requireSupport(f, texture->getFormat(), texture->getUsage() | MyGUI::TextureUsage::Write);
				 upload(texture, std::vector<unsigned char>(32, 255));
				 show(f, texture);
				 f.expectCorners(white);
			 }},
			{"resources",
			 "png-fidelity",
			 [](Fixture& f)
			 {
				 auto* texture = f.texture();
				 texture->loadFromFile("TransparentRgb.png");
				 verifyPng(f, texture);
			 }},
			{"resources",
			 "png-readable-bytes",
			 [](Fixture& f)
			 {
				 requireSupport(
					 f,
					 MyGUI::PixelFormat::R8G8B8A8,
					 MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read);
				 auto* texture = f.texture();
				 texture->loadFromFile("TransparentRgb.png");
				 require(read(texture) == pngBytes(), "PNG must preserve hidden RGB and top-down BGR(A) bytes");
			 }},
			{"resources",
			 "png-roundtrip",
			 [](Fixture& f)
			 {
				 struct Temporary
				 {
					 std::filesystem::path path = std::filesystem::temp_directory_path() /
						 ("mygui-platform-" +
						  std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
					 Temporary()
					 {
						 std::filesystem::create_directory(path);
					 }
					 ~Temporary()
					 {
						 std::error_code error;
						 std::filesystem::remove_all(path, error);
					 }
				 } temporary;
				 auto* source = f.texture();
				 source->loadFromFile("TransparentRgb.png");
				 source->saveToFile(MyGUI::utility::toUtf8(temporary.path / "RoundTrip.PNG"));
				 require(
					 std::filesystem::exists(temporary.path / "RoundTrip.PNG"),
					 "PNG saving must create the requested file");
				 verifyPng(f, source);
				 f.addResourceLocation(temporary.path);
				 auto* reloaded = f.texture();
				 reloaded->loadFromFile("RoundTrip.PNG");
				 verifyPng(f, reloaded);
				 if (f.render().isFormatSupported(
						 MyGUI::PixelFormat::R8G8B8A8,
						 MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read))
					 require(
						 read(source) == pngBytes() && read(reloaded) == pngBytes(),
						 "Roundtrip must independently preserve source and decoded bytes");
			 }},
			{"resources",
			 "filter-loaded",
			 [](Fixture& f)
			 {
				 auto* texture = f.texture();
				 texture->loadFromFile("TransparentRgb.png");
				 f.scene([&f, texture](MyGUI::IRenderTarget* target)
						 { f.quad(target, texture, white, {0, 0, 128, 128}, {0.25f, 0.25f, 0.25f, 0.25f}); });
				 f.capture();
				 f.expectCorners({64, 64, 64, 255});
			 }},
			{"resources",
			 "filter-uploaded",
			 [](Fixture& f)
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
			 }},
			{"resources",
			 "mask-picking",
			 [](Fixture& f)
			 {
				 requireSupport(
					 f,
					 MyGUI::PixelFormat::R8G8B8A8,
					 MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read);
				 MyGUI::MaskPickInfo mask;
				 require(mask.load("Mask.png") && !mask.empty(), "Readable PNG mask must load");
				 require(
					 mask.pick({16, 16}, {0, 0, 64, 64}) && mask.pick({48, 48}, {0, 0, 64, 64}),
					 "White mask coordinates must be accepted");
				 require(
					 !mask.pick({48, 16}, {0, 0, 64, 64}) && !mask.pick({16, 48}, {0, 0, 64, 64}),
					 "Nonwhite mask coordinates must be rejected");
			 }},
			{"rendering",
			 "capture-current-frame",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "tint-overlap-blend",
			 [](Fixture& f)
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
			 }},
			{"rendering", "widgets-clipping-removal", widgets},
			{"rendering",
			 "stream-grow-shrink",
			 [](Fixture& f)
			 {
				 auto* texture = solid(f, white);
				 auto* buffer = f.buffer();
				 size_t capacity = 6;
				 Pixel colour = red;
				 f.scene(
					 [&](MyGUI::IRenderTarget* target)
					 {
						 f.fill(buffer, capacity, target->getInfo(), {0, 0, 128, 128}, colour);
						 target->doRender(buffer, texture, capacity / 6 * 6);
					 });
				 for (int repeat = 0; repeat < 3; ++repeat)
					 for (size_t count : {4096u, 6u, 2048u, 6u})
					 {
						 capacity = count;
						 colour = colour == red ? green : red;
						 f.capture();
						 f.expectCorners(colour);
					 }
			 }},
			{"rendering",
			 "queued-updates",
			 [](Fixture& f)
			 {
				 auto* texture = solid(f, white);
				 auto* buffer = f.buffer();
				 int frame = 0;
				 f.scene(
					 [&](MyGUI::IRenderTarget* target)
					 {
						 const bool alternate = (++frame % 2) == 0;
						 upload(
							 texture,
							 alternate ? std::vector<unsigned char>{0, 255, 0, 255}
									   : std::vector<unsigned char>{0, 0, 255, 255});
						 f.fill(buffer, alternate ? 4096u : 6u, target->getInfo(), {0, 0, 128, 128}, white);
						 target->doRender(buffer, texture, 6);
					 });
				 for (int i = 0; i < 15; ++i)
					 f.drawOneFrame();
				 f.capture();
				 require(frame == 16, "Capture must not inject additional rendered frames");
				 f.expectCorners(green);
			 }},
			{"rendering",
			 "same-frame-vertex-updates",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "same-frame-texture-updates",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "shader-selection",
			 [](Fixture& f)
			 {
				 const auto files = shaderFiles();
				 if (files.first.empty())
					 throw Skip("This renderer has no programmable shader fixture");
				 f.render().registerShader("PlatformSwapChannels", files.first, files.second);
				 auto* custom = solid(f, red);
				 auto* normal = solid(f, red);
				 custom->setShader("PlatformSwapChannels");
				 f.scene(
					 [&](MyGUI::IRenderTarget* target)
					 {
						 f.quad(target, custom, white, {0, 0, 64, 128});
						 f.quad(target, normal, white, {64, 0, 64, 128});
					 });
				 f.capture();
				 f.expect(32, 64, blue);
				 f.expect(96, 64, red);
				 custom->setShader("Default");
				 f.capture();
				 f.expectCorners(red);
			 }},
			{"rendering",
			 "scene-depth-preservation",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "empty-submission-removal",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "resize",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "host-state",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "rtt-persistence-orientation",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "rtt-empty-clear",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "rtt-source-update",
			 [](Fixture& f)
			 {
				 auto* source = solid(f, red);
				 auto* output = renderTexture(f, 16, 32);
				 f.scene(
					 [&](MyGUI::IRenderTarget* target)
					 {
						 redraw(f, output, source);
						 f.quad(target, output);
					 });
				 f.capture();
				 f.expectCorners(red);
				 upload(source, {255, 0, 0, 255});
				 f.capture();
				 f.expectCorners(blue);
			 }},
			{"rendering",
			 "rtt-chain",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "rtt-read-write",
			 [](Fixture& f)
			 {
				 const auto usage =
					 MyGUI::TextureUsage::RenderTarget | MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write;
				 requireSupport(f, MyGUI::PixelFormat::R8G8B8A8, usage);
				 auto* output = f.texture();
				 output->createManual(2, 2, usage, MyGUI::PixelFormat::R8G8B8A8);
				 upload(output, std::vector<unsigned char>(16, 0));
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
				 for (int i = 0; i < 4; ++i)
					 expected.insert(expected.end(), {0, 255, 0, 255});
				 auto* bytes =
					 static_cast<unsigned char*>(output->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
				 require(bytes != nullptr, "RTT read/write lock must succeed");
				 const bool current = std::equal(expected.begin(), expected.end(), bytes);
				 bytes[0] = 255;
				 expected[0] = 255;
				 output->unlock();
				 require(current && read(output) == expected, "RTT read/write must preserve GPU-produced pixels");
			 }},
			{"rendering",
			 "rtt-nested",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "rtt-state-restoration",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "rtt-destroy-before-frame",
			 [](Fixture& f)
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
			 }},
			{"rendering",
			 "rtt-destruction",
			 [](Fixture& f)
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
			 }},
			{"lifecycle",
			 "fresh-fixture",
			 [](Fixture& f)
			 {
				 auto* texture = solid(f, green);
				 show(f, texture);
				 f.expectCorners(green);
			 }},
			{"lifecycle",
			 "replacement-fixture",
			 [](Fixture& f)
			 {
				 auto* texture = solid(f, red);
				 show(f, texture);
				 f.expectCorners(red);
			 }}};
	}

}

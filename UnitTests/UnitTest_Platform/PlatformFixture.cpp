#include "PlatformFixture.h"
#include "MyGUI_VertexData.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	#include <objbase.h>
#endif

namespace platformtest
{

	int runNativeTests(std::initializer_list<PlatformTestCase> _tests, void (*_initialise)(Fixture&))
	{
		try
		{
			Fixture fixture(false);
			fixture.caseName = "context-initialization";
			fixture.open();
			fixture.capture();
			if (_initialise)
				_initialise(fixture);
			std::vector<unittest::TestCase> tests;
			for (const auto& test : _tests)
				tests.push_back(
					{test.name,
					 [&, test]
					 {
						 fixture.caseName = test.name;
						 try
						 {
							 test.run(fixture);
							 fixture.resetCase();
						 }
						 catch (...)
						 {
							 const auto path = std::filesystem::path("platform-artifacts") /
								 ("native-" + std::string(backendName())) / (test.name + ".ppm");
							 try
							 {
								 fixture.saveFailure(path);
							 }
							 catch (const std::exception& error)
							 {
								 std::cerr << "Cannot save failure image: " << error.what() << '\n';
							 }
							 throw;
						 }
					 }});
			// Native state probes may leave the context unusable after an assertion.
			const int result = unittest::runTests(tests, unittest::FailurePolicy::Stop);
			fixture.close();
			return result;
		}
		catch (const std::exception& error)
		{
			std::cerr << "FATAL backend " << backendName() << ": " << error.what() << '\n';
			return 1;
		}
	}

	void CallbackLayer::renderToTarget(MyGUI::IRenderTarget* _target, bool _update)
	{
		if (callback)
			callback(_target);
		Base::renderToTarget(_target, _update);
	}

	Fixture::Fixture(bool _visible) :
		mVisible(_visible)
	{
		setWindowOptions(!_visible, true);
		setResourceFilename("");
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		// DirectX WIC texture loading requires COM on the thread running the fixture.
		const HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		require(SUCCEEDED(result) || result == RPC_E_CHANGED_MODE, "Failed to initialize COM for platform tests");
		mComInitialized = SUCCEEDED(result);
#endif
	}

	Fixture::~Fixture()
	{
		try
		{
			close();
		}
		catch (const std::exception& error)
		{
			std::cerr << "Fixture cleanup failed: " << error.what() << '\n';
		}
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		if (mComInitialized)
			CoUninitialize();
#endif
	}

	void Fixture::open()
	{
		if (!create(128, 128))
			throw Fatal("Failed to create graphics fixture");
		mCreated = true;
		require(getDrawableSize() == MyGUI::IntSize(128, 128), "Fixture must have a 128x128 drawable");
		require(render().getViewSize() == MyGUI::IntSize(128, 128), "Fixture must have an unscaled GUI view");
		require(MyGUI::Gui::getInstance().getDpiScale() == 1.0f, "Fixture must disable DPI scaling");
		require(mVisible || isWindowHidden(), "Graphics initialization must keep the requested window hidden");
	}

	void Fixture::close()
	{
		if (mCreated)
		{
			mCreated = false;
			destroy();
		}
	}

	void Fixture::setupResources()
	{
		BaseManager::setupResources();
		addResourceLocation(unittest::getResourcePath("UnitTest_Platform"));
	}

	void Fixture::createScene()
	{
		unittest::loadResources("UnitTest_Platform/Skins.xml");
		MyGUI::FactoryManager::getInstance().registerFactory<CallbackLayer>(
			MyGUI::LayerManager::getInstance().getCategoryName());
		mLayer = static_cast<CallbackLayer*>(
			MyGUI::LayerManager::getInstance().createLayerAt("Platform", "CallbackLayer", 0));
	}

	void Fixture::destroyScene()
	{
		if (mLayer)
			mLayer->callback = {};
		releaseResources();
		mLayer = nullptr;
	}

	void Fixture::releaseResources()
	{
		for (auto* value : mBuffers)
			render().destroyVertexBuffer(value);
		mBuffers.clear();
		mDrawBuffers.clear();
		for (auto iter = mTextures.rbegin(); iter != mTextures.rend(); ++iter)
			render().destroyTexture(*iter);
		mTextures.clear();
	}

	void Fixture::resetCase()
	{
		scene({});
		MyGUI::Gui::getInstance().destroyAllChildWidget();
		if (getDrawableSize() != MyGUI::IntSize(128, 128))
			resizeWindow(128, 128);
		// Retire normal consumers before deleting resources. Only this cleanup frame
		// synchronizes; queued-update tests themselves run uncaptured frames.
		capture();
		releaseResources();
	}

	void Fixture::scene(std::function<void(MyGUI::IRenderTarget*)> _callback)
	{
		if (!mLayer)
			throw Fatal("Fixture scene is not initialized");
		mLayer->callback = [this, callback = std::move(_callback)](MyGUI::IRenderTarget* target)
		{
			mDrawIndex = 0;
			if (callback)
				callback(target);
		};
	}

	const Capture& Fixture::capture()
	{
		if (getDrawableSize().width <= 0 || getDrawableSize().height <= 0)
			throw Fatal("Cannot capture an uninitialized window");
		requestFrameCapture();
		try
		{
			for (int frame = 0; frame < 8 && getFrameCapture().status == Capture::Status::Pending; ++frame)
				drawOneFrame();
		}
		catch (const std::exception& error)
		{
			failFrameCapture(error.what());
		}
		const auto& result = getFrameCapture();
		if (result.status != Capture::Status::Complete)
		{
			if (result.status == Capture::Status::Failed && !result.fatal)
				throw std::runtime_error("Graphics error: " + result.error);
			throw Fatal("Frame capture failed: " + result.error);
		}
		require(
			MyGUI::IntSize(result.width, result.height) == getDrawableSize(),
			"Capture must use current drawable dimensions");
		require(
			result.rgba.size() == size_t(result.width) * size_t(result.height) * 4,
			"Capture must be tightly packed RGBA8");
		return result;
	}

	void Fixture::expect(int _x, int _y, Pixel _expected) const
	{
		const auto& image = getFrameCapture();
		require(_x >= 0 && _y >= 0 && _x < image.width && _y < image.height, "Pixel assertion outside captured image");
		const auto* actual = image.rgba.data() + (size_t(_y) * size_t(image.width) + size_t(_x)) * 4;
		for (size_t channel = 0; channel < 3; ++channel)
			if (std::abs(int(actual[channel]) - int(_expected[channel])) > 1)
				throw std::runtime_error(
					"backend " + std::string(backendName()) + " / " + caseName + " pixel (" + std::to_string(_x) + "," +
					std::to_string(_y) + ") channel " + std::to_string(channel) + ": expected " +
					std::to_string(_expected[channel]) + ", actual " + std::to_string(actual[channel]));
	}

	void Fixture::expectCorners(Pixel _expected) const
	{
		const auto& image = getFrameCapture();
		for (int y : {8, image.height - 9})
			for (int x : {8, image.width - 9})
				expect(x, y, _expected);
		expect(image.width / 2, image.height / 2, _expected);
	}

	void Fixture::saveFailure(const std::filesystem::path& _file) const
	{
		const auto& image = getFrameCapture();
		if (image.rgba.empty())
			return;
		std::filesystem::create_directories(_file.parent_path());
		std::ofstream stream(_file, std::ios::binary);
		stream << "P6\n" << image.width << " " << image.height << "\n255\n";
		for (size_t offset = 0; offset < image.rgba.size(); offset += 4)
			stream.write(reinterpret_cast<const char*>(image.rgba.data() + offset), 3);
	}

	MyGUI::RenderManager& Fixture::render() const
	{
		return MyGUI::RenderManager::getInstance();
	}

	MyGUI::ITexture* Fixture::texture(const std::string& _name)
	{
		auto* value = render().createTexture(_name.empty() ? "PlatformTexture" + std::to_string(++mNameIndex) : _name);
		mTextures.push_back(value);
		return value;
	}

	void Fixture::removeTexture(MyGUI::ITexture* _texture)
	{
		mTextures.erase(std::remove(mTextures.begin(), mTextures.end(), _texture), mTextures.end());
		render().destroyTexture(_texture);
	}

	MyGUI::IVertexBuffer* Fixture::buffer()
	{
		auto* value = render().createVertexBuffer();
		mBuffers.push_back(value);
		return value;
	}

	void Fixture::fill(
		MyGUI::IVertexBuffer* _buffer,
		size_t _capacity,
		const MyGUI::RenderTargetInfo& _info,
		MyGUI::IntCoord _rect,
		Pixel _colour,
		MyGUI::FloatRect _uv)
	{
		const auto quad = quadVertices(_info, _rect, _colour, _uv);
		_buffer->setVertexCount(_capacity);
		require(_buffer->getVertexCount() == _capacity, "Vertex buffer must report requested count");
		auto* vertices = _buffer->lock();
		require(vertices != nullptr, "Vertex write lock must succeed");
		for (size_t index = 0; index < _capacity; ++index)
			vertices[index] = quad[index % quad.size()];
		_buffer->unlock();
	}

	std::array<MyGUI::Vertex, 6> Fixture::quadVertices(
		const MyGUI::RenderTargetInfo& _info,
		MyGUI::IntCoord _rect,
		Pixel _colour,
		MyGUI::FloatRect _uv) const
	{
		const float left = ((_info.pixScaleX * float(_rect.left) + _info.hOffset) * 2) - 1;
		const float top = 1 - ((_info.pixScaleY * float(_rect.top) + _info.vOffset) * 2);
		const float right = left + _info.pixScaleX * float(_rect.width) * 2;
		const float bottom = top - _info.pixScaleY * float(_rect.height) * 2;
		MyGUI::uint32 colour = (MyGUI::uint32(_colour[3]) << 24) | (MyGUI::uint32(_colour[1]) << 8);
		if (render().getVertexFormat() == MyGUI::VertexColourType::ColourABGR)
			colour |= (MyGUI::uint32(_colour[2]) << 16) | _colour[0];
		else
			colour |= (MyGUI::uint32(_colour[0]) << 16) | _colour[2];
		return {
			{{left, top, _info.maximumDepth, colour, _uv.left, _uv.top},
			 {right, top, _info.maximumDepth, colour, _uv.right, _uv.top},
			 {left, bottom, _info.maximumDepth, colour, _uv.left, _uv.bottom},
			 {left, bottom, _info.maximumDepth, colour, _uv.left, _uv.bottom},
			 {right, top, _info.maximumDepth, colour, _uv.right, _uv.top},
			 {right, bottom, _info.maximumDepth, colour, _uv.right, _uv.bottom}}};
	}

	void Fixture::quad(
		MyGUI::IRenderTarget* _target,
		MyGUI::ITexture* _texture,
		Pixel _colour,
		MyGUI::IntCoord _rect,
		MyGUI::FloatRect _uv)
	{
		if (mDrawIndex == mDrawBuffers.size())
			mDrawBuffers.push_back(buffer());
		auto* vertices = mDrawBuffers[mDrawIndex++];
		fill(vertices, 6, _target->getInfo(), _rect, _colour, _uv);
		_target->doRender(vertices, _texture, 6);
	}

	void upload(MyGUI::ITexture* _texture, const std::vector<unsigned char>& _bytes)
	{
		require(
			_bytes.size() == size_t(_texture->getWidth()) * size_t(_texture->getHeight()) * _texture->getNumElemBytes(),
			"Upload size mismatch");
		require(!_texture->isLocked(), "New upload must start unlocked");
		void* data = _texture->lock(MyGUI::TextureUsage::Write);
		require(data != nullptr, "Texture write lock must succeed");
		const bool locked = _texture->isLocked();
		std::memcpy(data, _bytes.data(), _bytes.size());
		_texture->unlock();
		require(locked && !_texture->isLocked(), "Write lock state transitions must be observable");
	}

	std::vector<unsigned char> read(MyGUI::ITexture* _texture)
	{
		require(!_texture->isLocked(), "New read must start unlocked");
		const auto* data = static_cast<const unsigned char*>(_texture->lock(MyGUI::TextureUsage::Read));
		require(data != nullptr, "Advertised texture read lock must succeed");
		const bool locked = _texture->isLocked();
		std::vector<unsigned char> result(
			data,
			data + size_t(_texture->getWidth()) * size_t(_texture->getHeight()) * _texture->getNumElemBytes());
		_texture->unlock();
		require(locked && !_texture->isLocked(), "Read lock state transitions must be observable");
		return result;
	}

	void checkInterleavedLocks(Fixture& _fixture)
	{
		checkInterleavedLocks(_fixture, {});
	}

	void checkInterleavedLocks(Fixture& _fixture, const std::function<void()>& _checkState)
	{
		const auto checkState = [&]
		{
			if (_checkState)
				_checkState();
		};
		const auto usage = MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write;
		requireSupport(_fixture, MyGUI::PixelFormat::R8G8B8A8, usage);
		auto* a = _fixture.texture();
		auto* b = _fixture.texture();
		a->createManual(3, 2, usage, MyGUI::PixelFormat::R8G8B8A8);
		checkState();
		b->createManual(5, 3, usage, MyGUI::PixelFormat::R8G8B8A8);
		checkState();
		for (bool reverse : {false, true})
		{
			auto* first = static_cast<unsigned char*>(a->lock(MyGUI::TextureUsage::Write));
			checkState();
			auto* second = static_cast<unsigned char*>(b->lock(MyGUI::TextureUsage::Write));
			require(first && second, "Different textures must support simultaneous locks");
			std::fill_n(first, 24, reverse ? 17 : 123);
			std::fill_n(second, 60, reverse ? 29 : 231);
			checkState();
			(reverse ? b : a)->unlock();
			checkState();
			(reverse ? a : b)->unlock();
			checkState();
			require(read(a) == std::vector<unsigned char>(24, reverse ? 17 : 123), "First lock lost its data");
			checkState();
			require(read(b) == std::vector<unsigned char>(60, reverse ? 29 : 231), "Second lock lost its data");
			checkState();
		}
	}

	MyGUI::ITexture* solid(Fixture& _fixture, Pixel _pixel)
	{
		auto* texture = _fixture.texture();
		const auto usage = MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write;
		require(
			_fixture.render().isFormatSupported(MyGUI::PixelFormat::R8G8B8A8, usage),
			"Basic RGBA write support is required");
		texture->createManual(1, 1, usage, MyGUI::PixelFormat::R8G8B8A8);
		upload(texture, {_pixel[2], _pixel[1], _pixel[0], _pixel[3]});
		return texture;
	}

	void requireSupport(Fixture& _fixture, MyGUI::PixelFormat _format, MyGUI::TextureUsage _usage)
	{
		if (!_fixture.render().isFormatSupported(_format, _usage))
			throw Skip(
				"Optional format/usage not advertised: " + std::to_string(_format.getValue()) + "/" +
				std::to_string(_usage.getValue()));
	}

}

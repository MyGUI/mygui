#pragma once

#include "BaseManager.h"
#include "TestSupport.h"
#include "MyGUI_OverlappedLayer.h"
#include <array>
#include <functional>
#include <stdexcept>
#include <utility>

namespace platformtest
{

	using unittest::require;
	using Pixel = std::array<unsigned char, 4>;
	using Capture = base::SdlBaseManager::FrameCapture;

	class Skip : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class Fatal : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class CallbackLayer : public MyGUI::OverlappedLayer
	{
		MYGUI_RTTI_DERIVED(CallbackLayer)
	public:
		std::function<void(MyGUI::IRenderTarget*)> callback;
		void renderToTarget(MyGUI::IRenderTarget* _target, bool _update) override;
	};

	class Fixture : public base::BaseManager
	{
	public:
		explicit Fixture(bool _visible);
		~Fixture() override;
		Fixture(const Fixture&) = delete;
		Fixture& operator=(const Fixture&) = delete;
		void open();
		void close();
		void resetCase();
		void scene(std::function<void(MyGUI::IRenderTarget*)> _callback);
		const Capture& capture();
		void expect(int _x, int _y, Pixel _expected) const;
		void expectCorners(Pixel _expected) const;
		void saveFailure(const std::filesystem::path& _file) const;
		MyGUI::ITexture* texture(const std::string& _name = "");
		void removeTexture(MyGUI::ITexture* _texture);
		MyGUI::IVertexBuffer* buffer();
		void quad(
			MyGUI::IRenderTarget* _target,
			MyGUI::ITexture* _texture,
			Pixel _colour = {255, 255, 255, 255},
			MyGUI::IntCoord _rect = {0, 0, 128, 128},
			MyGUI::FloatRect _uv = {0, 0, 1, 1});
		void fill(
			MyGUI::IVertexBuffer* _buffer,
			size_t _capacity,
			const MyGUI::RenderTargetInfo& _info,
			MyGUI::IntCoord _rect,
			Pixel _colour,
			MyGUI::FloatRect _uv = {0, 0, 1, 1});
		MyGUI::RenderManager& render() const;
		std::string caseName;

	protected:
		void setupResources() override;
		void createScene() override;
		void destroyScene() override;

	private:
		void releaseResources();
		CallbackLayer* mLayer{nullptr};
		std::vector<MyGUI::ITexture*> mTextures;
		std::vector<MyGUI::IVertexBuffer*> mBuffers;
		std::vector<MyGUI::IVertexBuffer*> mDrawBuffers;
		size_t mDrawIndex{0};
		size_t mNameIndex{0};
		bool mVisible;
		bool mCreated{false};
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
		bool mComInitialized{false};
#endif
	};

	struct Case
	{
		std::string group;
		std::string name;
		std::function<void(Fixture&)> run;
	};

	std::vector<Case> cases();
	std::string_view backendName();
	std::pair<std::string, std::string> shaderFiles();
	void upload(MyGUI::ITexture* _texture, const std::vector<unsigned char>& _bytes);
	std::vector<unsigned char> read(MyGUI::ITexture* _texture);
	// Native suites can inspect host state during the same portable lock sequence.
	void checkInterleavedLocks(Fixture& _fixture, const std::function<void()>& _checkState = {});
	MyGUI::ITexture* solid(Fixture& _fixture, Pixel _pixel);
	void requireSupport(Fixture& _fixture, MyGUI::PixelFormat _format, MyGUI::TextureUsage _usage);

}

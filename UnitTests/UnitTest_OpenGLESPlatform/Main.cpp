#include "TestSupport.h"
#include "TestRunner.h"
#include "MyGUI_OpenGLESPlatform.h"
#include "MyGUI_OpenGLESTexture.h"
#include "MyGUI_OpenGLESVertexBuffer.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <algorithm>
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>

namespace
{

	using unittest::require;
	using Bytes = std::vector<unsigned char>;

	class Context
	{
	public:
		Context()
		{
			// Optional ANGLE platform selection keeps the test usable on native EGL too.
			const char* platform = std::getenv("MYGUI_TEST_EGL_PLATFORM");
			if (platform && std::string_view(platform) == "angle-metal")
			{
				PFNEGLGETPLATFORMDISPLAYEXTPROC getDisplay = nullptr;
				const auto address = eglGetProcAddress("eglGetPlatformDisplayEXT");
				static_assert(sizeof(getDisplay) == sizeof(address));
				std::memcpy(&getDisplay, &address, sizeof(getDisplay));
				require(getDisplay != nullptr, "EGL platform extension is required for ANGLE");
				const EGLint attributes[] = {0x3203, 0x3489, EGL_NONE}; // ANGLE type: Metal
				mDisplay = getDisplay(0x3202, nullptr, attributes); // ANGLE platform
			}
			else
				mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			require(mDisplay != EGL_NO_DISPLAY && eglInitialize(mDisplay, nullptr, nullptr), "Cannot initialize EGL");
			require(eglBindAPI(EGL_OPENGL_ES_API), "Cannot select OpenGL ES");
			const EGLint configAttributes[] = {
				EGL_SURFACE_TYPE,
				EGL_PBUFFER_BIT,
				EGL_RENDERABLE_TYPE,
				EGL_OPENGL_ES3_BIT,
				EGL_RED_SIZE,
				8,
				EGL_GREEN_SIZE,
				8,
				EGL_BLUE_SIZE,
				8,
				EGL_ALPHA_SIZE,
				8,
				EGL_NONE};
			EGLConfig config = nullptr;
			EGLint count = 0;
			require(
				eglChooseConfig(mDisplay, configAttributes, &config, 1, &count) && count == 1,
				"No ES 3 EGL config");
			const EGLint surfaceAttributes[] = {EGL_WIDTH, 16, EGL_HEIGHT, 16, EGL_NONE};
			mSurface = eglCreatePbufferSurface(mDisplay, config, surfaceAttributes);
			const EGLint contextAttributes[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
			mContext = eglCreateContext(mDisplay, config, EGL_NO_CONTEXT, contextAttributes);
			require(
				mSurface != EGL_NO_SURFACE && mContext != EGL_NO_CONTEXT &&
					eglMakeCurrent(mDisplay, mSurface, mSurface, mContext),
				"Cannot create ES 3 context");
			std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
		}
		~Context()
		{
			eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			eglDestroyContext(mDisplay, mContext);
			eglDestroySurface(mDisplay, mSurface);
			eglTerminate(mDisplay);
		}

	private:
		EGLDisplay mDisplay{EGL_NO_DISPLAY};
		EGLContext mContext{EGL_NO_CONTEXT};
		EGLSurface mSurface{EGL_NO_SURFACE};
	};

	constexpr std::array<GLenum, 8> transferParameters = {
		GL_PACK_ALIGNMENT,
		GL_PACK_ROW_LENGTH,
		GL_PACK_SKIP_ROWS,
		GL_PACK_SKIP_PIXELS,
		GL_UNPACK_ALIGNMENT,
		GL_UNPACK_ROW_LENGTH,
		GL_UNPACK_SKIP_ROWS,
		GL_UNPACK_SKIP_PIXELS};
	constexpr std::array<GLint, 8> hostileValues = {8, 9, 1, 2, 8, 7, 2, 1};

	GLint integer(GLenum name)
	{
		GLint value = 0;
		glGetIntegerv(name, &value);
		return value;
	}

	class HostTransfers
	{
	public:
		HostTransfers()
		{
			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glGenBuffers(2, mBuffers.data());
			std::array<unsigned char, 512> bytes;
			bytes.fill(0xa5);
			for (size_t i = 0; i < mBuffers.size(); ++i)
			{
				const auto target = i == 0 ? GL_PIXEL_PACK_BUFFER : GL_PIXEL_UNPACK_BUFFER;
				glBindBuffer(target, mBuffers[i]);
				glBufferData(target, bytes.size(), bytes.data(), GL_STATIC_DRAW);
			}
			for (size_t i = 0; i < transferParameters.size(); ++i)
				glPixelStorei(transferParameters[i], hostileValues[i]);
		}
		~HostTransfers()
		{
			glDeleteBuffers(2, mBuffers.data());
			glDeleteTextures(1, &mTexture);
			for (auto name : transferParameters)
				glPixelStorei(name, name == GL_PACK_ALIGNMENT || name == GL_UNPACK_ALIGNMENT ? 4 : 0);
		}
		void check() const
		{
			for (size_t i = 0; i < transferParameters.size(); ++i)
				require(integer(transferParameters[i]) == hostileValues[i], "Host pixel-store state changed");
			require(GLuint(integer(GL_TEXTURE_BINDING_2D)) == mTexture, "Host texture binding changed");
			require(GLuint(integer(GL_PIXEL_PACK_BUFFER_BINDING)) == mBuffers[0], "Host pack buffer changed");
			require(GLuint(integer(GL_PIXEL_UNPACK_BUFFER_BINDING)) == mBuffers[1], "Host unpack buffer changed");
			for (auto target : {GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER})
			{
				const auto* bytes =
					static_cast<const unsigned char*>(glMapBufferRange(target, 0, 512, GL_MAP_READ_BIT));
				require(bytes != nullptr, "Cannot inspect host buffer");
				const bool intact = std::all_of(bytes, bytes + 512, [](auto byte) { return byte == 0xa5; });
				glUnmapBuffer(target);
				require(intact, "Texture transfer overwrote host PBO storage");
			}
			require(glGetError() == GL_NO_ERROR, "Transfer produced a GL error");
		}

	private:
		GLuint mTexture{};
		std::array<GLuint, 2> mBuffers{};
	};

	Bytes read(MyGUI::ITexture& texture)
	{
		const auto* data = static_cast<const unsigned char*>(texture.lock(MyGUI::TextureUsage::Read));
		Bytes result(data, data + size_t(texture.getWidth()) * size_t(texture.getHeight()) * texture.getNumElemBytes());
		texture.unlock();
		return result;
	}

	void testTransfers()
	{
		HostTransfers host;
		for (auto format :
			 {MyGUI::PixelFormat::R8G8B8A8,
			  MyGUI::PixelFormat::R8G8B8,
			  MyGUI::PixelFormat::L8,
			  MyGUI::PixelFormat::L8A8})
		{
			for (auto hint : {MyGUI::TextureUsage::Static, MyGUI::TextureUsage::Dynamic, MyGUI::TextureUsage::Stream})
			{
				MyGUI::OpenGLESTexture texture("transfer", nullptr);
				texture.createManual(3, 2, hint | MyGUI::TextureUsage::Write, format);
				host.check();
				Bytes expected(6 * texture.getNumElemBytes());
				for (size_t i = 0; i < expected.size(); ++i)
					expected[i] = static_cast<unsigned char>(i * 11);
				auto* data = static_cast<unsigned char*>(texture.lock(MyGUI::TextureUsage::Write));
				std::copy(expected.begin(), expected.end(), data);
				unittest::requireThrows(
					[&] { texture.lock(MyGUI::TextureUsage::Write); },
					"Invalid operation must be rejected");
				texture.unlock();
				host.check();
				require(read(texture) == expected, "Odd-width upload/readback mismatch");
				data = static_cast<unsigned char*>(texture.lock(MyGUI::TextureUsage::Read));
				require(texture.isLocked(), "Read lock must report locked");
				data[0] ^= 255;
				texture.unlock();
				require(!texture.isLocked() && read(texture) == expected, "Read unlock modified texture");
				data =
					static_cast<unsigned char*>(texture.lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
				require(std::equal(expected.begin(), expected.end(), data), "Read/write lock lost existing pixels");
				data[expected.size() - 1] = 23;
				expected.back() = 23;
				texture.unlock();
				require(read(texture) == expected, "Partial read/write update failed");
				host.check();
			}
		}
		MyGUI::OpenGLESTexture first("first", nullptr), second("second", nullptr);
		const auto format = MyGUI::PixelFormat::R8G8B8A8;
		first.createManual(1, 1, MyGUI::TextureUsage::Write, format);
		second.createManual(1, 1, MyGUI::TextureUsage::Write, format);
		auto* a = static_cast<unsigned char*>(first.lock(MyGUI::TextureUsage::Write));
		auto* b = static_cast<unsigned char*>(second.lock(MyGUI::TextureUsage::Write));
		std::fill_n(a, 4, 123);
		std::fill_n(b, 4, 231);
		first.unlock();
		second.unlock();
		require(
			read(first) == Bytes(4, 123) && read(second) == Bytes(4, 231),
			"Interleaved locks changed another texture");
		first.lock(MyGUI::TextureUsage::Write);
		first.destroy(); // ASan checks cleanup of outstanding CPU staging storage.
		require(!first.isLocked(), "Destroy must reset lock state");
		unittest::requireThrows(
			[&] { first.createManual(-1, 1, MyGUI::TextureUsage::Write, format); },
			"Invalid operation must be rejected");
		unittest::requireThrows(
			[&] { first.createManual(std::numeric_limits<int>::max(), 2, MyGUI::TextureUsage::Write, format); },
			"Invalid operation must be rejected");
		host.check();
	}

	class Loader : public MyGUI::OpenGLESImageLoader
	{
	public:
		void* loadImage(int& width, int& height, MyGUI::PixelFormat& format, const std::string&) override
		{
			width = height = 1;
			format = MyGUI::PixelFormat::R8G8B8A8;
			return new unsigned char[4]{17, 29, 41, 255};
		}
		void saveImage(int, int, MyGUI::PixelFormat, void* data, const std::filesystem::path&) override
		{
			require(static_cast<unsigned char*>(data)[0] == 17, "Save must receive current texture bytes");
			throw std::runtime_error("Simulated image encoder failure");
		}
	};

	void testLoader()
	{
		HostTransfers host;
		Loader loader;
		MyGUI::OpenGLESTexture texture("loaded", &loader);
		texture.loadFromFile("synthetic");
		require(read(texture) == Bytes({17, 29, 41, 255}), "Initial CPU upload mismatch");
		unittest::requireThrows<std::runtime_error>(
			[&] { texture.saveToFile("synthetic"); },
			"Failed save must release its read lock");
		require(!texture.isLocked(), "Failed save must release its read lock");
		auto* bytes = static_cast<unsigned char*>(texture.lock(MyGUI::TextureUsage::Write));
		std::fill_n(bytes, 4, 77);
		texture.unlock();
		require(read(texture) == Bytes(4, 77), "Loaded texture must support PBO updates");
		host.check();
	}

}

namespace glestest
{

	class ThrowingLayer : public MyGUI::OverlappedLayer
	{
		MYGUI_RTTI_DERIVED(ThrowingLayer)
	public:
		[[noreturn]] void renderToTarget(MyGUI::IRenderTarget*, bool) override
		{
			throw std::runtime_error("Simulated render callback failure");
		}
	};

}

namespace
{

	using glestest::ThrowingLayer;

	constexpr std::array<GLenum, 8> modes = {
		GL_BLEND,
		GL_CULL_FACE,
		GL_DEPTH_TEST,
		GL_STENCIL_TEST,
		GL_SCISSOR_TEST,
		GL_RASTERIZER_DISCARD,
		GL_SAMPLE_ALPHA_TO_COVERAGE,
		GL_SAMPLE_COVERAGE};

	struct Snapshot
	{
		std::vector<GLint> values;
		Snapshot()
		{
			for (auto mode : modes)
				values.push_back(glIsEnabled(mode));
			for (auto name :
				 {GL_BLEND_SRC_RGB,
				  GL_BLEND_DST_RGB,
				  GL_BLEND_SRC_ALPHA,
				  GL_BLEND_DST_ALPHA,
				  GL_BLEND_EQUATION_RGB,
				  GL_BLEND_EQUATION_ALPHA,
				  GL_DEPTH_WRITEMASK,
				  GL_CURRENT_PROGRAM,
				  GL_VERTEX_ARRAY_BINDING,
				  GL_ARRAY_BUFFER_BINDING,
				  GL_ACTIVE_TEXTURE,
				  GL_DRAW_FRAMEBUFFER_BINDING,
				  GL_READ_FRAMEBUFFER_BINDING,
				  GL_RENDERBUFFER_BINDING})
				values.push_back(integer(name));
			const auto active = integer(GL_ACTIVE_TEXTURE);
			for (auto unit : {GL_TEXTURE0, GL_TEXTURE3})
			{
				glActiveTexture(unit);
				values.push_back(integer(GL_TEXTURE_BINDING_2D));
				values.push_back(integer(GL_SAMPLER_BINDING));
			}
			glActiveTexture(active);
			for (auto name : {GL_COLOR_WRITEMASK, GL_VIEWPORT})
			{
				std::array<GLint, 4> result{};
				glGetIntegerv(name, result.data());
				values.insert(values.end(), result.begin(), result.end());
			}
			std::array<GLfloat, 4> clear{};
			glGetFloatv(GL_COLOR_CLEAR_VALUE, clear.data());
			for (auto value : clear)
				values.push_back(static_cast<GLint>(value * 255));
		}
	};

	void fill(MyGUI::OpenGLESVertexBuffer& buffer, size_t capacity = 6)
	{
		buffer.setVertexCount(capacity);
		const std::array<MyGUI::Vertex, 6> vertices = {
			{{-1, 1, 0, 0xffffffff, 0, 0},
			 {1, 1, 0, 0xffffffff, 1, 0},
			 {-1, -1, 0, 0xffffffff, 0, 1},
			 {-1, -1, 0, 0xffffffff, 0, 1},
			 {1, 1, 0, 0xffffffff, 1, 0},
			 {1, -1, 0, 0xffffffff, 1, 1}}};
		std::copy(vertices.begin(), vertices.end(), buffer.lock());
		buffer.unlock();
	}

	void testRendering(MyGUI::OpenGLESRenderManager& renderer)
	{
		GLuint vao = 0, buffer = 0, sampler = 0, renderbuffer = 0;
		std::array<GLuint, 2> framebuffers{}, textures{};
		glGenFramebuffers(2, framebuffers.data());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffers[0]);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffers[1]);
		glGenRenderbuffers(1, &renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
		glGenTextures(2, textures.data());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glGenSamplers(1, &sampler);
		glBindSampler(0, sampler);
		glSamplerParameteri(sampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		for (auto mode : modes)
			glEnable(mode);
		glDisable(GL_BLEND);
		glCullFace(GL_FRONT_AND_BACK);
		glScissor(0, 0, 0, 0);
		glDepthMask(GL_TRUE);
		glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE);
		glBlendEquationSeparate(GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT);
		glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ZERO, GL_ONE);
		glUseProgram(renderer.getShaderProgramId("Default"));
		glActiveTexture(GL_TEXTURE3);
		glViewport(1, 2, 8, 9);
		glClearColor(0.2f, 0.4f, 0.6f, 0.8f);
		const Snapshot before;
		{
			MyGUI::Gui gui;
			renderer.setViewSize(16, 16);
			gui.initialise("");
			MyGUI::FactoryManager::getInstance().registerFactory<ThrowingLayer>(
				MyGUI::LayerManager::getInstance().getCategoryName());
			MyGUI::LayerManager::getInstance().createLayerAt("Throwing", "ThrowingLayer", 0);
			bool failed = false;
			try
			{
				renderer.drawOneFrame();
			}
			catch (const std::runtime_error&)
			{
				failed = true;
			}
			gui.shutdown();
			require(failed && Snapshot().values == before.values, "Failed GUI callback must restore host state");
		}
		MyGUI::OpenGLESVertexBuffer vertices;
		fill(vertices);
		require(Snapshot().values == before.values, "Vertex creation/update changed host bindings");
		vertices.lock();
		unittest::requireThrows([&] { vertices.lock(); }, "Invalid operation must be rejected");
		unittest::requireThrows([&] { vertices.setVertexCount(8); }, "Invalid operation must be rejected");
		vertices.unlock();
		unittest::requireThrows([&] { vertices.unlock(); }, "Invalid operation must be rejected");
		unittest::requireThrows(
			[&] { vertices.setVertexCount(std::numeric_limits<size_t>::max()); },
			"Invalid operation must be rejected");
		fill(vertices);
		MyGUI::OpenGLESTexture source("source", nullptr), target("target", nullptr), nested("nested", nullptr);
		const auto format = MyGUI::PixelFormat::R8G8B8A8;
		// Public bytes are BGRA: red top row, blue bottom row.
		unsigned char pixels[] = {0, 0, 255, 255, 255, 0, 0, 255};
		source.createManual(1, 2, MyGUI::TextureUsage::Write, format, pixels);
		target.createManual(8, 8, MyGUI::TextureUsage::RenderTarget, format);
		nested.createManual(4, 4, MyGUI::TextureUsage::RenderTarget, format);
		auto* rtt = target.getRenderTarget();
		auto* inner = nested.getRenderTarget();
		require(Snapshot().values == before.values, "RTT creation changed host state");
		renderer.registerShader("Swap", "Offset_VP.glsl", "PlatformSwap_GLES_FP.glsl");
		require(
			glGetUniformLocation(renderer.getShaderProgramId("Default"), "YScale") !=
				glGetUniformLocation(renderer.getShaderProgramId("Swap"), "YScale"),
			"Shader fixture must exercise different YScale uniform locations");
		source.setShader("Swap");
		const auto oldProgram = source.getShaderId();
		unittest::requireThrows(
			[&] { renderer.registerShader("Swap", "Missing_VP.glsl", "PlatformSwap_GLES_FP.glsl"); },
			"Invalid operation must be rejected");
		require(source.getShaderId() == oldProgram, "Failed replacement must preserve a working shader");
		renderer.registerShader("Swap", "Offset_VP.glsl", "PlatformSwap_GLES_FP.glsl");
		require(source.getShaderId() != oldProgram, "Texture must resolve replacement shader handles");
		require(Snapshot().values == before.values, "Shader registration changed current program");
		for (const auto& shader : {"", "Swap"})
		{
			source.setShader(shader);
			rtt->begin();
			rtt->doRender(&vertices, &source, 6);
			fill(vertices, 64);
			rtt->doRender(&vertices, &source, 6);
			const Snapshot outer;
			inner->begin();
			inner->doRender(&vertices, &source, 6);
			inner->end();
			require(Snapshot().values == outer.values, "Nested RTT failed to restore outer pass");
			rtt->end();
			require(Snapshot().values == before.values, "RTT failed to restore host state");
			const auto result = read(target);
			const bool swapped = shader[0] != '\0';
			// RTT flips geometry so its first readback row corresponds to the GUI top.
			require(
				result[swapped ? 0 : 2] == 255 && result[swapped ? 2 : 0] == 0,
				"RTT top row has incorrect colour or orientation");
			const size_t last = 7 * 8 * 4;
			require(
				result[last + (swapped ? 2 : 0)] == 255 && result[last + (swapped ? 0 : 2)] == 0,
				"RTT bottom row has incorrect colour or orientation");
			require(glGetError() == GL_NO_ERROR, "Rendering produced a GL error");
		}
		// Sampling GPU-written pixels must use the same channel order as CPU uploads.
		source.setShader("");
		rtt->begin();
		rtt->doRender(&vertices, &source, 6);
		rtt->end();
		inner->begin();
		inner->doRender(&vertices, &target, 6);
		inner->end();
		const auto sampled = read(nested);
		require(sampled[2] > 240 && sampled[0] < 15, "Sampling an RTT swapped red and blue");
		for (auto mode : modes)
			glDisable(mode);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glBindSampler(0, 0);
		glDeleteSamplers(1, &sampler);
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vao);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &buffer);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(2, framebuffers.data());
		glDeleteRenderbuffers(1, &renderbuffer);
		glDeleteTextures(2, textures.data());
	}

}

// SDL's Windows entry-point wrapper requires the argc/argv signature, even when unused.
int main(int, char**)
{
	std::cout << std::unitbuf;
	try
	{
		Context context;
		MyGUI::OpenGLESPlatform platform;
		platform.getDataManagerPtr()->addResourceLocation(unittest::getMediaPath(), false);
		platform.getDataManagerPtr()->addResourceLocation(unittest::getResourcePath("UnitTest_Platform"), false);
		platform.getDataManagerPtr()->addResourceLocation(
			unittest::getResourcePath("UnitTest_OpenGLESPlatform"),
			false);
		platform.initialise(nullptr, "");
		int result = 1;
		try
		{
			require(
				platform.getRenderManagerPtr()->isPixelBufferObjectSupported(),
				"ES 3 core PBO support must be recognized");
			result = unittest::runTests(
				{
					{"transfers", testTransfers},
					{"loader", testLoader},
					{
						"rendering",
						[&] { testRendering(*platform.getRenderManagerPtr()); },
					},
				},
				unittest::FailurePolicy::Stop);
		}
		catch (...)
		{
			platform.shutdown();
			throw;
		}
		platform.shutdown();
		return result;
	}
	catch (const std::exception& error)
	{
		std::cerr << "FATAL backend OpenGLES: " << error.what() << '\n';
		return 1;
	}
}

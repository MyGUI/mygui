#include "PlatformFixture.h"
#include "MyGUI_OpenGLTexture.h"
#include <MyGUI_GL.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <limits>

namespace
{

	using unittest::require;

	constexpr std::array<GLenum, 8> pixelParameters = {
		GL_PACK_ALIGNMENT,
		GL_PACK_ROW_LENGTH,
		GL_PACK_SKIP_ROWS,
		GL_PACK_SKIP_PIXELS,
		GL_UNPACK_ALIGNMENT,
		GL_UNPACK_ROW_LENGTH,
		GL_UNPACK_SKIP_ROWS,
		GL_UNPACK_SKIP_PIXELS};
	constexpr std::array<GLint, 8> hostileValues = {8, 7, 2, 1, 8, 9, 1, 2};

	class HostTransfers
	{
	public:
		HostTransfers()
		{
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &mTextureBinding);
			glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &mPackBinding);
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &mUnpackBinding);
			for (size_t i = 0; i < pixelParameters.size(); ++i)
			{
				glGetIntegerv(pixelParameters[i], &mValues[i]);
				glPixelStorei(pixelParameters[i], hostileValues[i]);
			}
			glGenTextures(1, &mTexture);
			glBindTexture(GL_TEXTURE_2D, mTexture);
			glGenBuffers(2, mBuffers.data());
			std::array<unsigned char, 512> bytes;
			bytes.fill(0xa5);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, mBuffers[0]);
			glBufferData(GL_PIXEL_PACK_BUFFER, bytes.size(), bytes.data(), GL_STATIC_DRAW);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mBuffers[1]);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, bytes.size(), bytes.data(), GL_STATIC_DRAW);
		}
		~HostTransfers()
		{
			glBindTexture(GL_TEXTURE_2D, mTextureBinding);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, mPackBinding);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mUnpackBinding);
			glDeleteTextures(1, &mTexture);
			glDeleteBuffers(2, mBuffers.data());
			for (size_t i = 0; i < pixelParameters.size(); ++i)
				glPixelStorei(pixelParameters[i], mValues[i]);
		}
		void check() const
		{
			GLint value = 0;
			for (size_t i = 0; i < pixelParameters.size(); ++i)
			{
				glGetIntegerv(pixelParameters[i], &value);
				require(value == hostileValues[i], "Transfer must restore host pixel-store state");
			}
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &value);
			require(GLuint(value) == mTexture, "Transfer must restore host texture binding");
			glGetIntegerv(GL_PIXEL_PACK_BUFFER_BINDING, &value);
			require(GLuint(value) == mBuffers[0], "Transfer must restore host pack buffer binding");
			glGetIntegerv(GL_PIXEL_UNPACK_BUFFER_BINDING, &value);
			require(GLuint(value) == mBuffers[1], "Transfer must restore host unpack buffer binding");
			for (auto target : {GL_PIXEL_PACK_BUFFER, GL_PIXEL_UNPACK_BUFFER})
			{
				std::array<unsigned char, 512> bytes{};
				glGetBufferSubData(target, 0, bytes.size(), bytes.data());
				require(
					std::all_of(bytes.begin(), bytes.end(), [](auto byte) { return byte == 0xa5; }),
					"Transfer must not modify host pixel buffers");
			}
			require(glGetError() == GL_NO_ERROR, "Texture transfers must not generate GL errors");
		}

	private:
		GLint mTextureBinding{}, mPackBinding{}, mUnpackBinding{};
		std::array<GLint, 8> mValues{};
		GLuint mTexture{};
		std::array<GLuint, 2> mBuffers{};
	};

	void testTransfers(platformtest::Fixture& fixture)
	{
		HostTransfers host;
		for (auto format :
			 {MyGUI::PixelFormat::R8G8B8,
			  MyGUI::PixelFormat::R8G8B8A8,
			  MyGUI::PixelFormat::L8,
			  MyGUI::PixelFormat::L8A8})
			for (auto hint :
				 {MyGUI::TextureUsage::Static,
				  MyGUI::TextureUsage::Dynamic,
				  MyGUI::TextureUsage::Stream,
				  MyGUI::TextureUsage::RenderTarget})
			{
				auto* texture = fixture.texture();
				texture->createManual(3, 2, hint, format);
				host.check();
				std::vector<unsigned char> expected(6 * texture->getNumElemBytes());
				for (size_t i = 0; i < expected.size(); ++i)
					expected[i] = static_cast<unsigned char>(i * 11);
				platformtest::upload(texture, expected);
				host.check();
				auto* bytes = static_cast<unsigned char*>(texture->lock(MyGUI::TextureUsage::Read));
				const bool locked = texture->isLocked();
				const bool correct = std::equal(expected.begin(), expected.end(), bytes);
				bytes[0] = 255; // A read-only staging copy must not be uploaded by unlock.
				texture->unlock();
				require(locked && !texture->isLocked(), "Read lock must report its lifetime");
				require(correct && platformtest::read(texture) == expected, "Read lock must preserve texture bytes");
				host.check();
				bytes =
					static_cast<unsigned char*>(texture->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
				const bool preserved = std::equal(expected.begin(), expected.end(), bytes);
				bytes[expected.size() - 1] = 23;
				expected.back() = 23;
				host.check();
				texture->unlock();
				require(
					preserved && platformtest::read(texture) == expected,
					"PBO read/write must preserve untouched texels");
				host.check();
			}
		// File loading supplies initial CPU data, so no upload PBO exists yet.
		auto* loaded = fixture.texture();
		loaded->loadFromFile("TransparentRgb.png");
		host.check();
		const std::vector<unsigned char> white(32, 255);
		platformtest::upload(loaded, white);
		require(platformtest::read(loaded) == white, "File-loaded texture must support its first PBO write");
		host.check();
	}

	void testInterleavedLocks(platformtest::Fixture& fixture)
	{
		auto* a = platformtest::solid(fixture, {255, 0, 0, 255});
		auto* b = platformtest::solid(fixture, {0, 255, 0, 255});
		HostTransfers host;
		auto* first = static_cast<unsigned char*>(a->lock(MyGUI::TextureUsage::Write));
		auto* second = static_cast<unsigned char*>(b->lock(MyGUI::TextureUsage::Write));
		std::fill_n(first, 4, 123);
		std::fill_n(second, 4, 231);
		host.check();
		a->unlock();
		b->unlock();
		require(platformtest::read(a) == std::vector<unsigned char>(4, 123), "First lock must upload its own PBO");
		require(platformtest::read(b) == std::vector<unsigned char>(4, 231), "Second lock must upload its own PBO");
		host.check();
	}

	void testTransferFailures(platformtest::Fixture& fixture)
	{
		HostTransfers host;
		auto* texture = fixture.texture();
		for (int size : {0, -1, std::numeric_limits<int>::max()})
		{
			bool rejected = false;
			try
			{
				texture->createManual(size, size, MyGUI::TextureUsage::Write, MyGUI::PixelFormat::R8G8B8A8);
			}
			catch (const MyGUI::Exception&)
			{
				rejected = true;
			}
			require(rejected, "Invalid dimensions must be rejected before issuing GL allocation calls");
			host.check();
		}
		for (auto access : {MyGUI::TextureUsage::Read, MyGUI::TextureUsage::Write})
		{
			texture->createManual(3, 2, MyGUI::TextureUsage::Static, MyGUI::PixelFormat::R8G8B8);
			platformtest::upload(texture, std::vector<unsigned char>(18, 42));
			texture->lock(access);
			bool rejected = false;
			try
			{
				texture->lock(access);
			}
			catch (const MyGUI::Exception&)
			{
				rejected = true;
			}
			require(rejected && texture->isLocked(), "Rejected second lock must retain the original lock");
			host.check();
			texture->destroy(); // Both CPU read locks and mapped PBO writes must be released.
			require(!texture->isLocked(), "Destroy must release an outstanding lock");
			host.check();
		}
	}

	constexpr std::array<GLenum, 7> rasterModes =
		{GL_BLEND, GL_CULL_FACE, GL_DEPTH_TEST, GL_STENCIL_TEST, GL_SCISSOR_TEST, GL_ALPHA_TEST, GL_COLOR_LOGIC_OP};

	std::vector<GLint> rasterState()
	{
		std::vector<GLint> values;
		for (auto mode : rasterModes)
			values.push_back(glIsEnabled(mode));
		for (auto parameter :
			 {GL_BLEND_SRC_RGB,
			  GL_BLEND_DST_RGB,
			  GL_BLEND_SRC_ALPHA,
			  GL_BLEND_DST_ALPHA,
			  GL_BLEND_EQUATION_RGB,
			  GL_BLEND_EQUATION_ALPHA,
			  GL_CURRENT_PROGRAM,
			  GL_ACTIVE_TEXTURE,
			  GL_CLIENT_ACTIVE_TEXTURE,
			  GL_MATRIX_MODE,
			  GL_ARRAY_BUFFER_BINDING,
			  GL_DEPTH_WRITEMASK,
			  GL_DEPTH_FUNC,
			  GL_CULL_FACE_MODE,
			  GL_STENCIL_FUNC,
			  GL_STENCIL_REF,
			  GL_STENCIL_VALUE_MASK,
			  GL_LOGIC_OP_MODE})
		{
			GLint value = 0;
			glGetIntegerv(parameter, &value);
			values.push_back(value);
		}
		GLint active = 0, texture = 0;
		glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
		values.push_back(texture);
		glActiveTexture(GL_TEXTURE0);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &texture);
		values.push_back(texture);
		glActiveTexture(active);
		std::array<GLint, 4> mask{}, scissor{};
		std::array<GLint, 2> polygon{};
		glGetIntegerv(GL_COLOR_WRITEMASK, mask.data());
		glGetIntegerv(GL_SCISSOR_BOX, scissor.data());
		glGetIntegerv(GL_POLYGON_MODE, polygon.data());
		values.insert(values.end(), mask.begin(), mask.end());
		values.insert(values.end(), scissor.begin(), scissor.end());
		values.insert(values.end(), polygon.begin(), polygon.end());
		GLint client = 0;
		glGetIntegerv(GL_CLIENT_ACTIVE_TEXTURE, &client);
		for (auto unit : {GL_TEXTURE0, GL_TEXTURE2})
		{
			glActiveTexture(unit);
			glClientActiveTexture(unit);
			values.push_back(glIsEnabled(GL_TEXTURE_2D));
			values.push_back(glIsEnabled(GL_TEXTURE_COORD_ARRAY));
			std::array<GLfloat, 16> matrix{};
			glGetFloatv(GL_TEXTURE_MATRIX, matrix.data());
			for (auto value : matrix)
				values.push_back(static_cast<GLint>(value * 1024));
		}
		glActiveTexture(active);
		glClientActiveTexture(client);
		return values;
	}

	void resetRasterState()
	{
		for (auto mode : rasterModes)
			glDisable(mode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
		glUseProgram(0);
		glClientActiveTexture(GL_TEXTURE0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE2);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
	}

	void testRasterState(platformtest::Fixture& fixture)
	{
		auto* white = platformtest::solid(fixture, {255, 255, 255, 255});
		fixture.scene(
			[&](MyGUI::IRenderTarget* target)
			{
				require(!glIsEnabled(GL_STENCIL_TEST), "GUI must disable inherited stencil testing");
				GLint active = 0;
				glGetIntegerv(GL_ACTIVE_TEXTURE, &active);
				require(active == GL_TEXTURE0, "GUI must use texture unit zero");
				std::array<GLfloat, 16> matrix{};
				glGetFloatv(GL_TEXTURE_MATRIX, matrix.data());
				const std::array<GLfloat, 16> identity{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
				require(matrix == identity, "GUI must isolate host texture matrix");
				fixture.quad(target, white);
			});
		for (auto mode :
			 {GL_CULL_FACE, GL_DEPTH_TEST, GL_POLYGON_MODE, GL_SCISSOR_TEST, GL_ALPHA_TEST, GL_STENCIL_TEST})
		{
			resetRasterState();
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			if (mode == GL_POLYGON_MODE)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glEnable(mode);
			glCullFace(GL_FRONT_AND_BACK);
			glDepthFunc(GL_NEVER);
			glAlphaFunc(GL_NEVER, 0.0f);
			glScissor(0, 0, 0, 0);
			glStencilFunc(GL_NEVER, 0, ~0u);
			const auto saved = rasterState();
			fixture.capture();
			fixture.expectCorners({255, 255, 255, 255});
			require(rasterState() == saved, "GUI must restore each hostile raster state");
		}
		resetRasterState();
		GLuint buffer = 0;
		std::array<GLuint, 2> textures{};
		glGenBuffers(1, &buffer);
		glGenTextures(2, textures.data());
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glTranslatef(5.0f, 7.0f, 0.0f);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glEnable(GL_TEXTURE_2D);
		glClientActiveTexture(GL_TEXTURE2);
		// A host shader must be suspended for the fixed-function GUI pass.
		const GLuint program = glCreateProgram();
		const GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
		const char* source = "#version 120\nvoid main() { gl_FragColor = vec4(0.0); }";
		glShaderSource(shader, 1, &source, nullptr);
		glCompileShader(shader);
		glAttachShader(program, shader);
		glLinkProgram(program);
		GLint linked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);
		require(linked == GL_TRUE, "Host shader must link");
		glUseProgram(program);
		for (auto mode : rasterModes)
			glEnable(mode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDepthFunc(GL_NEVER);
		glStencilFunc(GL_NEVER, 7, 0x3f);
		glScissor(0, 0, 0, 0);
		glLogicOp(GL_XOR);
		glColorMask(GL_FALSE, GL_TRUE, GL_FALSE, GL_FALSE);
		glBlendFuncSeparate(GL_ZERO, GL_ONE, GL_ONE, GL_ZERO);
		glBlendEquationSeparate(GL_FUNC_REVERSE_SUBTRACT, GL_MAX);
		const auto saved = rasterState();
		fixture.capture();
		fixture.expectCorners({255, 255, 255, 255});
		require(rasterState() == saved, "GUI must restore combined host raster state and object bindings");
		resetRasterState();
		glDeleteBuffers(1, &buffer);
		glDeleteProgram(program);
		glDeleteShader(shader);
		glDeleteTextures(2, textures.data());

		fixture.scene(
			[](MyGUI::IRenderTarget* target)
			{
				if (target)
					throw std::runtime_error("deliberate render failure");
			});
		const auto beforeFailure = rasterState();
		bool threw = false;
		try
		{
			MyGUI::OpenGLRenderManager::getInstance().drawOneFrame();
		}
		catch (const std::runtime_error&)
		{
			threw = true;
		}
		require(threw && rasterState() == beforeFailure, "Failed GUI pass must restore host state");

		// Intentional debug state can still be applied after the GUI pass begins.
		fixture.scene(
			[&](MyGUI::IRenderTarget* target)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				fixture.quad(target, white);
			});
		const auto& capture = fixture.capture();
		size_t lit = 0;
		for (size_t i = 0; i < capture.rgba.size(); i += 4)
			if (capture.rgba[i] != 0)
				++lit;
		require(
			lit > 0 && lit < size_t(capture.width * capture.height) / 4,
			"Explicit GUI wireframe must remain possible");
		GLint polygon[2]{};
		glGetIntegerv(GL_POLYGON_MODE, polygon);
		require(polygon[0] == GL_FILL && polygon[1] == GL_FILL, "Debug wireframe must not leak into the host");
		fixture.resetCase();
	}

	void testStencilAttachment(platformtest::Fixture& fixture)
	{
		auto* white = platformtest::solid(fixture, {255, 255, 255, 255});
		auto* colour = fixture.texture();
		colour->createManual(16, 16, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
		GLuint framebuffer = 0, depthStencil = 0;
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			static_cast<MyGUI::OpenGLTexture*>(colour)->getTextureId(),
			0);
		glGenRenderbuffers(1, &depthStencil);
		glBindRenderbuffer(GL_RENDERBUFFER, depthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 16, 16);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthStencil);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencil);
		require(
			glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
			"Stencil test needs a complete framebuffer");
		glViewport(0, 0, 16, 16);
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NEVER, 0, ~0u);
		auto& renderer = MyGUI::OpenGLRenderManager::getInstance();
		renderer.begin();
		fixture.quad(&renderer, white);
		renderer.end();
		require(glIsEnabled(GL_STENCIL_TEST), "GUI must restore host stencil enable");
		std::array<unsigned char, 4> pixel{};
		glReadPixels(8, 8, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel.data());
		require(
			pixel == std::array<unsigned char, 4>{255, 255, 255, 255},
			"Host stencil must not suppress GUI pixels on a stencil framebuffer");
		glDisable(GL_STENCIL_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteFramebuffers(1, &framebuffer);
		glDeleteRenderbuffers(1, &depthStencil);
		glViewport(0, 0, 128, 128);
		require(glGetError() == GL_NO_ERROR, "Stencil isolation must not generate GL errors");
	}

	void testRttState(platformtest::Fixture& fixture)
	{
		GLuint hostDraw = 0, hostRead = 0;
		glGenFramebuffers(1, &hostDraw);
		glGenFramebuffers(1, &hostRead);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, hostDraw);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, hostRead);
		const auto checkBindings = [&]
		{
			GLint draw = 0, read = 0;
			glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw);
			glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read);
			require(
				GLuint(draw) == hostDraw && GLuint(read) == hostRead,
				"RTT must restore distinct host framebuffers");
		};
		auto* texture = fixture.texture();
		texture->createManual(16, 8, MyGUI::TextureUsage::RenderTarget, MyGUI::PixelFormat::R8G8B8A8);
		GLuint hostRenderbuffer = 0, hostTexture = 0;
		glGenRenderbuffers(1, &hostRenderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, hostRenderbuffer);
		glGenTextures(1, &hostTexture);
		glBindTexture(GL_TEXTURE_2D, hostTexture);
		auto* target = texture->getRenderTarget();
		checkBindings();
		GLint binding = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &binding);
		require(GLuint(binding) == hostTexture, "RTT construction must restore texture binding");
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &binding);
		require(GLuint(binding) == hostRenderbuffer, "RTT construction must restore renderbuffer binding");
		glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
		glViewport(3, 4, 51, 29);
		const auto state = rasterState();
		target->begin();
		target->end();
		checkBindings();
		require(rasterState() == state, "Standalone RTT must restore host raster state");
		std::array<GLint, 4> viewport{};
		std::array<GLfloat, 4> clear{};
		glGetIntegerv(GL_VIEWPORT, viewport.data());
		glGetFloatv(GL_COLOR_CLEAR_VALUE, clear.data());
		require(viewport == std::array<GLint, 4>{3, 4, 51, 29}, "RTT must restore host viewport");
		require(clear == std::array<GLfloat, 4>{0.25f, 0.5f, 0.75f, 1.0f}, "RTT must restore host clear colour");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &hostDraw);
		glDeleteFramebuffers(1, &hostRead);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glDeleteTextures(1, &hostTexture);
		glDeleteRenderbuffers(1, &hostRenderbuffer);
		glViewport(0, 0, 128, 128);
		require(glGetError() == GL_NO_ERROR, "RTT state restoration must not generate GL errors");
	}

}

int main()
{
	try
	{
		platformtest::Fixture fixture(false);
		fixture.open();
		fixture.capture(); // Includes initialization errors; do not drain GL errors.
		require(
			MyGUI::OpenGLRenderManager::getInstance().isPixelBufferObjectSupported(),
			"This native test requires a PBO-capable context");
		testTransfers(fixture);
		std::cout << "PASS PBO/CPU transfers and host pixel-store state\n";
		testInterleavedLocks(fixture);
		std::cout << "PASS interleaved PBO locks\n";
		testTransferFailures(fixture);
		std::cout << "PASS transfer validation and locked destruction\n";
		testRasterState(fixture);
		std::cout << "PASS raster isolation, restoration and explicit wireframe\n";
		testStencilAttachment(fixture);
		std::cout << "PASS stencil attachment isolation\n";
		testRttState(fixture);
		std::cout << "PASS standalone RTT state restoration\n";
		fixture.close();
		return 0;
	}
	catch (const std::exception& error)
	{
		std::cerr << "FAIL " << error.what() << '\n';
		return 1;
	}
}

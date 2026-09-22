#include "PlatformFixture.h"
#include "MyGUI_OpenGL3Texture.h"
#include <MyGUI_GL.h>
#include <algorithm>
#include <array>
#include <iostream>

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
		for (auto hint : {MyGUI::TextureUsage::Static, MyGUI::TextureUsage::Dynamic, MyGUI::TextureUsage::Stream})
		{
			auto* texture = fixture.texture();
			texture->createManual(3, 2, hint | MyGUI::TextureUsage::Write, MyGUI::PixelFormat::R8G8B8);
			host.check();
			std::vector<unsigned char> expected(18);
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
			bytes = static_cast<unsigned char*>(texture->lock(MyGUI::TextureUsage::Read | MyGUI::TextureUsage::Write));
			const bool preserved = std::equal(expected.begin(), expected.end(), bytes);
			bytes[17] = 23;
			expected[17] = 23;
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
		HostTransfers host;
		platformtest::checkInterleavedLocks(fixture, [&] { host.check(); });
	}

	constexpr std::array<GLenum, 7> rasterModes = {
		GL_BLEND,
		GL_CULL_FACE,
		GL_DEPTH_TEST,
		GL_STENCIL_TEST,
		GL_SCISSOR_TEST,
		GL_RASTERIZER_DISCARD,
		GL_COLOR_LOGIC_OP};

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
			  GL_VERTEX_ARRAY_BINDING,
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
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void testRasterState(platformtest::Fixture& fixture)
	{
		auto* white = platformtest::solid(fixture, {255, 255, 255, 255});
		fixture.scene([&](MyGUI::IRenderTarget* target) { fixture.quad(target, white); });
		for (auto mode : {GL_CULL_FACE, GL_DEPTH_TEST, GL_POLYGON_MODE})
		{
			resetRasterState();
			if (mode == GL_POLYGON_MODE)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glEnable(mode);
			glCullFace(GL_FRONT_AND_BACK);
			glDepthFunc(GL_NEVER);
			const auto saved = rasterState();
			fixture.capture();
			fixture.expectCorners({255, 255, 255, 255});
			require(rasterState() == saved, "GUI must restore each hostile raster state");
		}
		resetRasterState();
		GLuint vao = 0, buffer = 0;
		std::array<GLuint, 2> textures{};
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &buffer);
		glGenTextures(2, textures.data());
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glUseProgram(MyGUI::OpenGL3RenderManager::getInstance().getShaderProgramId("Default"));
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
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &buffer);
		glDeleteTextures(2, textures.data());

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
		auto* target = texture->getRenderTarget();
		checkBindings();
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
		require(MyGUI::OpenGL3RenderManager::getInstance().isPixelBufferObjectSupported(), "OpenGL3 must support PBOs");
		testTransfers(fixture);
		std::cout << "PASS PBO/CPU transfers and host pixel-store state\n";
		testInterleavedLocks(fixture);
		std::cout << "PASS interleaved PBO locks\n";
		testRasterState(fixture);
		std::cout << "PASS raster isolation, restoration and explicit wireframe\n";
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

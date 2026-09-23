#include "CanvasTestSupport.h"
#include "TestRunner.h"

namespace
{

	using unittest::require;
	void testGrowthAndCallbacks()
	{
		unittest::CanvasFixture fixture;
		auto* canvas = fixture.canvas;
		canvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_REQUESTED);
		require(
			canvas->getTextureRealSize() == MyGUI::IntSize(128, 64),
			"Requested dimensions must round up to powers of two");
		require(
			canvas->getTextureSrcSize() == MyGUI::IntSize(65, 33),
			"Source size must retain exact requested dimensions");
		require(
			fixture.updates.size() == 1 && fixture.updates.back().textureChanged,
			"Creation must notify the new texture");
		canvas->setSize(200, 90);
		canvas->setSize(260, 130);
		canvas->updateTexture();
		require(fixture.renderer.created == 1, "Resize must defer allocation until the next frame");
		fixture.frame();
		require(
			canvas->getTextureRealSize() == MyGUI::IntSize(512, 256) && fixture.renderer.created == 2 &&
				fixture.preChanges == 1,
			"Multiple pending resizes must coalesce into one replacement");
		require(fixture.updates.back().requested, "Reallocation must preserve an explicit content update request");
		canvas->setSize(40, 30);
		fixture.frame();
		require(
			fixture.renderer.created == 2 && !fixture.updates.back().textureChanged,
			"Shrinking must reuse the existing allocation");
		canvas->updateTexture();
		canvas->updateTexture();
		const auto events = fixture.updates.size();
		fixture.frame();
		require(
			fixture.updates.size() == events + 1 && fixture.updates.back().requested,
			"Repeated update requests must coalesce");
		fixture.frame();
		require(fixture.updates.size() == events + 1, "Idle frames must not repeat update callbacks");
		fixture.renderer.render();
		require(fixture.renderer.drawnVertices == 6, "A visible Canvas must draw its texture quad");
		fixture.finish();
	}

	void testModesPixelsAndLifetime()
	{
		unittest::CanvasFixture fixture;
		auto* canvas = fixture.canvas;
		canvas->createTexture(MyGUI::IntSize(31, 17), MyGUI::Canvas::TRM_PT_CONST_SIZE);
		require(canvas->getTextureRealSize() == MyGUI::IntSize(32, 32), "Constant texture size must round up");
		canvas->setSize(400, 300);
		fixture.frame();
		require(canvas->getTextureRealSize() == MyGUI::IntSize(32, 32), "Constant mode must not grow with the widget");
		auto* pixels = static_cast<unsigned char*>(canvas->lock(MyGUI::TextureUsage::Write));
		require(canvas->isLocked(), "Canvas must forward texture lock state");
		pixels[0] = 123;
		canvas->unlock();
		require(!canvas->isLocked(), "Unlock must clear texture lock state");
		pixels = static_cast<unsigned char*>(canvas->lock(MyGUI::TextureUsage::Read));
		require(pixels[0] == 123, "Pixels must survive unlock and relock");
		canvas->unlock();
		canvas->setResizeMode(MyGUI::Canvas::TRM_PT_VIEW_ALL);
		canvas->setSize(129, 65);
		fixture.frame();
		require(canvas->getTextureRealSize() == MyGUI::IntSize(256, 128), "Changing resize mode must permit growth");
		auto* texture = static_cast<unittest::CanvasTexture*>(canvas->getTexture());
		require(texture->listener != nullptr, "Canvas must subscribe to invalidation");
		texture->listener->textureInvalidate(texture);
		fixture.frame();
		require(fixture.updates.back().requested, "Texture invalidation must request a content update");
		canvas->destroyTexture();
		require(
			!canvas->isTextureCreated() && fixture.renderer.textures.empty(),
			"Explicit destruction must release ownership");
		canvas->createTexture(0, 0, MyGUI::Canvas::TRM_PT_CONST_SIZE);
		require(canvas->getTextureRealSize() == MyGUI::IntSize(1, 1), "Zero texture dimensions must clamp to one");
		canvas->updateTexture();
		fixture.finish();
	}

	void testDestroyPendingUpdate()
	{
		unittest::CanvasFixture fixture;
		fixture.canvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_REQUESTED);
		fixture.canvas->setSize(300, 200);
		fixture.canvas->updateTexture();
		const auto updates = fixture.updates.size();
		fixture.canvas->destroyTexture();
		fixture.frame();
		require(
			!fixture.canvas->isTextureCreated() && fixture.renderer.textures.empty(),
			"Pending resize must not resurrect a destroyed texture");
		require(fixture.updates.size() == updates, "Destroying a texture must cancel queued update callbacks");
		fixture.renderer.render();
		fixture.canvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_ALL);
		require(
			fixture.canvas->getTextureRealSize() == MyGUI::IntSize(512, 256),
			"Canvas must support creation after cancellation");
		fixture.finish();
	}

	void testDisableManagedResize()
	{
		unittest::CanvasFixture fixture;
		fixture.canvas->createTexture(MyGUI::Canvas::TRM_PT_VIEW_ALL);
		fixture.canvas->setSize(300, 200);
		fixture.canvas->setTextureManaged(false);
		fixture.frame();
		require(
			fixture.canvas->getTextureRealSize() == MyGUI::IntSize(128, 64) && !fixture.canvas->isTextureManaged(),
			"Disabling management must prevent a pending resize from replacing the texture");
		fixture.canvas->setTextureManaged(true);
		fixture.canvas->updateTexture();
		fixture.frame();
		require(
			fixture.canvas->getTextureRealSize() == MyGUI::IntSize(512, 256),
			"Re-enabled management must allow requested growth");
		fixture.finish();
	}

}

int main()
{
	return unittest::runTests({
		{"Growth and callbacks", testGrowthAndCallbacks},
		{"Modes, pixels and lifetime", testModesPixelsAndLifetime},
		{"Destroy with pending update", testDestroyPendingUpdate},
		{"Disable pending managed resize", testDisableManagedResize},
	});
}

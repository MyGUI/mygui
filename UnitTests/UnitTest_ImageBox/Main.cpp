/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI.h"
#include "MyGUI_SubSkin.h"
#include "SkinTestContext.h"
#include "BehaviourTestSupport.h"

namespace imagebox_test
{

	using unittest::require;
	const std::string atlas = "MyGUI_BlueWhiteSkins.png";

	// Expose the UVs received by the real skin without changing ImageBox's behavior.
	class ObservedSubSkin : public MyGUI::SubSkin
	{
		MYGUI_RTTI_DERIVED(ObservedSubSkin)

	public:
		const MyGUI::FloatRect& getUV() const
		{
			return mRectTexture;
		}
	};

	struct Fixture
	{
		Fixture()
		{
			MyGUI::LayerManager::getInstance().createLayerAt("Main", "OverlappedLayer", 0);
			MyGUI::FactoryManager::getInstance().registerFactory<ObservedSubSkin>(
				MyGUI::SubWidgetManager::getInstance().getCategoryName());
			unittest::loadResources("UnitTest_ImageBox/TestResources.xml");
		}

		unittest::SkinTestContext context;
	};

	static MyGUI::ImageBox* createImage()
	{
		return MyGUI::Gui::getInstance().createWidget<MyGUI::ImageBox>(
			"TestImageBox",
			MyGUI::IntCoord(0, 0, 32, 32),
			MyGUI::Align::Default,
			"Main");
	}

	static void checkImage(MyGUI::ImageBox* _image, const MyGUI::IntCoord& _coord)
	{
		require(_image->_getTextureName() == atlas, "A selected image must use its atlas texture");
		const auto size = _image->getImageSize();
		require(size.width > 0 && size.height > 0, "The atlas must have valid dimensions");
		const MyGUI::FloatRect expected(
			float(_coord.left) / size.width,
			float(_coord.top) / size.height,
			float(_coord.right()) / size.width,
			float(_coord.bottom()) / size.height);
		const auto& actual = _image->getSubWidgetMain()->castType<ObservedSubSkin>()->getUV();
		require(
			std::abs(actual.left - expected.left) < 0.00001f && std::abs(actual.top - expected.top) < 0.00001f &&
				std::abs(actual.right - expected.right) < 0.00001f &&
				std::abs(actual.bottom - expected.bottom) < 0.00001f,
			"Selected image UVs must match the expected atlas rectangle");
	}

	static void testTiles()
	{
		Fixture fixture;
		auto* image = createImage();
		require(
			image->getItemCount() == 0 && image->getImageIndex() == MyGUI::ITEM_NONE,
			"A new ImageBox must have no tiles or selection");
		image->setImageTexture(atlas);
		checkImage(image, MyGUI::IntCoord(MyGUI::IntPoint(), image->getImageSize()));
		image->setImageInfo(atlas, MyGUI::IntCoord(8, 16, 48, 32), MyGUI::IntSize(16, 16));
		require(image->getItemCount() == 6, "A three by two tile region must produce six items");
		for (size_t index = 0; index < 6; ++index)
		{
			image->setImageIndex(index);
			require(image->getItemSelect() == index, "Simple and expanded selection APIs must agree");
			checkImage(image, MyGUI::IntCoord(8 + int(index % 3) * 16, 16 + int(index / 3) * 16, 16, 16));
		}
		image->resetItemSelect();
		require(
			image->getItemSelect() == MyGUI::ITEM_NONE && image->_getTextureName().empty(),
			"Resetting selection must hide the image");
		image->setItemSelect(2);
		checkImage(image, MyGUI::IntCoord(40, 16, 16, 16));
		image->setImageRect(MyGUI::IntRect(16, 32, 48, 64));
		require(image->getItemCount() == 4, "Changing the rectangle must rebuild the tile grid");
		checkImage(image, MyGUI::IntCoord(16, 48, 16, 16));
		image->setProperty("ImageTexture", atlas);
		image->setProperty("ImageCoord", "0 0 32 16");
		image->setProperty("ImageTile", "8 8");
		image->setProperty("ImageIndex", "7");
		require(image->getItemCount() == 8, "Image properties must rebuild and select the tile grid");
		checkImage(image, MyGUI::IntCoord(24, 8, 8, 8));
		image->setImageInfo(atlas, MyGUI::IntCoord(0, 0, 35, 19), MyGUI::IntSize(16, 16));
		require(image->getItemCount() == 2, "Partial tiles must be excluded");
		image->setImageTile(MyGUI::IntSize(0, 16));
		require(image->getItemCount() == 0 && image->_getTextureName().empty(), "Zero tile width must clear the grid");
		image->setImageInfo(atlas, MyGUI::IntCoord(0, 0, 16, 16), MyGUI::IntSize(1, 1));
		require(image->getItemCount() == 256, "The maximum supported tile count must be accepted");
		image->setImageCoord(MyGUI::IntCoord(0, 0, 17, 16));
		require(image->getItemCount() == 0, "Oversized tile grids must be rejected");
		MyGUI::Gui::getInstance().destroyWidget(image);

		image = createImage();
		image->setImageTexture(atlas);
		image->setImageCoord(MyGUI::IntCoord(8, 16, 20, 30));
		require(
			image->getItemCount() == 1 && image->getImageIndex() == 0,
			"Initial coordinates must define one selected tile");
		checkImage(image, MyGUI::IntCoord(8, 16, 20, 30));
		MyGUI::Gui::getInstance().destroyWidget(image);
	}

	static void testItems()
	{
		Fixture fixture;
		auto* image = createImage();
		image->setImageTexture(atlas);
		image->addItem(MyGUI::IntCoord(0, 0, 16, 16));
		image->addItem(MyGUI::IntCoord(16, 0, 16, 16));
		image->addItem(MyGUI::IntCoord(32, 0, 16, 16));
		image->setItemSelect(1);
		image->insertItem(0, MyGUI::IntCoord(48, 0, 16, 16));
		require(
			image->getItemCount() == 4 && image->getItemSelect() == 2,
			"Inserting before selection must preserve the selected item");
		checkImage(image, MyGUI::IntCoord(16, 0, 16, 16));
		image->deleteItem(0);
		require(image->getItemSelect() == 1, "Deleting before selection must shift its index back");
		checkImage(image, MyGUI::IntCoord(16, 0, 16, 16));
		image->deleteItem(1);
		require(image->getItemSelect() == 1, "Deleting a selected middle item must select its successor");
		checkImage(image, MyGUI::IntCoord(32, 0, 16, 16));
		image->setItem(1, MyGUI::IntCoord(64, 32, 8, 8));
		checkImage(image, MyGUI::IntCoord(64, 32, 8, 8));
		image->deleteItem(1);
		require(image->getItemSelect() == 0, "Deleting the selected last item must select its predecessor");
		checkImage(image, MyGUI::IntCoord(0, 0, 16, 16));
		image->deleteItem(0);
		require(
			image->getItemCount() == 0 && image->getItemSelect() == MyGUI::ITEM_NONE &&
				image->_getTextureName().empty(),
			"Deleting the final item must clear selection and texture");
		image->deleteAllItems();
		MyGUI::Gui::getInstance().destroyWidget(image);
	}

	static void testAnimation()
	{
		Fixture fixture;
		auto& gui = MyGUI::Gui::getInstance();
		auto* image = createImage();
		image->setImageTexture(atlas);
		const MyGUI::IntCoord first(0, 0, 16, 16), second(16, 0, 16, 16), third(32, 0, 16, 16);
		image->addItem(first);
		image->addItemFrame(0, second);
		image->insertItemFrame(0, 1, third);
		image->setItemFrame(0, 1, second);
		image->deleteItemFrame(0, 2);
		image->addItemFrameDuplicate(0, 0);
		image->insertItemFrameDuplicate(0, 1, 1);
		// The resulting sequence is first, second, second, first.
		image->setItemFrameRate(0, 0.25f);
		require(image->getItemFrameRate(0) == 0.25f, "Frame duration must round-trip");
		image->setItemSelect(0);
		checkImage(image, first);
		gui.eventFrameStart(0.125f);
		checkImage(image, first);
		gui.eventFrameStart(0.125f);
		checkImage(image, second);
		gui.eventFrameStart(0.5f);
		checkImage(image, first);
		gui.eventFrameStart(0.5f);
		checkImage(image, second);
		image->setItemFrameRate(0, 0);
		gui.eventFrameStart(1.0f);
		checkImage(image, second);
		image->resetItemSelect();
		gui.eventFrameStart(1.0f);
		require(image->_getTextureName().empty(), "Unselected animations must remain hidden");
		image->deleteAllItemFrames(0);
		image->addItemFrame(0, third);
		image->setItemSelect(0);
		checkImage(image, third);
		gui.eventFrameStart(1.0f);
		checkImage(image, third);
		image->setItemResourceInfo(
			MyGUI::ImageIndexInfo{
				atlas,
				MyGUI::IntSize(16, 16),
				0.25f,
				{MyGUI::IntPoint(0, 0), MyGUI::IntPoint(16, 0)}});
		image->deleteAllItems();
		gui.eventFrameStart(1.0f);
		require(image->getItemSelect() == MyGUI::ITEM_NONE, "Clearing animated items must leave no selection");
		image->setItemResourceInfo(
			MyGUI::ImageIndexInfo{
				atlas,
				MyGUI::IntSize(16, 16),
				0.25f,
				{MyGUI::IntPoint(0, 0), MyGUI::IntPoint(16, 0)}});
		gui.destroyWidget(image);
		gui.eventFrameStart(1.0f);
	}

	static void testResources()
	{
		Fixture fixture;
		auto* image = createImage();
		require(!image->setItemResource("MissingImages"), "Missing resources must return false");
		require(image->setItemResource("TestImages"), "An existing image resource must load");
		auto* resource = image->getItemResource();
		require(resource != nullptr && image->getItemCount() == 1, "The resource must supply one image item");
		checkImage(image, MyGUI::IntCoord(8, 16, 16, 16));
		image->setProperty("ImageName", "Animated");
		MyGUI::Gui::getInstance().eventFrameStart(0.25f);
		checkImage(image, MyGUI::IntCoord(24, 16, 16, 16));
		image->setItemName("Idle");
		image->setProperty("ImageGroup", "Large");
		checkImage(image, MyGUI::IntCoord(32, 48, 32, 32));
		image->setItemResourceInfo(resource, "Small", "Idle");
		checkImage(image, MyGUI::IntCoord(8, 16, 16, 16));
		image->setItemGroup("");
		require(image->_getTextureName().empty(), "An empty resource group must hide the image");
		image->setItemGroup("Small");
		checkImage(image, MyGUI::IntCoord(8, 16, 16, 16));
		image->setItemName("Missing");
		require(
			image->getItemCount() == 0 && image->_getTextureName().empty(),
			"Missing resource names must clear the image");
		image->setItemName("Idle");
		image->setItemResourcePtr(nullptr);
		require(
			image->getItemResource() == nullptr && image->_getTextureName().empty(),
			"Clearing the resource must hide the image");
		image->setProperty("ImageResource", "TestImages");
		checkImage(image, MyGUI::IntCoord(8, 16, 16, 16));
		MyGUI::Gui::getInstance().destroyWidget(image);
	}

}

int main()
{
	return unittest::runTests({
		{"Tiles", imagebox_test::testTiles},
		{"Items", imagebox_test::testItems},
		{"Animation", imagebox_test::testAnimation},
		{"Resources", imagebox_test::testResources},
	});
}

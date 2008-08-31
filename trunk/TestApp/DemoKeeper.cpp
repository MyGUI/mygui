/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "DemoKeeper.h"

namespace demo
{

	void DemoKeeper::createScene()
	{

		using namespace MyGUI;
		const IntSize & view = Gui::getInstance().getViewSize();
		const IntSize size(200, 200);

		base::BaseManager::getInstance().addResourceLocation("../../Media/TestApp");
		base::BaseManager::getInstance().setWallpaper("wallpaper4.jpg");

		EditPtr edit = Gui::getInstance().createWidget<Edit>("Memo", IntCoord((view.width - size.width) / 2, (view.height - size.height) / 2, size.width, size.height), Align::Default, "Main");
		edit->setCaption("seig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshs seig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshsseig iudy gisudh giluehdi fughdhg lskdhf lg hdlIHd ILUHds lHs lhG Lshs");
	}

	void DemoKeeper::destroyScene()
	{
	}

} // namespace demo

/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
/*
требование и мотивация:

виджет сий, будет называться Canvas и суть его отображенна в названии.
варианты использования, для внешнего программиста, всего два

1) я маленький даун, я хочу виджет в котором я могу попиксельно рисовать то что я хочу и когда захочу.
2) я хочу прикрутить нечто что рисует в текстуру примитивы или еще что либо.

соответсвенно реализация обновления двух видов:

1) есть некое кеширование, опциональное, пользователь лочит текстурку пишет туда попиксельно или как захочет и анлочит и все радуются. 
  но тут проблема в том что если не будет кеширования то все пропадет при девайс лост что он намалевал.
  для вариантов видео или рендер бокса не страшно, оно и так обновляется перфрейм.

2) есть некая библиотека, котоая управляет примитивами, ей нужен указатель на некую память и ее размеры, и все, она сама там что то делает
  и без кеширования ибо рулит примитивами. ей нужен метод апдейт, в который она и выводит все разом с нуля.

отсюда выходит интерфейс следующего типа.
  создать виджет, указав ему только размеры, не надо напрягать пользователя деталями, т.е. никакх имен, зачем ему нужно имя текстуры?
  зачем группа зачем формат, он хочет тыкать пиксели. т.е. обязательно только размер и то не факт =)
  методы для указания нужно ли кеширование
  методы сет пиксель
  методы лок анлок
  событие апдейта

для наследования должны быть методы апдейта без евента, т.е. если я библиотека, я хочу создать текстуру
и в вирт методе апдейт все самому ресовать а не снаружи

виджет Canvas должен иметь флаги, такие как режим растягивания
1) созданная текстура не меняет размер, она расятгивается
2) созданая текстура есть пиксель в пиксель, соответвенно пересоздается как только виджет увеличился и его рамера не достаточно
   правда об этом мона подумать в производных классах

   ниже будет два варианта использования виджетов
	F1 управление первой текстурой
	F2 управление второй текстурой
*/

namespace demo
{

	void DemoKeeper::createScene()
    {
		// потемнее скин
		mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

		// первая мета текстура
		// мы по евенту лочим и добавляем в текстуру данные и все
		mTexture1 = new MyGUI::Texture( MyGUI::IntSize(256, 256), Ogre::PF_R8G8B8A8, "mTexture1", "General" );
		MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("Panel", MyGUI::IntCoord(10, 10, 256, 256), MyGUI::Align::Default, "Overlapped");
		mImage1 = widget->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, 256, 256), MyGUI::Align::Stretch);
		mImage1->setImageTexture(mTexture1->getName());

		// фтарая мета текстура
		// мы запоминаем примитив(ы) для рендера и говорим текстуре что данные обновились
		// а данные обновляем в методу апдейт
		mTexture2 = new MyGUI::Texture( MyGUI::IntSize(256, 256), Ogre::PF_R8G8B8A8, "mTexture2", "General" );
		mTexture2->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture2 );
		widget = mGUI->createWidget<MyGUI::Widget>("Panel", MyGUI::IntCoord(310, 10, 256, 256), MyGUI::Align::Default, "Overlapped");
		mImage2 = widget->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, 256, 256), MyGUI::Align::Stretch);
		mImage2->setImageTexture(mTexture2->getName());
	}

    void DemoKeeper::destroyScene()
    {
    }

	void DemoKeeper::requestUpdateTexture2( MyGUI::TexturePtr texture )
    {
		mTexture2->lock();
		for (VectorPintInfo::const_iterator iter = mPaintData.begin(); iter!=mPaintData.end(); ++iter) {
			const PaintInfo& data = *iter;
			mTexture2->setPixel(data.x, data.y, data.colour);
		}
		mTexture2->unlock();
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		bool result = BaseManager::keyPressed(arg);

		if(arg.key == OIS::KC_F1) {

			// почему то данные сбрасываются при каждом локе =/

			mTexture1->lock();

			size_t count = rand() % 64;
			while (count != 0) {
				mTexture1->setPixel(
					rand() % 256, rand() % 256,
					Ogre::ColourValue(
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255)));
				count --;
			}

			mTexture1->unlock();

		}
		else if( arg.key == OIS::KC_F2) {

			size_t count = rand() % 64;
			while (count != 0) {
				PaintInfo info;
				info.x = rand() % 256;
				info.y = rand() % 256;
				info.colour = Ogre::ColourValue(
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255));

				mPaintData.push_back(info);
				count --;
			}

			// и говорим что данные устарели,
			// по идее евент придет на границе кадра а не сразу
			mTexture2->updateData();
		}

		return result;
	}
	 
} // namespace demo

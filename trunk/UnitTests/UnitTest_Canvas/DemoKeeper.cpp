/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "precompiled.h"
#include "DemoKeeper.h"
/*
���������� � ���������:

������ ���, ����� ���������� Canvas � ���� ��� ����������� � ��������.
�������� �������������, ��� �������� ������������, ����� ���

1) � ��������� ����, � ���� ������ � ������� � ���� ����������� �������� �� ��� � ���� � ����� ������.
2) � ���� ���������� ����� ��� ������ � �������� ��������� ��� ��� ��� ����.

������������� ���������� ���������� ���� �����:

1) ���� ����� �����������, ������������, ������������ ����� ��������� ����� ���� ����������� ��� ��� ������� � ������� � ��� ��������. 
  �� ��� �������� � ��� ��� ���� �� ����� ����������� �� ��� �������� ��� ������ ���� ��� �� ���������.
  ��� ��������� ����� ��� ������ ����� �� �������, ��� � ��� ����������� ��������.

2) ���� ����� ����������, ������ ��������� �����������, �� ����� ��������� �� ����� ������ � �� �������, � ���, ��� ���� ��� ��� �� ������
  � ��� ����������� ��� ����� �����������. �� ����� ����� ������, � ������� ��� � ������� ��� ����� � ����.
Re: �) �� � ����� ����� ������ ���� ��������... �� ���� ��������� Canvas, ���� ��� �����/������� � ������ ����� ������ ��� �����. ����� ��� �����
    ��� � ����� � ����� ������...
	�) 


������ ������� ��������� ���������� ����.
  ������� ������, ������ ��� ������ �������, �� ���� ��������� ������������ ��������, �.�. ������ ����, ����� ��� ����� ��� ��������?
  ����� ������ ����� ������, �� ����� ������ �������. �.�. ����������� ������ ������ � �� �� ���� =)
  ������ ��� �������� ����� �� �����������
  ������ ��� �������
  ������ ��� �����
  ������� �������

��� ������������ ������ ���� ������ ������� ��� ������, �.�. ���� � ����������, � ���� ������� ��������
� � ���� ������ ������ ��� ������ �������� � �� �������

������ Canvas ������ ����� �����, ����� ��� ����� ������������
1) ��������� �������� �� ������ ������, ��� �������������
2) �������� �������� ���� ������� � �������, ������������ ������������� ��� ������ ������ ���������� � ��� ������ �� ����������
   ������ �� ���� ���� �������� � ����������� �������

   ���� ����� ��� �������� ������������� ��������
	F1 ���������� ������ ���������
	F2 ���������� ������ ���������
*/

namespace demo
{

	void DemoKeeper::createScene()
    {
		// �������� ����
		mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

		mCanvas1Size = 260;
		mCanvas2Size = 260;
		mCanvas3Size = 512;

		//base::BaseManager::getInstance().addResourceLocation("../../Media/Common/Wallpapers");
		//base::BaseManager::getInstance().setWallpaper("wallpaper0.jpg");

		mCanvasFactory = new MyGUI::factory::CanvasFactory();

		// ������ ���� ��������
		// �� �� ������ ����� � ��������� � �������� ������ � ���
		// Re: ��� ����
		mPanel1 = mGUI->createWidget<MyGUI::Widget>("Panel", MyGUI::IntCoord(10, 10, mCanvas1Size, mCanvas1Size), MyGUI::Align::Default, "Overlapped");
		mCanvas1 = mPanel1->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(0, 0, mCanvas1Size, mCanvas1Size), MyGUI::Align::LeftTop);
		mCanvas1->createTexture(/* mCanvas1Size, mCanvas1Size */); // ������ ����� ��, ��� �������
		mCanvas1->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture1 );

		//MyGUI::StaticImagePtr image1 = mPanel1->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, mCanvas1Size, mCanvas1Size), MyGUI::Align::Stretch);
		//image1->setImageTexture( mCanvas1->getName() );

		// Re: ���
		mPanel2 = mGUI->createWidget<MyGUI::Widget>("Panel", MyGUI::IntCoord(310, 10, mCanvas2Size, mCanvas2Size), MyGUI::Align::Default, "Overlapped");
		mCanvas2 = mPanel2->createWidget< MyGUI::Canvas >( "Canvas", MyGUI::IntCoord(0, 0, mCanvas2Size, mCanvas2Size), MyGUI::Align::LeftTop);
		mCanvas2->setCacheUse( true );
		mCanvas2->createTexture(/* mCanvas2Size, mCanvas2Size */); // �������� � ��������� ������� ������ - ������ ��� �� ������ �������
		mCanvas2->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture2 );
		
		// ������ ���� ��������
		// �� ���������� ��������(�) ��� ������� � ������� �������� ��� ������ ����������
		// � ������ ��������� � ������ ������
		// Re: ���� ��� - ���������
		mPanel3 = mGUI->createWidget<MyGUI::Widget>("Panel", MyGUI::IntCoord(610, 10, mCanvas3Size, mCanvas3Size), MyGUI::Align::Default, "Overlapped");
		mCanvas3 = mPanel3->createWidget< MyGUI::Canvas >("Canvas", MyGUI::IntCoord(0, 0, mCanvas3Size, mCanvas3Size), MyGUI::Align::LeftTop);
		mCanvas3->createTexture(/* mCanvas3Size, mCanvas3Size */);
		mCanvas3->requestUpdateTexture = MyGUI::newDelegate( this, &DemoKeeper::requestUpdateTexture3 );
	}	

    void DemoKeeper::destroyScene()
    {
		delete mCanvasFactory;
    }

	void DemoKeeper::requestUpdateTexture1( MyGUI::CanvasPtr canvas )
    {
		//canvas->loadFromFile( "wallpaper0.jpg" );
	}

	// Load from cache
	void DemoKeeper::requestUpdateTexture2( MyGUI::CanvasPtr canvas )
    {
		if( canvas->getCacheUse() && ! canvas->isCacheEmpty() )
		{
			// Re: ��-��������� � ����� ������� ����.. ����� ������ � ������ ����, ���� ����������
			// - ��� RM_EXACT_REQUEST
			if( canvas->getResizeMode() == MyGUI::Canvas::RM_EXACT_REQUEST )
				canvas->restoreFromCache();
			else // �� ���� ������... ����� ��� =) ... � ��������� ��������� ��������
				canvas->restoreFromCacheResampled( Ogre::Image::Box( 0, 0, canvas->getTextureRealWidth(), canvas->getTextureRealHeight() ), Ogre::Image::FILTER_BILINEAR );
		}
	}

	// Primitives used 
	void DemoKeeper::requestUpdateTexture3( MyGUI::CanvasPtr canvas )
    {
		canvas->lock();
		for (VectorPaintInfo::const_iterator iter = mPaintData.begin(); iter!=mPaintData.end(); ++iter) {
			const PaintInfo& data = *iter;
			canvas->setPixel(data.x, data.y, data.colour);
		}
		canvas->unlock();
	}

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		bool result = BaseManager::keyPressed(arg);

		const Ogre::Real plusSize = 20;

		if(arg.key == OIS::KC_F1) {

			// ������ �� ������ ������������ ��� ������ ���� =/
			// Re: ���

			mCanvas1->lock();

			size_t count = rand() % 64;
			while (count != 0) {
				mCanvas1->setPixel(
					rand() % 256, rand() % 256,
					Ogre::ColourValue(
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255)));
				count --;
			}

			mCanvas1->unlock();

		} else if( arg.key == OIS::KC_F2) {
			mCanvas2->lock();

			// Re: ��� �������� - ��� ��������� :)

			size_t count = rand() % 64;
			while (count != 0) {
				mCanvas2->setPixel(
					rand() % 256, rand() % 256,
					Ogre::ColourValue(
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255),
					rand()%256 / float(255)));
				count --;
			}

			mCanvas2->unlock();
			
		} else if( arg.key == OIS::KC_F3) {

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

			// � ������� ��� ������ ��������,
			// �� ���� ����� ������ �� ������� ����� � �� �����
			// Re: �� ����� ���������� ����������� :)
			mCanvas3->updateTexture();
		/* bigger */
		}  else if(arg.key == OIS::KC_Q) {
			mCanvas1Size += plusSize;
			mCanvas1->setSize( mCanvas1Size, mCanvas1Size );
			/*mCanvas2->setPosition( mCanvas2->getPosition().left + plusSize,
								  mCanvas2->getPosition().top );

			mCanvas3->setPosition( mCanvas3->getPosition().left + plusSize,
								  mCanvas3->getPosition().top );*/
			mPanel1->setSize( mCanvas1Size, mCanvas1Size );

			mPanel2->setPosition( mPanel2->getPosition().left + plusSize,
								  mPanel2->getPosition().top );

			mPanel3->setPosition( mPanel3->getPosition().left + plusSize,
								  mPanel3->getPosition().top );
		} else if(arg.key == OIS::KC_W) {
			mCanvas2Size += plusSize;
			mCanvas2->setSize( mCanvas2Size, mCanvas2Size );
			/*mCanvas3->setPosition( mCanvas3->getPosition().left + plusSize,
								  mCanvas3->getPosition().top );*/
			mPanel2->setSize( mCanvas2Size, mCanvas2Size );

			mPanel3->setPosition( mPanel3->getPosition().left + plusSize,
								  mPanel3->getPosition().top );
		} else if(arg.key == OIS::KC_E) {
			mCanvas3Size += plusSize;
			mCanvas3->setSize( mCanvas3Size, mCanvas3Size );
			mPanel3->setSize( mCanvas3Size, mCanvas3Size );
		// smaller
		} else if(arg.key == OIS::KC_A) {
			mCanvas1Size -= plusSize;
			mCanvas1->setSize( mCanvas1Size, mCanvas1Size );

			/*mCanvas2->setPosition( mCanvas2->getPosition().left - plusSize,
								  mCanvas2->getPosition().top );

			mCanvas3->setPosition( mCanvas3->getPosition().left - plusSize,
								  mCanvas3->getPosition().top );*/
			mPanel1->setSize( mCanvas1Size, mCanvas1Size );

			mPanel2->setPosition( mPanel2->getPosition().left - plusSize,
								  mPanel2->getPosition().top );

			mPanel3->setPosition( mPanel3->getPosition().left - plusSize,
								  mPanel3->getPosition().top );
		} else if(arg.key == OIS::KC_S) {
			mCanvas2Size -= plusSize;
			mCanvas2->setSize( mCanvas2Size, mCanvas2Size );

			/*mCanvas2->setPosition( mCanvas3->getPosition().left - plusSize,
								  mCanvas3->getPosition().top );*/
			mPanel2->setSize( mCanvas2Size, mCanvas2Size );

			mPanel3->setPosition( mPanel3->getPosition().left - plusSize,
								  mPanel3->getPosition().top );
		} else if(arg.key == OIS::KC_D) {
			mCanvas3Size -= plusSize;
			mCanvas3->setSize( mCanvas3Size, mCanvas3Size );
			mPanel3->setSize( mCanvas3Size, mCanvas3Size );
		}


		return result;
	}
	 
} // namespace demo

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

		// ������ ���� ��������
		// �� �� ������ ����� � ��������� � �������� ������ � ���
		mTexture1 = new MyGUI::Texture( MyGUI::IntSize(256, 256), Ogre::PF_R8G8B8A8, "mTexture1", "General" );
		MyGUI::WidgetPtr widget = mGUI->createWidget<MyGUI::Widget>("Panel", MyGUI::IntCoord(10, 10, 256, 256), MyGUI::Align::Default, "Overlapped");
		mImage1 = widget->createWidget<MyGUI::StaticImage>("StaticImage", MyGUI::IntCoord(0, 0, 256, 256), MyGUI::Align::Stretch);
		mImage1->setImageTexture(mTexture1->getName());

		// ������ ���� ��������
		// �� ���������� ��������(�) ��� ������� � ������� �������� ��� ������ ����������
		// � ������ ��������� � ������ ������
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

			// ������ �� ������ ������������ ��� ������ ���� =/

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

			// � ������� ��� ������ ��������,
			// �� ���� ����� ������ �� ������� ����� � �� �����
			mTexture2->updateData();
		}

		return result;
	}
	 
} // namespace demo

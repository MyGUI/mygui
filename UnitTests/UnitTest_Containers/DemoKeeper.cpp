/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

#include "precompiled.h"
#include "DemoKeeper.h"

namespace demo
{
	bool DemoKeeper::frameStarted( const Ogre::FrameEvent & evt )
	{
		bool res = BaseManager::frameStarted( evt );


		return res;
	}

	void DemoKeeper::createScene()
    {
		// I LOVE IT :)
		mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

		
		mFlowContainerFactory = new MyGUI::factory::FlowContainerFactory();
		mSpacerFactory = new MyGUI::factory::SpacerFactory();

		Ogre::String m = mFlowContainerFactory->getTypeName();

		MyGUI::WindowPtr window = mGUI->createWidget< MyGUI::Window >( "WindowCS", MyGUI::IntCoord( 0, 0, 300, 300 ), MyGUI::Align::Default, "Overlapped" );

		MyGUI::FlowContainerPtr flow = window->createWidget< MyGUI::FlowContainer >( "FlowContainer", MyGUI::IntCoord( 0, 0, 300, 300 ), MyGUI::Align::HStretch | MyGUI::Align::VStretch, "Overlapped" );
		
		/*for( int i = 0; i < 20; ++i )
		{
			MyGUI::ButtonPtr b = flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntSize( 70, 50 ), MyGUI::Align::Default );
			b->setCaption( Ogre::UTFString( L"Button " ) + Ogre::UTFString( Ogre::StringConverter::toString( i+1 ) ) );
			mButtons.push_back( b ); 

			flow->addSpacer( MyGUI::IntSize( 10, 10 ) );

			if( i % 4 == 3 )
				flow->addLineBreak( 10 );
		}

		mGUI->destroyWidget( mButtons[ 3 ] );*/


		flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntSize( 70, 50 ), MyGUI::Align::Default )
			->setCaption( "Button1" );


		flow->addSpacer( MyGUI::FloatSize( 1, 0 ) );

		flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntSize( 70, 50 ), MyGUI::Align::Default )
			->setCaption( "Button2" );

		flow->addSpacer( MyGUI::FloatSize( 1, 0 ) );

		flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntSize( 70, 50 ), MyGUI::Align::Default )
			->setCaption( "Button3" );

		flow->addLineBreak( 10 );

		for( int i = 4; i < 20; ++i )
		{
			MyGUI::ButtonPtr b = flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntSize( 70, 50 ), MyGUI::Align::Default );
			b->setCaption( Ogre::UTFString( L"Button " ) + Ogre::UTFString( Ogre::StringConverter::toString( i+1 ) ) );
			
			flow->addSpacer( MyGUI::IntSize( 10, 10 ) );

			if( i % 4 == 3 )
				flow->addLineBreak( 10 );
		}


		int p = 0;
	}	

    void DemoKeeper::destroyScene()
    {
		delete mFlowContainerFactory;
    }
	 
} // namespace demo

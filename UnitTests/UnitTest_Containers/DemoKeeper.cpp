/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/

#include "precompiled.h"
#include "DemoKeeper.h"
#include "MyGUI_FlowContainer.h"

namespace demo
{
	bool DemoKeeper::frameStarted( const Ogre::FrameEvent & evt )
	{
		bool res = BaseManager::frameStarted( evt );

		//MyGUI::PointerManager::getInstance().get

		return res;
	}

	
	bool DemoKeeper::mouseMoved( const OIS::MouseEvent &arg )
	{
		MyGUI::MYGUI_OUT( arg.state.X.abs, ", ", arg.state.Y.abs );

		return BaseManager::mouseMoved( arg );
	}

	void DemoKeeper::clear()
	{
		//MyGUI::EnumeratorLayerKeeperPtr layers = MyGUI::LayerManager::getInstance().getEnumerator();


		//layers.
	}

	void DemoKeeper::test_VerticalSpacer2()
	{
		MyGUI::WindowPtr window = mGUI->createWidget< MyGUI::Window >( "WindowCS", MyGUI::IntCoord( 0, 0, 300, 300 ), MyGUI::Align::Default, "Overlapped" );

		MyGUI::FlowContainerPtr flow = window->createWidget< MyGUI::FlowContainer >( "FlowContainer", MyGUI::IntCoord( MyGUI::IntPoint(), window->getClientCoord().size() ), MyGUI::Align::HStretch | MyGUI::Align::VStretch, "Overlapped" );
		
		MyGUI::FlowContainer::WidgetInfo* info = nullptr;


		MyGUI::ButtonPtr button = flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default );
		button->setCaption( "Button1" );

		info = flow->getWidgetInfo( button );
		info->sizeDesc.setSize( MyGUI::FloatSize( 0.5, 0.5 ) );

		button = flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default );
		button->setCaption( "Button2" );

		info = flow->getWidgetInfo( button );
		info->sizeDesc.setSize( MyGUI::FloatSize( 0.5, 0.5 ) );

		

	}

	void DemoKeeper::test_VerticalSpacer()
	{
		MyGUI::WindowPtr window = mGUI->createWidget< MyGUI::Window >( "WindowCS", MyGUI::IntCoord( 0, 0, 300, 300 ), MyGUI::Align::Default, "Overlapped" );

		MyGUI::FlowContainerPtr flow = window->createWidget< MyGUI::FlowContainer >( "FlowContainer", MyGUI::IntCoord( MyGUI::IntPoint(), window->getClientCoord().size() ), MyGUI::Align::HStretch | MyGUI::Align::VStretch, "Overlapped" );
		
		//flow->createWidget< MyGUI::FlowContainer >( "FlowContainer", MyGUI::IntCoord( 0, 0, 100, 100 ), MyGUI::Align::HStretch | MyGUI::Align::VStretch, "Overlapped" );
		

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

		MyGUI::SpacerPtr lineBreak = nullptr;
		MyGUI::SpacerPtr spacer = nullptr;
		MyGUI::FlowContainer::WidgetInfo* info = nullptr;


		flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default )
			->setCaption( "Button1" );

		//flow->addSpacer( MyGUI::FloatSize( 0.2, 0 ) );
		spacer = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord(), MyGUI::Align::Default );
		info = flow->getWidgetInfo( spacer );
		info->sizeDesc.setSize( MyGUI::FloatSize( 0.2, 0 ) );

		flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default )
			->setCaption( "Button2" );

		//flow->addSpacer( MyGUI::FloatSize( 0.8, 0 ) );
		spacer = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord(), MyGUI::Align::Default );
		info = flow->getWidgetInfo( spacer );
		info->sizeDesc.setSize( MyGUI::FloatSize( 0.2, 0 ) );

		flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default )
			->setCaption( "Button3" );

		lineBreak = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord(), MyGUI::Align::Default );
		info = flow->getWidgetInfo( lineBreak );
		info->sizeDesc.setSize( MyGUI::FloatSize( 0, 0.5 ) );
		info->breakLine = true;

		/*flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default )
			->setCaption( "Button4" );*/

		/*lineBreak = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord(), MyGUI::Align::Default );
		info = flow->getWidgetInfo( lineBreak );
		info->sizeDesc.setSize( MyGUI::FloatSize( 0, 1 ) );
		info->breakLine = true;

		MyGUI::ButtonPtr b = flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default );
		b->setCaption( "Button0" );

		lineBreak = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord( 0, 0, 0, 0 ), MyGUI::Align::Default );
		info = flow->getWidgetInfo( lineBreak );
		info->sizeDesc.setSize( MyGUI::IntSize( 5, 5 ) );
		info->breakLine = true;

		b = flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default );
		b->setCaption( "Button1" );

		lineBreak = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord( 0, 0, 0, 0 ), MyGUI::Align::Default );
		info = flow->getWidgetInfo( lineBreak );
		info->sizeDesc.setSize( MyGUI::IntSize( 5, 5 ) );
		info->breakLine = true;*/

		for( int i = 0; i < 4; ++i )
		{
			MyGUI::ButtonPtr b = flow->createWidget< MyGUI::Button >( "Button", MyGUI::IntCoord( 0, 0, 70, 50 ), MyGUI::Align::Default );
			b->setCaption( "Button " + Ogre::StringConverter::toString( i+1 ) );
			
			//flow->addSpacer( MyGUI::IntSize( 10, 10 ) );

			spacer = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord( 0, 0, 50, 50 ), MyGUI::Align::Default );
			
			info = flow->getWidgetInfo( spacer );
			info->sizeDesc.setMinSize( MyGUI::IntSize( 0, 0 ) );
			//szDesc->setSize( pxSize );
			//szDesc->setMinSize( pxSize );
			//szDesc->setMaxSize( pxSize );

			if( i % 4 == 3 )
			{
				lineBreak = flow->createWidget< MyGUI::Spacer >( "Spacer", MyGUI::IntCoord( 0, 0, 0, 0 ), MyGUI::Align::Default );

				info = flow->getWidgetInfo( lineBreak );
				info->sizeDesc.setSize( MyGUI::IntSize( MyGUI::INT_SIZE_UNBOUND.width, 5 ) );
				info->breakLine = true;

			}
				//flow->addLineBreak( 10 );
		}

		flow->update();
	}


	void DemoKeeper::createScene()
    {
		// I LOVE IT :)
		mGUI->load("core_theme_black_orange.xml");
		mGUI->load("core_skin.xml");

		
		mFlowContainerFactory = new MyGUI::factory::FlowContainerFactory();
		mSpacerFactory = new MyGUI::factory::SpacerFactory();

		Ogre::String m = mFlowContainerFactory->getTypeName();

		test_VerticalSpacer();

		int p = 0;
	}	

	bool DemoKeeper::keyPressed( const OIS::KeyEvent &arg )
	{
		if( arg.key == OIS::KC_1 )
		{
			//clear();
			//test_VerticalSpacer();
		}

		if( arg.key == OIS::KC_2 )
		{
			//clear();
			//test_VerticalSpacer2();
		}

		return base::BaseManager::keyPressed( arg );
	}

    void DemoKeeper::destroyScene()
    {
		delete mFlowContainerFactory;
    }
	 
} // namespace demo

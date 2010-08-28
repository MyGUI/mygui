/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "EditorState.h"
#include "Application.h"

namespace tools
{

	EditorState::EditorState() :
		mMainPane(nullptr)
	{
	}

	EditorState::~EditorState()
	{
	}

	void EditorState::initState()
	{
		mMainPane = new MainPane();
	}

	void EditorState::cleanupState()
	{
		delete mMainPane;
		mMainPane = nullptr;
	}

	void EditorState::pauseState()
	{
		mMainPane->setVisible(false);
	}

	void EditorState::resumeState()
	{
		mMainPane->setVisible(true);
	}

} // namespace tools

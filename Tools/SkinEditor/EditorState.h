/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __EDITOR_STATE_H__
#define __EDITOR_STATE_H__

#include "StateController.h"
#include "MainPane.h"

namespace tools
{
	class EditorState :
		public StateController
	{
	public:
		EditorState();
		virtual ~EditorState();

		virtual void initState();
		virtual void cleanupState();

		virtual void pauseState();
		virtual void resumeState();

	private:
		MainPane* mMainPane;
	};

} // namespace tools

#endif // __EDITOR_STATE_H__

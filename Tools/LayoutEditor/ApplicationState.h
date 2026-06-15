/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f4f2e520_ac8d_4c34_8c5e_b3e0508e4732_
#define _f4f2e520_ac8d_4c34_8c5e_b3e0508e4732_

#include "StateController.h"

namespace tools
{

	class ApplicationState : public StateController
	{
	public:
		void initState() override;
		void cleanupState() override;

		void pauseState() override;
		void resumeState() override;
	};

}

#endif

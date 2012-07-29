/*!
	@file
	@author		George Evmenov
	@date		03/2011
*/
#ifndef __GRID_H__
#define __GRID_H__

#include <MyGUI_Singleton.h>
#include "BaseLayout/BaseLayout.h"
#include "sigslot.h"

namespace tools
{
	class Grid :
		public MyGUI::Singleton<Grid>,
		public sigslot::has_slots<>
	{
	public:
		Grid();
		virtual ~Grid();

		void initialise();
		void shutdown();

		enum GridLine{ Previous, Closest, Next };
		int toGrid(int _value, GridLine _line = Closest) const;

	private:
		void notifySettingsChanged(const std::string& _path);

		int mGridStep;
	};

} // namespace tools

#endif // __GRID_H__

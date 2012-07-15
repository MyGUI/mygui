/*!
	@file
	@author		George Evmenov
	@date		03/2011
*/
#ifndef __GRID_H__
#define __GRID_H__

#include <MyGUI_Singleton.h>
#include "BaseLayout/BaseLayout.h"

namespace tools
{
	class GridManager : public MyGUI::Singleton<GridManager>
	{
	public:
		GridManager();
		virtual ~GridManager();

		void initialise();
		void shutdown();

		enum GridLine{ Previous, Closest, Next };
		int toGrid(int _value, GridLine _line = Closest) const;
	private:
		void notifySettingsChanged(const MyGUI::UString& _sectorName, const MyGUI::UString& _propertyName);

		int mGridStep;
	};

} // namespace tools

#endif // __GRID_H__

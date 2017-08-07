#ifndef UPDATES_H
#define UPDATES_H

#include "libnethogs.h"
#include <mutex>
#include <map>

class Updates final
{
	Updates() = delete;

public:
	struct Update
	{
		int action;
		NethogsMonitorRecord record;
	};

	static void setRowUpdate(int action, NethogsMonitorRecord const& record);
	static void setNetHogsMonitorStatus(int status);

	static int  getNetHogsMonitorStatus();
	static bool getRowUpdate(Update& update);

	typedef std::map<int, Update> RowUpdatesMap;
	static RowUpdatesMap m_row_updates_map;

	static RowUpdatesMap getRows();

private:

	static std::mutex m_mutex;
	static int m_nethogs_monitor_status;
};

#endif // UPDATES_H

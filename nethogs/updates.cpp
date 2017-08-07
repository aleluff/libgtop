#include "decpcap.c"
#include "process.cpp"
#include "connection.cpp"
#include "conninode.cpp"
#include "inode2prog.cpp"
#include "devices.cpp"
#include "packet.cpp"
#include "libnethogs.cpp"
#include "updates.h"
#include <string.h>

std::mutex Updates::m_mutex;
Updates::RowUpdatesMap Updates::m_row_updates_map;
int Updates::m_nethogs_monitor_status = NETHOGS_STATUS_OK;

void Updates::setRowUpdate(int action, NethogsMonitorRecord const& record)
{
	if( action == NETHOGS_APP_ACTION_REMOVE || record.sent_bytes || record.recv_bytes )
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		Update update;
		memset(&update, 0, sizeof(update));
		update.action = action;
		update.record = record;
		m_row_updates_map[record.record_id] = update;
	}
}

bool Updates::getRowUpdate(Updates::Update& update)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if( m_row_updates_map.empty() )
		return false;
	update = m_row_updates_map.begin()->second;
	m_row_updates_map.erase(m_row_updates_map.begin());
	return true;
}

Updates::RowUpdatesMap Updates::getRows()
{
	return m_row_updates_map;
}

void Updates::setNetHogsMonitorStatus(int status)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_nethogs_monitor_status = status;
}

int Updates::getNetHogsMonitorStatus()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_nethogs_monitor_status;
}

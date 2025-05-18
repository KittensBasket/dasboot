#include <dasboot/core/daemon/daemon.hpp>
#include <dasboot/cru/os.hpp>

int main()
{
	auto ret = NOs::DaemonizeCurrentProcess();
	if(ret.Code == NCommon::TStatus::ECode::Failed)
	{
		std::cout << "Got error trying to daemonize process: " << ret.Error << std::endl;

		return 1;
	}

	try
	{
		NDaemon::TDaemon daemon(DASBOOTD_SOCK);
		daemon.Run();
	}
	catch(...)
	{
		std::cout << "Oops, got an error..." << std::endl;

		return 1;
	}

	return 0;
}
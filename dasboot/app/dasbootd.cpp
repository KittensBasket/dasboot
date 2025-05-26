#include <dasboot/core/daemon/daemon.hpp>
#include <dasboot/cru/os.hpp>
#include <exception>
#include <ostream>
#include <ratio>

int main()
{
	try
	{
		NDaemon::TDaemon daemon(DASBOOTD_SOCK);
		daemon.Run();
	}
	catch(std::exception& e)
	{
		std::cout << "Oops, got an error..." << std::endl;
		std::cout << e.what() << std::endl;

		return 1;
	}

	return 0;
}
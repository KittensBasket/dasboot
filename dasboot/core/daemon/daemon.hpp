#pragma once

#include <dasboot/core/container/container.hpp>
#include <messages.pb.h>
#include <unordered_map>
#include <zmq.hpp>

namespace NDaemon
{

class TDaemon final
{
  public:
	TDaemon();
	~TDaemon();

	TDaemon(const TDaemon &daemon) = delete;
	TDaemon(TDaemon &&daemon) = delete;
	TDaemon &operator=(const TDaemon &daemon) = delete;
	TDaemon &operator=(TDaemon &&daemon) = delete;

	void Run();

  private:
	NMessages::TResult DoBuild(const NMessages::TBuildOptions& options);
	NMessages::TResult DoRun(const NMessages::TRunOptions& options);
	NMessages::TResult DoStart(const NMessages::TStartOptions& options);
	NMessages::TResult DoStop(const NMessages::TStopOptions& options);
	NMessages::TResult DoPs(const NMessages::TPsOptions& options);
	NMessages::TResult DoRm(const NMessages::TRmOptions& options);
	NMessages::TResult DoExec(const NMessages::TExecOptions& options);
	NMessages::TResult DoAttach(const NMessages::TAttachOptions& options);

	zmq::context_t Ctx;
	zmq::socket_t Sock;

	// uint64_t NextId = 0;
	std::unordered_map<uint64_t, NContainer::TContainer> Containers;
	std::unordered_map<std::string, uint64_t> NameToId;
};
}; // namespace NDaemon
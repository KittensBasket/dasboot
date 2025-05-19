#pragma once

#include <dasboot/core/container/container.hpp>
#include <messages.pb.h>
#include <unordered_map>
#include <zmq.hpp>

class TDaemonTest;

namespace NDaemon
{

class TDaemon final
{
  public:
	friend class ::TDaemonTest;	

	explicit TDaemon(const std::string& address);
	explicit TDaemon(const char* address);
	~TDaemon();

	TDaemon() = delete;
	TDaemon(const TDaemon &daemon) = delete;
	TDaemon(TDaemon &&daemon) = delete;
	TDaemon &operator=(const TDaemon &daemon) = delete;
	TDaemon &operator=(TDaemon &&daemon) = delete;

	void Run();

  private:
	NMessages::TResult GetAndParseRequest();
	void SendResponse(const NMessages::TResult& response);

	void Stop();

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
	std::string SocketAddress;

	// uint64_t NextId = 0;
	std::unordered_map<uint64_t, NContainer::TContainer> Containers;
	std::unordered_map<std::string, uint64_t> NameToId;
};
}; // namespace NDaemon
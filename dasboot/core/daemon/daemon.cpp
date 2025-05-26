#include "daemon.hpp"

#include <dasboot/cru/os.hpp>
#include <exception>
#include <iostream>
#include <messages.pb.h>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


namespace NDaemon
{

TDaemon::TDaemon(const std::string& address) : SocketAddress(address)
{
	Sock = zmq::socket_t(Ctx, zmq::socket_type::rep);
	Sock.bind(std::string("ipc://") + SocketAddress);
}

TDaemon::TDaemon(const char* address) : SocketAddress(address)
{
	Sock = zmq::socket_t(Ctx, zmq::socket_type::rep);
	coordinator = NCoordinator::TCoordinator();
	Sock.bind(std::string("ipc://") + SocketAddress);
}

TDaemon::~TDaemon()
{
	Sock.close();
	Ctx.close();

	NOs::RemoveFile(SocketAddress);
}

NMessages::TResult TDaemon::GetAndParseRequest()
{
	zmq::message_t request;
	NMessages::TResult result;

	auto res = Sock.recv(request, zmq::recv_flags::none);
	if(!res.has_value())
	{
		throw "Got EAGAIN, exiting";
	}

	if(request.to_string() == "STOP")
	{
		Stop();
	}

	NMessages::TOptionsWrapper wrapper;

    wrapper.ParseFromString(request.to_string());

	switch(wrapper.option_case())
	{
		case NMessages::TOptionsWrapper::OptionCase::kBuildOptions: {
			result = DoBuild(wrapper.build_options());
			break;
        }
		case NMessages::TOptionsWrapper::OptionCase::kRunOptions: {
			result = DoRun(wrapper.run_options());
            break;
        }
		case NMessages::TOptionsWrapper::OptionCase::kStartOptions: {
			result = DoStart(wrapper.start_options());
			break;
        }
		case NMessages::TOptionsWrapper::OptionCase::kStopOptions: {
			result = DoStop(wrapper.stop_options());
			break;
        }
		case NMessages::TOptionsWrapper::OptionCase::kPsOptions: {
			result = DoPs(wrapper.ps_options());
			break;
        }
		case NMessages::TOptionsWrapper::OptionCase::kRmOptions: {
			result = DoRm(wrapper.rm_options());
			break;
        }
		case NMessages::TOptionsWrapper::OptionCase::kExecOptions: {
			result = DoExec(wrapper.exec_options());
			break;
        }
		case NMessages::TOptionsWrapper::OptionCase::kAttachOptions: {
			result = DoAttach(wrapper.attach_options());
            break;
        }
		default:
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Unknown request");
	}

	return result;
}

void TDaemon::SendResponse(const NMessages::TResult& response)
{
	zmq::message_t responseString(response.SerializeAsString());
	Sock.send(responseString, zmq::send_flags::none);
}

void TDaemon::Run()
{
	while(true)
	{
		SendResponse(GetAndParseRequest());
	}
}

void TDaemon::Stop()
{
	exit(0);
}

NMessages::TResult TDaemon::DoBuild(const NMessages::TBuildOptions& options)
{
	NMessages::TResult result;
	coordinator.Build(options);
	result.set_code(NMessages::ReturnCode::SUCESS);
	result.set_text("hello");
    return result;
}

NMessages::TResult TDaemon::DoRun(const NMessages::TRunOptions &options)
{
	NMessages::TResult result;

	if(options.has_name())
	{
		if(NameToId.contains(options.name()))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Container with such name already exists");
		}
		else
		{
			// uint64_t id = NextId++;
			// NContainer::TContainer container({.id = id});

			// container.Build();
			// container.Exec();

			// Containers.emplace(id, std::move(container));

			// result.set_code(NMessages::ReturnCode::SUCESS);
			// result.set_text("Container has been successfully ran");

			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Not implemented");
		}
	}
	else
	{
		result.set_code(NMessages::ReturnCode::ERROR);
		result.set_text("No name given");
	}

	return result;
}

NMessages::TResult TDaemon::DoStart(const NMessages::TStartOptions &options)
{
	NMessages::TResult result;

	if (options.has_id())
	{
		uint64_t id = std::stoull(options.id());
		if (!Containers.contains(id))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such id");
		}
		else
		{
			// Containers.at(id).Exec();
			result.set_code(NMessages::ReturnCode::SUCESS);
			result.set_text("Successfully started container");
		}
	}
	else if (options.has_name())
	{
		if (!NameToId.contains(options.name()))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such name");
		}
		else
		{
			// Containers.at(NameToId.at(options.name())).Exec();

			result.set_code(NMessages::ReturnCode::SUCESS);
			result.set_text("Successfully started container");
		}
	}
	else
	{
		result.set_code(NMessages::ReturnCode::ERROR);
		result.set_text("No name or id given");
	}

	return result;
}

NMessages::TResult TDaemon::DoStop(const NMessages::TStopOptions &options)
{
	NMessages::TResult result;

	if (options.has_id())
	{
		uint64_t id = std::stoull(options.id());
		if (!Containers.contains(id))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such id");
		}
		else
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Not implemented");
		}
	}
	else if (options.has_name())
	{
		if (!NameToId.contains(options.name()))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such name");
		}
		else
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Not implemented");
		}
	}
	else
	{
		result.set_code(NMessages::ReturnCode::ERROR);
		result.set_text("No name or id given");
	}

	return result;
}

NMessages::TResult TDaemon::DoPs(const NMessages::TPsOptions &options)
{
	NMessages::TResult result;
    auto [ans, status] = coordinator.Ps(options);
	result.set_code(NMessages::ReturnCode::SUCESS);
	result.set_text(ans);
	return result;
}

NMessages::TResult TDaemon::DoRm(const NMessages::TRmOptions &options)
{
	NMessages::TResult result;

	if(options.has_id())
	{
		if(!Containers.contains(std::stoull(options.id())))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such id");
		}
		else
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Not implemented");
		}
	}
	else if(options.has_name())
	{
		if(!NameToId.contains(options.name()))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such name");
		}
		else
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Not implemented");
		}
	}
	else
	{
		result.set_code(NMessages::ReturnCode::ERROR);
		result.set_text("No name or id given");
	}	
	
	return result;
}

NMessages::TResult TDaemon::DoExec(const NMessages::TExecOptions & options)
{
    NMessages::TResult result;
    coordinator.Exec(options);
	result.set_code(NMessages::ReturnCode::SUCESS);
	result.set_text("hello");
    return result;
}

NMessages::TResult TDaemon::DoAttach(const NMessages::TAttachOptions & options)
{
	NMessages::TResult result;

	if(options.has_id())
	{
		if(!Containers.contains(std::stoull(options.id())))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such id");
		}
		else
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Not implemented");
		}
	}
	else if(options.has_name())
	{
		if(!NameToId.contains(options.name()))
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("No container with such name");
		}
		else
		{
			result.set_code(NMessages::ReturnCode::ERROR);
			result.set_text("Not implemented");
		}
	}
	else
	{
		result.set_code(NMessages::ReturnCode::ERROR);
		result.set_text("No name or id given");
	}	

	return result;
}

}; // namespace NDaemon

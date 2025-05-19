#include <gtest/gtest.h>

#include <iostream>

#include <dasboot/core/daemon/daemon.hpp>
#include <dasboot/cru/os.hpp>

//TODO(riogor): ALL TESTS IN COMMENTS ARE CURRENTLY FAILING AND BROKEN, BECAUSE OF ABSENCE OF INNER IMPLEMENTATION

class TDaemonTest : public ::testing::Test
{
    protected:
        NDaemon::TDaemon daemon{"dasboot.sock"};

    void SetUp() override 
    {
    }

    void TearDown() override
    {
    }

    auto GetAndParseRequest()
    {
        return daemon.GetAndParseRequest();
    }

    auto SendResponse(const NMessages::TResult& response)
    {
        daemon.SendResponse(response);
    }

	auto DoBuild(const NMessages::TBuildOptions& options)
    {
        return daemon.DoBuild(options);
    }

	auto DoRun(const NMessages::TRunOptions& options)
    {
        return daemon.DoRun(options);
    }

	auto DoStart(const NMessages::TStartOptions& options)
    {
        return daemon.DoStart(options);
    }

	auto DoStop(const NMessages::TStopOptions& options)
    {
        return daemon.DoStop(options);
    }

	auto DoPs(const NMessages::TPsOptions& options)
    {
        return daemon.DoPs(options);
    }

	auto DoRm(const NMessages::TRmOptions& options)
    {
        return daemon.DoRm(options);
    }

	auto DoExec(const NMessages::TExecOptions& options)
    {
        return daemon.DoExec(options);
    }

	auto DoAttach(const NMessages::TAttachOptions& options)
    {
        return daemon.DoAttach(options);
    }
};

TEST_F(TDaemonTest, DaemonStart)
{
	EXPECT_TRUE(NOs::IsPathExists("dasboot.sock"));
}

TEST_F(TDaemonTest, DaemonUnknownCommand)
{
    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::req);
    sock.connect("ipc://" "dasboot.sock");
    sock.send(zmq::buffer("a"), zmq::send_flags::none);    

    auto result = GetAndParseRequest();

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "Unknown request");
}

TEST_F(TDaemonTest, DaemonBuildCommandByNameCorrect)
{
    NMessages::TBuildOptions request;
    request.set_name("a");

	auto result = DoBuild(request);

	EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
	EXPECT_EQ(result.text(), "Not implemented");
}

// TEST_F(TDaemonTest, DaemonBuildCommandByNameDuplicate)
// {
//     NMessages::TBuildOptions request;
//     request.set_name("a");

//     {
//         auto result = DoBuild(request);

//         EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//         EXPECT_EQ(result.text(), "Not implemented");
//     }

//     {
//         auto result = DoBuild(request);

//         EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//         EXPECT_EQ(result.text(), "Container with such name already exists");
//     }
// }

TEST_F(TDaemonTest, DaemonBuildCommandNoName)
{
    NMessages::TBuildOptions request;

    auto result = DoBuild(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No name given");
}

TEST_F(TDaemonTest, DaemonRunCommandByNameCorrect)
{
    NMessages::TRunOptions request;
    request.set_name("a");

    auto result = DoRun(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "Not implemented");
}

// TEST_F(TDaemonTest, DaemonRunCommandByNameDuplicate)
// {
//     NMessages::TRunOptions request;
//     request.set_name("a");

//     {
//         auto result = DoRun(request);

//         EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//         EXPECT_EQ(result.text(), "Not implemented");
//     }

//     {
//         auto result = DoRun(request);

//         EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//         EXPECT_EQ(result.text(), "Container with such name already exists");
//     }
// }

TEST_F(TDaemonTest, DaemonRunCommandNoName)
{
    NMessages::TRunOptions request;

    auto result = DoRun(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No name given");
}

// TEST_F(TDaemonTest, DaemonStartCommandByNameCorrect)
// {
//     NMessages::TStartOptions request;
//     request.set_name("a");

//     auto result = DoStart(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonStartCommandNoContainerWithName)
{
    NMessages::TStartOptions request;
    request.set_name("a");

    auto result = DoStart(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such name");
}

// TEST_F(TDaemonTest, DaemonStartCommandByIdCorrect)
// {
//     NMessages::TStartOptions request;
//     request.set_id("1");

//     auto result = DoStart(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonStartCommandNoContainerWithId)
{
    NMessages::TStartOptions request;
    request.set_id("1");

    auto result = DoStart(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such id");
}

TEST_F(TDaemonTest, DaemonStartCommandNoNameNoId)
{
    NMessages::TStartOptions request;

    auto result = DoStart(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No name or id given");
}

// TEST_F(TDaemonTest, DaemonStopCommandByNameCorrect)
// {
//     NMessages::TStopOptions request;
//     request.set_name("a");

//     auto result = DoStop(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonStopCommandNoContainerWithName)
{
    NMessages::TStopOptions request;
    request.set_name("a");

    auto result = DoStop(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such name");
}

// TEST_F(TDaemonTest, DaemonStopCommandByIdCorrect)
// {
//     NMessages::TStopOptions request;
//     request.set_id("0");

//     auto result = DoStop(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonStopCommandNoContainerWithId)
{
    NMessages::TStopOptions request;
    request.set_id("1");

    auto result = DoStop(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such id");
}

TEST_F(TDaemonTest, DaemonStopCommandNoNameNoId)
{
    NMessages::TStopOptions request;

    auto result = DoStop(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No name or id given");
}

TEST_F(TDaemonTest, DaemonPsCommand)
{
    NMessages::TPsOptions request;

    auto result = DoPs(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::SUCESS);
    EXPECT_EQ(result.text(), "");
}

// TEST_F(TDaemonTest, DaemonRmCommandByNameCorrect)
// {
//     NMessages::TRmOptions request;
//     request.set_name("a");

//     auto result = DoRm(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonRmCommandNoContainerWithName)
{
    NMessages::TRmOptions request;
    request.set_name("a");

    auto result = DoRm(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such name");
}

// TEST_F(TDaemonTest, DaemonRmCommandByIdCorrect)
// {
//     NMessages::TRmOptions request;
//     request.set_id("0");

//     auto result = DoRm(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonRmCommandNoContainerWithId)
{
    NMessages::TRmOptions request;
    request.set_id("1");

    auto result = DoRm(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such id");
}

TEST_F(TDaemonTest, DaemonRmCommandNoNameNoId)
{
    NMessages::TRmOptions request;

    auto result = DoRm(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No name or id given");
}

// TEST_F(TDaemonTest, DaemonExecCommandByNameCorrect)
// {
//     NMessages::TExecOptions request;
//     request.set_name("a");

//     auto result = DoExec(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonExecCommandNoContainerWithName)
{
    NMessages::TExecOptions request;
    request.set_name("a");

    auto result = DoExec(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such name");
}

// TEST_F(TDaemonTest, DaemonExecCommandByIdCorrect)
// {
//     NMessages::TExecOptions request;
//     request.set_id("0");

//     auto result = DoExec(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonExecCommandNoContainerWithId)
{
    NMessages::TExecOptions request;
    request.set_id("1");

    auto result = DoExec(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such id");
}

TEST_F(TDaemonTest, DaemonExecCommandNoNameNoId)
{
    NMessages::TExecOptions request;

    auto result = DoExec(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No name or id given");
}

// TEST_F(TDaemonTest, DaemonAttachCommandByNameCorrect)
// {
//     NMessages::TAttachOptions request;
//     request.set_name("a");

//     auto result = DoAttach(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonAttachCommandNoContainerWithName)
{
    NMessages::TAttachOptions request;
    request.set_name("a");

    auto result = DoAttach(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such name");
}

// TEST_F(TDaemonTest, DaemonAttachCommandByIdCorrect)
// {
//     NMessages::TAttachOptions request;
//     request.set_id("0");

//     auto result = DoAttach(request);

//     EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
//     EXPECT_EQ(result.text(), "Not implemented");
// }

TEST_F(TDaemonTest, DaemonAttachCommandNoContainerWithId)
{
    NMessages::TAttachOptions request;
    request.set_id("1");

    auto result = DoAttach(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No container with such id");
}

TEST_F(TDaemonTest, DaemonAttachCommandNoNameNoId)
{
    NMessages::TAttachOptions request;

    auto result = DoAttach(request);

    EXPECT_EQ(result.code(), NMessages::ReturnCode::ERROR);
    EXPECT_EQ(result.text(), "No name or id given");

}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	auto result = RUN_ALL_TESTS();
	return result;
}
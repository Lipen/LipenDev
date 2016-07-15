#include <event.h>
#include <evhttp.h>

#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <vector>
#include <iostream>

#include <tr1/functional>
#include <tr1/memory>

#include "tcp_server_socket.h"
#include "inet_address_v4.h"
#include "thread.h"
#include "command_line.h"
#include "logger.h"
#include "file_holder.h"

namespace Network {
	namespace Private {
		DECLARE_RUNTIME_EXCEPTION(EventBaseHolder)

		class EventBaseHolder
			: private Common::NonCopyable
		{
		public:
			EventBaseHolder()
				: EventBase(event_base_new())
			{
				if (!EventBase)
					throw EventBaseHolderException("Failed to create new event_base");
			}
			~EventBaseHolder()
			{
				event_base_free(EventBase);
			}
			event_base* GetBase() const
			{
				return EventBase;
			}

		private:
			event_base *EventBase;
		};

		DECLARE_RUNTIME_EXCEPTION(HttpEventHolder)

		class HttpEventHolder
			: public EventBaseHolder
		{
		public:
			typedef std::tr1::function<void (char const *, evbuffer *)> RequestHandler;
			HttpEventHolder(SocketHandle sock, RequestHandler const &handler)
				: Handler(handler)
				, Http(evhttp_new(GetBase()))
			{
				evhttp_set_allowed_methods(Http, EVHTTP_REQ_GET);
				evhttp_set_gencb(Http, &HttpEventHolder::RawHttpRequestHandler, this);
				if (evhttp_accept_socket(Http, sock) == -1)
					throw HttpEventHolderException("Failed to accept socket for http");
			}
			~HttpEventHolder()
			{
				evhttp_free(Http);
			}

		private:
			RequestHandler Handler;
			evhttp *Http;

			static void RawHttpRequestHandler(evhttp_request *request, void *prm)
			{
				reinterpret_cast<HttpEventHolder *>(prm)->ProcessRequest(request);
			}

			void ProcessRequest(evhttp_request *request)
			{
				try
				{
					Handler(request->uri, request->output_buffer);
					evhttp_send_reply(request, HTTP_OK, "OK", request->output_buffer);
				}
				catch (std::exception const &e)
				{
					evhttp_send_reply(request, HTTP_INTERNAL,
														e.what() ? e.what() : "Internal server error.",
														request->output_buffer);
				}
			}
		};

		class ServerThread
			: private HttpEventHolder
			, private System::Thread
		{
		public:
			ServerThread(SocketHandle sock, std::string const &rootDir, std::string const &defaultPage)
				: HttpEventHolder(sock, std::tr1::bind(&ServerThread::OnRequest, this,
																							 std::tr1::placeholders::_1,
																							 std::tr1::placeholders::_2))
				, Thread(std::tr1::bind(&ServerThread::DispatchProc, this))
				, RootDir(rootDir)
				, DefaultPage(defaultPage)
			{
			}
			~ServerThread()
			{
				IsRun = false;
			}

		private:
			enum { WaitTimeout = 10000 };

			bool volatile IsRun;
			std::string RootDir;
			std::string DefaultPage;

			void DispatchProc()
			{
				IsRun = true;
				while(IsRun)
				{
					if (event_base_loop(GetBase(), EVLOOP_NONBLOCK))
					{
						Common::Log::GetLogInst() << "Failed to run dispatch events";
						break;
					}
					usleep(WaitTimeout);
				}
			}

			void OnRequest(char const *resource, evbuffer *outBuffer)
			{
				std::string FileName;
				GetFullFileName(resource, &FileName);
				try
				{
					System::FileHolder File(FileName);
					if (!File.GetSize())
					{
						evbuffer_add_printf(outBuffer, "Empty file");
						return;
					}
					evbuffer_add_file(outBuffer, File.GetHandle(), 0, File.GetSize());
					File.Detach();
				}
				catch (System::FileHolderException const &)
				{
					evbuffer_add_printf(outBuffer, "File not found");
				}
			}

			void GetFullFileName(char const *resource, std::string *fileName) const
			{
				fileName->append(RootDir);
				if (!resource || !strcmp(resource, "/"))
				{
					fileName->append("/");
					fileName->append(DefaultPage);
				}
				else
				{
					fileName->append(resource);
				}
			}
		};

	}

	class HTTPServer
		: private TCPServerSocket
	{
	public:
		HTTPServer(InetAddress const &locAddr, int backlog,
							int maxThreadsCount,
							std::string const &rootDir, std::string const &defaultPage)
			: TCPServerSocket(locAddr, backlog)
		{
			for (int i = 0 ; i < maxThreadsCount ; ++i)
			{
				ServerThreads.push_back(ServerThreadPtr(new Private::ServerThread(GetHandle(),
					rootDir, defaultPage)));
			}
		}

	private:
		typedef std::tr1::shared_ptr<Private::ServerThread> ServerThreadPtr;
		typedef std::vector<ServerThreadPtr> ServerThreadPool;
		ServerThreadPool ServerThreads;
	};

}

int main(int argc, char const **argv)
{
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		std::cerr << "Failed to call signal(SIGPIPE, SIG_IGN)" << std::endl;
		return 0;
	}
	try
	{
		char const ServerAddr[] = "Server";
		char const ServerPort[] = "Port";
		char const MaxBacklog[] = "Backlog";
		char const ThreadsCount[] = "Threads";
		char const RootDir[] = "Root";
		char const DefaultPage[] = "DefaultPage";

		// Server:127.0.0.1 Port:5555 Backlog:10 Threads:4 Root:./ DefaultPage:index.html
		Common::CommandLine CmdLine(argc, argv);

		Network::HTTPServer Srv(
				Network::InetAddressV4::CreateFromString(
					CmdLine.GetStrParameter(ServerAddr),
					CmdLine.GetParameter<unsigned short>(ServerPort)),
				CmdLine.GetParameter<unsigned>(MaxBacklog),
				CmdLine.GetParameter<unsigned>(ThreadsCount),
				CmdLine.GetStrParameter(RootDir),
				CmdLine.GetStrParameter(DefaultPage)
			);
		std::cin.get();
	}
	catch (std::exception const &e)
	{
		Common::Log::GetLogInst() << e.what();
	}
	return 0;
}

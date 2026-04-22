
#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../../include/http/HttpRequest.hpp"
#include "../../include/http/HttpResponse.hpp"
#include <string>
#include <map>
#include <vector>
#include <sys/types.h>
#include <ctime>

class CgiHandler
{
	private:
		std::string script_path;
		std::string interpreter_path;
		
		// Helper: Build CGI environment variables
		std::vector<std::string> buildEnvironment(const HttpRequest& req) const;
		
		// Helper: Extract headers from raw output
		std::map<std::string, std::string> extractHeaders(const std::string& raw);

		HttpResponse makeErrorResponse(const std::string& status, const std::string& reason,
			const std::string& message) const;
		void finalizeResponse(HttpResponse& resp) const;
		void closeSessionFd(int& fd) const;

	public:
		struct CgiSession
		{
			pid_t pid;
			int stdin_fd;
			int stdout_fd;
			std::string input;
			std::string output;
			size_t input_offset;
			time_t start_time;
			bool stdin_closed;
			bool stdout_closed;
			bool process_done;
			int exit_status;

			CgiSession();
		};

		CgiHandler();
		CgiHandler(const std::string& script, const std::string& interpreter);
		~CgiHandler();

		// Compatibility helper for local tests. The final server should use the
		// session methods and register stdin_fd/stdout_fd in its main epoll loop.
		HttpResponse execute(const HttpRequest& req);

		bool startSession(const HttpRequest& req, CgiSession& session);
		bool writeRequestBody(CgiSession& session);
		bool readOutput(CgiSession& session);
		bool updateProcessStatus(CgiSession& session);
		bool hasTimedOut(const CgiSession& session, int timeout_seconds) const;
		void killSession(CgiSession& session);
		HttpResponse finishSession(CgiSession& session, bool timed_out);

		HttpResponse parseCgiOutput(const std::string& output, int exit_status, bool timed_out);
};

#endif

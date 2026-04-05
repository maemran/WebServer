
#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "../../include/http/HttpRequest.hpp"
#include "../../include/http/HttpResponse.hpp"
#include <string>
#include <map>

class CgiHandler
{
	private:
		std::string script_path;
		std::string interpreter_path;
		
		// Helper: Build CGI environment variables
		void buildEnvironment(const HttpRequest& req, char** env);
		
		// Helper: Parse CGI response headers
		HttpResponse parseCgiOutput(const std::string& output);
		
		// Helper: Extract headers from raw output
		std::map<std::string, std::string> extractHeaders(const std::string& raw);

	public:
		CgiHandler();
		CgiHandler(const std::string& script, const std::string& interpreter);
		~CgiHandler();

		// Execute CGI script with given request, return response
		HttpResponse execute(const HttpRequest& req);
};

#endif

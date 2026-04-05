
#include "../../include/cgi/CgiHandler.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>

CgiHandler::CgiHandler() : script_path(""), interpreter_path("") {}

CgiHandler::CgiHandler(const std::string& script, const std::string& interpreter)
	: script_path(script), interpreter_path(interpreter) {}

CgiHandler::~CgiHandler() {}

HttpResponse CgiHandler::execute(const HttpRequest& req)
{
	// Step 1: Create pipes for communication
	int stdin_pipe[2];   // Script reads from it
	int stdout_pipe[2];  // Script writes to it

	if (pipe(stdin_pipe) == -1 || pipe(stdout_pipe) == -1)
	{
		HttpResponse resp;
		resp.setStatusCode("500");
		resp.setBody("CGI: Failed to create pipes");
		return resp;
	}

	// Step 2: Fork child process
	pid_t pid = fork();
	if (pid == -1)
	{
		close(stdin_pipe[0]);
		close(stdin_pipe[1]);
		close(stdout_pipe[0]);
		close(stdout_pipe[1]);
		
		HttpResponse resp;
		resp.setStatusCode("500");
		resp.setBody("CGI: Failed to fork");
		return resp;
	}

	if (pid == 0)  // Child process
	{
		// Redirect stdin/stdout
		dup2(stdin_pipe[0], STDIN_FILENO);
		dup2(stdout_pipe[1], STDOUT_FILENO);

		close(stdin_pipe[0]);
		close(stdin_pipe[1]);
		close(stdout_pipe[0]);
		close(stdout_pipe[1]);

		// Build CGI environment variables
		char* env[50];  // Enough for typical CGI vars
		buildEnvironment(req, env);

		// Execute the interpreter with script
		const char* args[] = { interpreter_path.c_str(), script_path.c_str(), NULL };
		execve(interpreter_path.c_str(), (char* const*)args, env);

		// If execve fails, exit child
		std::exit(127);
	}
	else  // Parent process
	{
		close(stdin_pipe[0]);  // Don't read from stdin
		close(stdout_pipe[1]); // Don't write to stdout

		// Send request body to CGI stdin
		const std::string& body = req.getEntityBody();
		if (!body.empty())
		{
			write(stdin_pipe[1], body.c_str(), body.length());
		}
		close(stdin_pipe[1]);

		// Read CGI stdout
		std::string output;
		char buffer[4096];
		ssize_t bytes_read;

		while ((bytes_read = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0)
		{
			output.append(buffer, bytes_read);
		}
		close(stdout_pipe[0]);

		// Wait for child to finish
		int status;
		waitpid(pid, &status, 0);

		// Parse CGI output into HttpResponse
		HttpResponse resp = parseCgiOutput(output);
		return resp;
	}
}

void CgiHandler::buildEnvironment(const HttpRequest& req, char** env)
{
	static char method_str[256];
	static char content_length_str[256];
	static char script_filename_str[512];
	static char request_uri_str[512];

	int idx = 0;

	// CGI/1.1 required variables
	snprintf(method_str, sizeof(method_str), "REQUEST_METHOD=%s", req.getMethod().c_str());
	env[idx++] = method_str;

	snprintf(script_filename_str, sizeof(script_filename_str), "SCRIPT_FILENAME=%s", script_path.c_str());
	env[idx++] = script_filename_str;

	snprintf(request_uri_str, sizeof(request_uri_str), "REQUEST_URI=%s", req.getUri().getPath().c_str());
	env[idx++] = request_uri_str;

	// Content length (for POST)
	const std::map<std::string, std::string>& all_headers = req.getHeaders();
	std::map<std::string, std::string>::const_iterator content_it = all_headers.find("Content-Length");
	if (content_it != all_headers.end())
	{
		snprintf(content_length_str, sizeof(content_length_str), "CONTENT_LENGTH=%s", content_it->second.c_str());
		env[idx++] = content_length_str;
	}

	// HTTP headers → CGI_VAR format
	// (Example: "Content-Type" header becomes "CONTENT_TYPE")
	const std::map<std::string, std::string>& headers = req.getHeaders();
	for (std::map<std::string, std::string>::const_iterator it = headers.begin();
		 it != headers.end() && idx < 48; ++it)
	{
		static char header_str[512];
		std::string key = it->first;
		// Convert to uppercase with underscores
		for (size_t i = 0; i < key.length(); ++i)
		{
			if (key[i] == '-')
				key[i] = '_';
			else if (key[i] >= 'a' && key[i] <= 'z')
				key[i] -= ('a' - 'A');
		}
		snprintf(header_str, sizeof(header_str), "HTTP_%s=%s", key.c_str(), it->second.c_str());
		env[idx++] = header_str;
	}

	// Null terminator
	env[idx] = NULL;
}

HttpResponse CgiHandler::parseCgiOutput(const std::string& output)
{
	// Find empty line that separates headers from body
	size_t header_end = output.find("\r\n\r\n");
	if (header_end == std::string::npos)
		header_end = output.find("\n\n");

	std::string header_part;
	std::string body_part;

	if (header_end != std::string::npos)
	{
		header_part = output.substr(0, header_end);
		body_part = output.substr(header_end + (output[header_end] == '\r' ? 4 : 2));
	}
	else
	{
		// No proper header/body split, treat all as body
		body_part = output;
	}

	// Extract headers
	std::map<std::string, std::string> headers = extractHeaders(header_part);

	// Default status is 200, unless CGI specifies "Status:" header
	std::string status_str = "200";
	std::string reason = "OK";

	if (headers.find("Status") != headers.end())
	{
		std::string status_line = headers["Status"];
		size_t space = status_line.find(' ');
		if (space != std::string::npos)
		{
			status_str = status_line.substr(0, space);
			reason = status_line.substr(space + 1);
		}
		headers.erase("Status");  // Remove from headers, put in status line
	}

	HttpResponse resp;
	resp.setStatusCode(status_str);
	resp.setReasonPhrase(reason);
	resp.setBody(body_part);

	// Add all headers from CGI output
	for (std::map<std::string, std::string>::iterator it = headers.begin();
		 it != headers.end(); ++it)
	{
		resp.addHeader(it->first, it->second);
	}

	return resp;
}

std::map<std::string, std::string> CgiHandler::extractHeaders(const std::string& raw)
{
	std::map<std::string, std::string> headers;
	std::istringstream stream(raw);
	std::string line;

	while (std::getline(stream, line))
	{
		// Remove trailing \r if present
		if (!line.empty() && line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);

		if (line.empty())
			break;

		// Find colon separator
		size_t colon_pos = line.find(':');
		if (colon_pos != std::string::npos)
		{
			std::string key = line.substr(0, colon_pos);
			std::string value = line.substr(colon_pos + 1);

			// Trim leading space from value
			if (!value.empty() && value[0] == ' ')
				value.erase(0, 1);

			headers[key] = value;
		}
	}

	return headers;
}

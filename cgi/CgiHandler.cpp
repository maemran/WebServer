/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:26:21 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/22 17:44:07 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "CgiHandler.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>


static std::string toUpperUnderscore(const std::string& value)
{
	std::string out = value;
	for (size_t i = 0; i < out.length(); ++i)
	{
		if (out[i] == '-')
			out[i] = '_';
		else if (out[i] >= 'a' && out[i] <= 'z')
			out[i] -= ('a' - 'A');
	}
	return out;
}
static std::string toLowerString(const std::string& value)
{
	std::string out = value;
	for (size_t i = 0; i < out.length(); ++i)
	{
		if (out[i] >= 'A' && out[i] <= 'Z')
			out[i] += ('a' - 'A');
	}
	return out;
}

static std::string getHeaderValue(const std::map<std::string, std::string>& headers,
	const std::string& name)
{
	std::string wanted = toLowerString(name);
	for (std::map<std::string, std::string>::const_iterator it = headers.begin();
		it != headers.end(); ++it)
	{
		if (toLowerString(it->first) == wanted)
			return it->second;
	}
	return "";
}

static std::string getDirectoryName(const std::string& path)
{
	size_t slash = path.find_last_of('/');
	if (slash == std::string::npos)
		return ".";
	if (slash == 0)
		return "/";
	return path.substr(0, slash);
}

static std::string getBaseName(const std::string& path)
{
	size_t slash = path.find_last_of('/');
	if (slash == std::string::npos)
		return path;
	return path.substr(slash + 1);
}

static bool hasHeader(const std::vector<std::string>& headers, const std::string& name)
{
	std::string prefix = toLowerString(name) + ":";
	for (std::vector<std::string>::const_iterator it = headers.begin();
		it != headers.end(); ++it)
	{
		if (toLowerString(it->substr(0, prefix.length())) == prefix)
			return true;
	}
	return false;
}

static bool headerNameEquals(const std::string& left, const std::string& right)
{
	return toLowerString(left) == toLowerString(right);
}

static std::map<std::string, std::string>::iterator findHeader(
	std::map<std::string, std::string>& headers, const std::string& name)
{
	for (std::map<std::string, std::string>::iterator it = headers.begin();
		it != headers.end(); ++it)
	{
		if (headerNameEquals(it->first, name))
			return it;
	}
	return headers.end();
}

static std::string defaultReasonPhrase(const std::string& status)
{
	if (status == "200")
		return "OK";
	if (status == "201")
		return "Created";
	if (status == "204")
		return "No Content";
	if (status == "301")
		return "Moved Permanently";
	if (status == "302")
		return "Found";
	if (status == "400")
		return "Bad Request";
	if (status == "403")
		return "Forbidden";
	if (status == "404")
		return "Not Found";
	if (status == "405")
		return "Method Not Allowed";
	if (status == "411")
		return "Length Required";
	if (status == "413")
		return "Content Too Large";
	if (status == "500")
		return "Internal Server Error";
	if (status == "501")
		return "Not Implemented";
	if (status == "502")
		return "Bad Gateway";
	if (status == "504")
		return "Gateway Timeout";
	if (status == "505")
		return "HTTP Version Not Supported";
	return "";
}

static void setNonBlocking(int fd)
{
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

CgiHandler::CgiSession::CgiSession()
	: pid(-1),
	  stdin_fd(-1),
	  stdout_fd(-1),
	  input(""),
	  output(""),
	  input_offset(0),
	  start_time(0),
	  stdin_closed(true),
	  stdout_closed(true),
	  process_done(false),
	  exit_status(0)
{}

CgiHandler::CgiHandler() : script_path(""), interpreter_path("") {}

CgiHandler::CgiHandler(const std::string& script, const std::string& interpreter)
	: script_path(script), interpreter_path(interpreter) {}

CgiHandler::~CgiHandler() {}

HttpResponse CgiHandler::execute(const HttpRequest& req)
{
	CgiSession session;

	if (!startSession(req, session))
		return makeErrorResponse("500", "Internal Server Error", "CGI: failed to start");

	while (!session.stdout_closed || !session.stdin_closed || !session.process_done)
	{
		if (hasTimedOut(session, 10))
		{
			killSession(session);
			return finishSession(session, true);
		}

		fd_set readfds;
		fd_set writefds;
		FD_ZERO(&readfds);
		FD_ZERO(&writefds);
		int max_fd = -1;

		if (!session.stdout_closed)
		{
			FD_SET(session.stdout_fd, &readfds);
			max_fd = std::max(max_fd, session.stdout_fd);
		}
		if (!session.stdin_closed)
		{
			FD_SET(session.stdin_fd, &writefds);
			max_fd = std::max(max_fd, session.stdin_fd);
		}

		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if (max_fd >= 0 && select(max_fd + 1, &readfds, &writefds, NULL, &timeout) > 0)
		{
			if (!session.stdout_closed && FD_ISSET(session.stdout_fd, &readfds))
				readOutput(session);
			if (!session.stdin_closed && FD_ISSET(session.stdin_fd, &writefds))
				writeRequestBody(session);
		}
		updateProcessStatus(session);
	}

	return finishSession(session, false);
}

bool CgiHandler::startSession(const HttpRequest& req, CgiSession& session)
{
	int stdin_pipe[2];
	int stdout_pipe[2];

	if (pipe(stdin_pipe) == -1)
		return false;
	if (pipe(stdout_pipe) == -1)
	{
		close(stdin_pipe[0]);
		close(stdin_pipe[1]);
		return false;
	}

	pid_t pid = fork();
	if (pid == -1)
	{
		close(stdin_pipe[0]);
		close(stdin_pipe[1]);
		close(stdout_pipe[0]);
		close(stdout_pipe[1]);
		return false;
	}

	if (pid == 0)
	{
		dup2(stdin_pipe[0], STDIN_FILENO);
		dup2(stdout_pipe[1], STDOUT_FILENO);
		close(stdin_pipe[0]);
		close(stdin_pipe[1]);
		close(stdout_pipe[0]);
		close(stdout_pipe[1]);

		std::vector<std::string> env_strings = buildEnvironment(req);
		std::vector<char*> env;
		for (size_t i = 0; i < env_strings.size(); ++i)
			env.push_back(const_cast<char*>(env_strings[i].c_str()));
		env.push_back(NULL);

		std::string script_dir = getDirectoryName(script_path);
		std::string script_name = getBaseName(script_path);
		if (chdir(script_dir.c_str()) == -1)
			std::exit(127);

		const char* args[] = { interpreter_path.c_str(), script_name.c_str(), NULL };
		execve(interpreter_path.c_str(), (char* const*)args, &env[0]);
		std::exit(127);
	}

	close(stdin_pipe[0]);
	close(stdout_pipe[1]);
	setNonBlocking(stdin_pipe[1]);
	setNonBlocking(stdout_pipe[0]);

	session.pid = pid;
	session.stdin_fd = stdin_pipe[1];
	session.stdout_fd = stdout_pipe[0];
	session.input = req.getEntityBody();
	session.output = "";
	session.input_offset = 0;
	session.start_time = std::time(NULL);
	session.stdin_closed = false;
	session.stdout_closed = false;
	session.process_done = false;
	session.exit_status = 0;
	return true;
}

bool CgiHandler::writeRequestBody(CgiSession& session)
{
	if (session.stdin_closed)
		return true;
	if (session.input_offset >= session.input.length())
	{
		closeSessionFd(session.stdin_fd);
		session.stdin_closed = true;
		return true;
	}

	size_t remaining = session.input.length() - session.input_offset;
	size_t chunk = remaining > 8192 ? 8192 : remaining;
	ssize_t bytes_written = write(session.stdin_fd,
		session.input.c_str() + session.input_offset, chunk);

	if (bytes_written > 0)
	{
		session.input_offset += static_cast<size_t>(bytes_written);
		if (session.input_offset >= session.input.length())
		{
			closeSessionFd(session.stdin_fd);
			session.stdin_closed = true;
		}
		return true;
	}

	closeSessionFd(session.stdin_fd);
	session.stdin_closed = true;
	return false;
}

bool CgiHandler::readOutput(CgiSession& session)
{
	char buffer[8192];

	if (session.stdout_closed)
		return true;

	ssize_t bytes_read = read(session.stdout_fd, buffer, sizeof(buffer));
	if (bytes_read > 0)
	{
		session.output.append(buffer, bytes_read);
		return true;
	}

	closeSessionFd(session.stdout_fd);
	session.stdout_closed = true;
	return bytes_read == 0;
}

bool CgiHandler::updateProcessStatus(CgiSession& session)
{
	if (session.process_done)
		return true;

	pid_t result = waitpid(session.pid, &session.exit_status, WNOHANG);
	if (result == session.pid || result < 0)
		session.process_done = true;
	return session.process_done;
}

bool CgiHandler::hasTimedOut(const CgiSession& session, int timeout_seconds) const
{
	if (session.start_time == 0)
		return false;
	return std::time(NULL) - session.start_time > timeout_seconds;
}

void CgiHandler::killSession(CgiSession& session)
{
	if (session.pid > 0 && !session.process_done)
		kill(session.pid, SIGKILL);
	closeSessionFd(session.stdin_fd);
	closeSessionFd(session.stdout_fd);
	session.stdin_closed = true;
	session.stdout_closed = true;
	if (session.pid > 0)
		waitpid(session.pid, &session.exit_status, 0);
	session.process_done = true;
}

HttpResponse CgiHandler::finishSession(CgiSession& session, bool timed_out)
{
	closeSessionFd(session.stdin_fd);
	closeSessionFd(session.stdout_fd);
	session.stdin_closed = true;
	session.stdout_closed = true;
	updateProcessStatus(session);
	if (!session.process_done)
		return makeErrorResponse("500", "Internal Server Error", "CGI: process is still running");
	return parseCgiOutput(session.output, session.exit_status, timed_out);
}

std::vector<std::string> CgiHandler::buildEnvironment(const HttpRequest& req) const
{
	std::vector<std::string> env;
	const std::map<std::string, std::string>& headers = req.getHeaders();
	std::string uri = req.getUri().getUri();
	std::string query;
	size_t query_pos = uri.find('?');

	if (query_pos != std::string::npos)
		query = uri.substr(query_pos + 1);

	env.push_back("GATEWAY_INTERFACE=CGI/1.1");
	env.push_back("REQUEST_METHOD=" + req.getMethod());
	env.push_back("SCRIPT_FILENAME=" + script_path);
	env.push_back("SCRIPT_NAME=" + req.getUri().getPath());
	env.push_back("PATH_INFO=" + script_path);
	env.push_back("PATH_TRANSLATED=" + script_path);
	env.push_back("REQUEST_URI=" + uri);
	env.push_back("QUERY_STRING=" + query);
	env.push_back("SERVER_PROTOCOL=" + req.getHttpVersion());
	env.push_back("REDIRECT_STATUS=200");

	std::string content_length = getHeaderValue(headers, "Content-Length");
	if (!content_length.empty())
		env.push_back("CONTENT_LENGTH=" + content_length);

	std::string content_type = getHeaderValue(headers, "Content-Type");
	if (!content_type.empty())
		env.push_back("CONTENT_TYPE=" + content_type);

	for (std::map<std::string, std::string>::const_iterator it = headers.begin();
		 it != headers.end(); ++it)
	{
		std::string lower_key = toLowerString(it->first);
		if (lower_key == "content-length" || lower_key == "content-type")
			continue;
		env.push_back("HTTP_" + toUpperUnderscore(it->first) + "=" + it->second);
	}

	return env;
}

HttpResponse CgiHandler::parseCgiOutput(const std::string& output, int exit_status, bool timed_out)
{
	if (timed_out)
		return makeErrorResponse("504", "Gateway Timeout", "CGI: script timed out");
	if ((!WIFEXITED(exit_status) || WEXITSTATUS(exit_status) != 0) && output.empty())
		return makeErrorResponse("502", "Bad Gateway", "CGI: script failed");

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
		return makeErrorResponse("502", "Bad Gateway", "CGI: missing response headers");
	}

	// Extract headers
	std::map<std::string, std::string> headers = extractHeaders(header_part);

	// Default status is 200, unless CGI specifies "Status:" header
	std::string status_str = "200";
	std::string reason = "OK";

	std::map<std::string, std::string>::iterator status_it = findHeader(headers, "Status");
	std::map<std::string, std::string>::iterator location_it = findHeader(headers, "Location");
	std::map<std::string, std::string>::iterator content_type_it = findHeader(headers, "Content-Type");

	if (status_it != headers.end())
	{
		std::string status_line = status_it->second;
		size_t space = status_line.find(' ');
		if (space != std::string::npos)
		{
			status_str = status_line.substr(0, space);
			reason = status_line.substr(space + 1);
		}
		else
			status_str = status_line;
		headers.erase(status_it);  // Remove from headers, put in status line
	}
	else if (location_it != headers.end())
	{
		status_str = "302";
		reason = "Found";
	}
	if (reason == "OK" && status_str != "200")
		reason = defaultReasonPhrase(status_str);
	if (content_type_it == headers.end() && location_it == headers.end() && status_str != "204")
		return makeErrorResponse("502", "Bad Gateway", "CGI: missing Content-Type header");

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
	finalizeResponse(resp);

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

HttpResponse CgiHandler::makeErrorResponse(const std::string& status, const std::string& reason,
	const std::string& message) const
{
	HttpResponse resp;
	resp.setStatusCode(status);
	resp.setReasonPhrase(reason);
	resp.setBody(message);
	resp.addHeader("Content-Type", "text/plain");
	finalizeResponse(resp);
	return resp;
}

void CgiHandler::finalizeResponse(HttpResponse& resp) const
{
	if (!hasHeader(resp.getHeaders(), "Content-Length"))
	{
		std::ostringstream length;
		length << resp.getBody().length();
		resp.addHeader("Content-Length", length.str());
	}
	if (!hasHeader(resp.getHeaders(), "Connection"))
		resp.addHeader("Connection", "close");

	std::string raw = "HTTP/1.0 " + resp.getStatusCode() + " "
		+ resp.getReasonPhrase() + "\r\n";
	const std::vector<std::string>& headers = resp.getHeaders();
	for (std::vector<std::string>::const_iterator it = headers.begin();
		it != headers.end(); ++it)
		raw += *it + "\r\n";
	raw += "\r\n";
	raw += resp.getBody();
	resp.setResponse(raw);
}

void CgiHandler::closeSessionFd(int& fd) const
{
	if (fd != -1)
	{
		close(fd);
		fd = -1;
	}
}

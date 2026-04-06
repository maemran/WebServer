/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saabo-sh <saabo-sh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 13:33:58 by saabo-sh          #+#    #+#             */
/*   Updated: 2026/04/06 14:50:57 by saabo-sh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



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
		
		// Static: Map of file extensions to interpreter paths (supports multiple CGI types)
		static std::map<std::string, std::string> cgiInterpreters;
		
		// Helper: Build CGI environment variables
		void buildEnvironment(const HttpRequest& req, char** env);
		
		// Helper: Parse CGI response headers
		HttpResponse parseCgiOutput(const std::string& output);
		
		// Helper: Extract headers from raw output
		std::map<std::string, std::string> extractHeaders(const std::string& raw);
		
		// Helper: Extract file extension from script path (e.g., ".py", ".pl")
		std::string getFileExtension(const std::string& path) const;
		
		// Helper: Determine the correct interpreter based on script extension
		std::string getInterpreterForScript(const std::string& script) const;
		
		// Helper: Check if file exists and is executable
		bool fileExists(const std::string& path) const;

	public:
		CgiHandler();
		CgiHandler(const std::string& script, const std::string& interpreter);
		~CgiHandler();

		// Static: Initialize CGI type mappings (call this once at startup)
		static void initializeCgiInterpreters();
		
		// Static: Register a new CGI type (extension -> interpreter mapping)
		static void registerCgiType(const std::string& extension, const std::string& interpreter);
		
		// Static: Get registered interpreter for an extension
		static std::string getCgiInterpreter(const std::string& extension);

		// Execute CGI script with given request, return response
		// Now automatically detects script type and uses appropriate interpreter
		HttpResponse execute(const HttpRequest& req);
};

#endif

#pragma once

#include "CGI_utils.hpp"

namespace	ws
{
	struct Location // I give up, I don't know how to do it without redeclaring
	{
		std::string path;
		std::map<std::string, t_vec_str> config;
	};

	// only one per binary (php, python, ruby, etc.) from which run() is called with the script directory? not sure
	class CGI
	{

		private:
			std::map<std::string, std::string>	envp;
			std::string							name;
			std::string							binPath;

		public:
			CGI() {}
			~CGI() {}

			int	run(std::string cgiPath)
			{

				if (!file_exists(cgiPath))
					return 1; // file not found?

				// setup the environment variables
				envp.insert(std::make_pair<std::string, std::string>("SERVER_NAME", "should be the server name"));
				envp.insert(std::make_pair<std::string, std::string>("SERVER_SOFTWARE", "should be the server software"));
				envp.insert(std::make_pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
				envp.insert(std::make_pair<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
				envp.insert(std::make_pair<std::string, std::string>("SERVER_PORT", "should be the server port"));
				envp.insert(std::make_pair<std::string, std::string>("REQUEST_METHOD", "should be the request method"));
				envp.insert(std::make_pair<std::string, std::string>("CONTENT_TYPE", "should be the content type"));
				envp.insert(std::make_pair<std::string, std::string>("CONTENT_LENGTH", "should be the content length"));
				envp.insert(std::make_pair<std::string, std::string>("PATH_INFO", "should be the path requested"));
				envp.insert(std::make_pair<std::string, std::string>("PATH_TRANSLATED", "should be the full path"));
				envp.insert(std::make_pair<std::string, std::string>("SCRIPT_NAME", "should be the script name"));
				envp.insert(std::make_pair<std::string, std::string>("QUERY_STRING", "should be the query string"));
				// and probably some other env vars to setup the sameway .....
				
				
				return this->exec(cgiPath);
			}

		private:
			int	exec(std::string cgiPath)
			{
				pid_t	pid = fork();

				if (pid > 0)
					return 1;
				if (!pid)
				{
					int	ret = execle(binPath.c_str(), binPath.c_str(), cgiPath.c_str(), NULL, map_to_envp(envp));
					exit(1); // execve failed
				}
				int	status;
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					return WEXITSTATUS(status);
				return 0;
			}
			// std::string const &getCGI() const;
	};
}
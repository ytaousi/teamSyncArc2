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
			std::map<std::string, std::string>	envpMap;
			char								**envp;
			char								**args; // still need to fill args...
			std::string							name;
			std::string							binPath;
			std::string							cgiOutputFile;
			std::string							cgiInputFile;
			int									cgiInputFd;
			int									cgiOutputFd;
			int									shouldDupInputAlso; // Because its POST OR DELETE with body

		public:
			CGI()
			{

			}
			~CGI() {}

			int	run(std::string cgiPath)
			{

				if (!file_exists(cgiPath))
					return 1; // file not found?

				// setup the environment variables
				envpMap.insert(std::make_pair<std::string, std::string>("SERVER_NAME", "should be the server name"));
				envpMap.insert(std::make_pair<std::string, std::string>("SERVER_SOFTWARE", "should be the server software"));
				envpMap.insert(std::make_pair<std::string, std::string>("GATEWAY_INTERFACE", "CGI/1.1"));
				envpMap.insert(std::make_pair<std::string, std::string>("SERVER_PROTOCOL", "HTTP/1.1"));
				envpMap.insert(std::make_pair<std::string, std::string>("SERVER_PORT", "should be the server port"));
				envpMap.insert(std::make_pair<std::string, std::string>("REQUEST_METHOD", "should be the request method"));
				envpMap.insert(std::make_pair<std::string, std::string>("CONTENT_TYPE", "should be the content type"));
				envpMap.insert(std::make_pair<std::string, std::string>("CONTENT_LENGTH", "should be the content length"));
				envpMap.insert(std::make_pair<std::string, std::string>("PATH_INFO", "should be the path requested"));
				envpMap.insert(std::make_pair<std::string, std::string>("PATH_TRANSLATED", "should be the full path"));
				envpMap.insert(std::make_pair<std::string, std::string>("SCRIPT_NAME", "should be the script name"));
				envpMap.insert(std::make_pair<std::string, std::string>("QUERY_STRING", "should be the query string"));
				// and probably some other env vars to setup the sameway .....
				

				return this->exec(cgiPath);
			}

		private:
			int	exec(std::string cgiPath)
			{
				// pid_t	pid = fork();

				// if (pid > 0)
				// 	return 1;
				// if (!pid)
				// {
				// 	int	ret = execle(binPath.c_str(), binPath.c_str(), cgiPath.c_str(), NULL, map_to_envp(envp));
				// 	exit(1); // execve failed
				// }
				// int	status;
				// waitpid(pid, &status, 0);
				// if (WIFEXITED(status))
				// 	return WEXITSTATUS(status);
				// return 0;

				pid_t	pid;
				int		status;

				pid = fork();
				if (pid < 0)
				{
					perror("cgi fork failed");
					exit(1);
				}
				if (pid == 0)
				{
					// if request method is POST we need to dup2 the input also
					if (shouldDupInputAlso == 1)
					{
						if ((cgiInputFd = open(cgiInputFile.c_str(), O_RDONLY)) < 0)
						{
							perror("error oppenning inputfile");
							exit(1);
						}
						if (dup2(cgiInputFd, 0) == -1)
						{
							perror("error dup2 input");
							exit(1);
						}
					}
					if ((cgiOutputFd = open(cgiOutputFile.c_str(), O_WRONLY)) < 0)
					{
						perror("error oppenning outputfile");
						exit(1);
					}
					if (dup2(cgiOutputFd, 1) == -1)
					{
						perror("error dup2 output");
						exit(1);
					}
					// still need to fill the args...
					if (execve(binPath.c_str(), args, map_to_envp(envpMap)) < 0)
					{
						perror("execve katke3ke3");
						exit(1);
					}
				}
				else
				{
					waitpid(pid, &status, 0);
					if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_FAILURE)
					{
						perror("cgi process failed");
						exit(1);
					}
				}
			}
			// std::string const &getCGI() const;
	};
}
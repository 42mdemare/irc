#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "config.h"

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE  	"\033[0;34m"

class Client {
	private:
		int			_client_fd;
		int			_portClient;
		bool		_isAuthenticated;
		bool		_passAuth;
		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _buffer;
		std::string _hostClient;

		Client();

	public:
		Client(int fd, std::string hostClient, int portClient);
		~Client();
		Client(const Client &copy);
		Client &operator=(const Client &src);

		std::string	getNickname() const;
		std::string	getUsername() const;
		std::string	getRealname() const;
		std::string	getBuffer();
		bool		getIsAuthenticated() const;
		bool		getIsPass() const;
		int			getFd() const;
		std::string	getHostClient() const;
		int			getPortClient() const;
		std::string	getPrefix() const;

		void 		setNickname(std::string nickname);
		void 		setUsername(std::string username);
		void 		setRealname(std::string realname);
		void 		setBuffer(std::string buffer);
		void 		unsetBuffer();
		void 		setIsAuthenticated(bool isAuthenticated);
		void 		setIsPass(bool isPass);
};

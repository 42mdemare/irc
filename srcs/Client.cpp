#include "../includes/Client.hpp"

#pragma region "FORME_CANONIQUE"

Client::Client(int fd, std::string hostClient, int portClient) {
	if (DEBUG_MODE)
		std::cout << GREEN << "Default constructor Client called" << RESET << std::endl;
	_client_fd = fd;
	_isAuthenticated = false;
	_passAuth = false;
	_hostClient = hostClient;
	_portClient = portClient;
}

Client::~Client() {
	if (DEBUG_MODE)
		std::cout << RED << "Default destructor Client called" << RESET << std::endl;
}

Client::Client(const Client &copy) {
	if (DEBUG_MODE)
		std::cout << BLUE << "Client Copy Constructor called" << RESET << std::endl;
	_nickname = copy._nickname;
	_username = copy._username;
	_realname = copy._realname;
	_buffer = copy._buffer;
	_isAuthenticated = copy._isAuthenticated;
	_passAuth = copy._passAuth;
	_client_fd = copy._client_fd;
	_portClient = copy._portClient;
	_hostClient = copy._hostClient;
}

Client &Client::operator=(const Client &src) {
	if (DEBUG_MODE)
		std::cout << BLUE << "Client Assigment operator called" << RESET << std::endl;
	if (this != &src) {
		_nickname = src._nickname;
		_username = src._username;
		_realname = src._realname;
		_buffer = src._buffer;
		_isAuthenticated = src._isAuthenticated;
		_passAuth = src._passAuth;
		_client_fd = src._client_fd;
		_portClient = src._portClient;
		_hostClient = src._hostClient;
	}
	return *this;
}

#pragma endregion

#pragma region "GUETTERS"

std::string	Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getRealname() const { return _realname; }
std::string Client::getBuffer() { return _buffer; }
bool 		Client::getIsAuthenticated() const { return _isAuthenticated; }
bool 		Client::getIsPass() const { return _passAuth; }
int 		Client::getFd() const { return _client_fd; }
std::string Client::getHostClient() const { return _hostClient; }
int			Client::getPortClient() const { return _portClient; }
std::string Client::getPrefix() const { return _nickname + "!" + _username + "@" + _hostClient; }

#pragma endregion 

#pragma region "SETTERS"

void Client::setNickname(std::string nickname) { _nickname = nickname; }
void Client::setUsername(std::string username) { _username = username; }
void Client::setRealname(std::string realname) { _realname = realname; }
void Client::setBuffer(std::string buffer) { _buffer += buffer; }
void Client::unsetBuffer() { _buffer.clear(); }
void Client::setIsAuthenticated(bool isAuthenticated) { _isAuthenticated = isAuthenticated; }
void Client::setIsPass(bool isPass) { _passAuth = isPass; }

#pragma endregion 

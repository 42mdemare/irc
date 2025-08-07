#include "../includes/Server.hpp"

#pragma region "FORME_CANONIQUE"

Server::Server(std::string port, std::string password) {
	if (DEBUG_MODE) {
		std::cout << GREEN << "Default constructor Server called" << RESET << std::endl;
		std::cout << "PORT = " << port << std::endl;
		std::cout << "PASSWORD = " << password << std::endl;
		std::cout << "g_shutdown = " << g_shutdown << std::endl;
	}

	if (chkDigit(port) == 1) {
		g_shutdown = 1;
		throw WrongPort();
	}
	_port = atol(port.c_str());
	_password = password;
	_maxClients = 1024;

	initializeCommands();
	serverCreateAndConnect();
	createChannel("#general", "Open Discussion", NULL);
}

Server::~Server() {
	if (DEBUG_MODE)
		std::cout << RED << "Default destructor Server called" << RESET << std::endl;
	std::map<std::string, Channel*>::iterator it;
	for (it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second)
			delete it->second;
	}
}

Server::Server(const Server &copy) {
	if (DEBUG_MODE)
		std::cout << BLUE << "Server Copy Constructor called" << RESET << std::endl;
	this->_port = copy._port;
	this->_password = copy._password;
	this->_sockfd = copy._sockfd;
	this->_channels = copy._channels;
	this->_clients = copy._clients;
	this->_maxClients = copy._maxClients;
}

Server &Server::operator=(const Server &src) {
	if (DEBUG_MODE)
		std::cout << BLUE << "Server Assigment operator called" << RESET << std::endl;
	if (this != &src) {
		this->_port = src._port;
		this->_password = src._password;
		this->_sockfd = src._sockfd;
		this->_channels = src._channels;
		this->_clients = src._clients;
		this->_maxClients = src._maxClients;
	}
	return *this;
}
#pragma endregion

#pragma region "GETTERS"

int Server::getSockFd() const { return _sockfd; }

std::string Server::getPassword() const { return _password; }

Client*	Server::getClient(const int client_fd) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getFd() == client_fd) {
			return (_clients[i]);
		}
	}
	return NULL;
}

const std::vector<Client*>& Server::getClients() const { return _clients; }

std::map<std::string, Channel*>	Server::getChannels() const { return _channels; }

Channel*	Server::getChannel(const std::string& name) {
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

unsigned long Server::getMaxclients() { return _maxClients; }

#pragma endregion

#pragma region "SERVER"

void Server::initializeCommands() {
	_commands["PASS"]		= &handlePASS;
	_commands["NICK"]		= &handleNICK;
	_commands["USER"]		= &handleUser;
	_commands["PING"]		= &handlePong;
	_commands["JOIN"]		= &handleJoin;
	_commands["KICK"]		= &handleKick;
	_commands["PRIVMSG"]	= &handlePrivmsg;
	_commands["MODE"]		= &handleMode;
	_commands["INVITE"] 	= &handleInvite;
	_commands["PART"] 		= &handlePart;
	_commands["TOPIC"] 		= &handleTopic;
	_commands["WHO"] 		= &handleWho;

}

void	Server::serverCreateAndConnect() {
	// Creer un socket serveur TCP pret a accepter les connexions entrantes
		// AF_INET: protocole IPv4
		// SOCK_STREAM: socket de type flux = TCP (Transmission Control Protocol)
		// 0: protocole par defaut = TCP
	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_sockfd < 0) {
		g_shutdown = 1;
		throw ConnectionFailure();
	}

	// On met le socket du serveur en mode non-bloquant.
	// Si ca echoue, le serveur ne peut pas demarrer correctement.
	if (fcntl(_sockfd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << RED << "Error on server socket: fcntl()" << RESET << std::endl;
		throw ConnectionFailure(); 
	}

	// Structure contenant adresse IP et port du serveur
	sockaddr_in	serverAddr;

	//Initialiser la structure a 0 pour eviter valeurs indefinies
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(_port); // Conversion du port en ordre reseau (grand endian)
	serverAddr.sin_addr.s_addr = INADDR_ANY; // Accepte connexions de toutes les adresses disponibles

	// Associer socket avec addresse IP et port definis dans serverAddr (reserve ce port pour ce socket)
	if (bind(_sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		g_shutdown = 1;
		throw ConnectionFailure();
	}
	// Mettre le socket en mode ecoute, pret a accepter les connexions entrantes (SOMAXCONN = maximum de connexions en file d'attente)
	if (listen(_sockfd, SOMAXCONN) < 0) {
		g_shutdown = 1;
		throw ConnectionFailure();
	}
	std::cout << GREEN << "IRC server connected on port " << _port << "." << RESET << std::endl;
}

void	Server::closeServer() {
	// Fermer connexions client
	for (size_t i = 0; i < _clients.size(); i++) {
		if (DEBUG_MODE)
			std::cout << RED << "Closing client #" << _clients[i]->getFd() << "." << RESET << std::endl;
		close(_clients[i]->getFd());
		delete _clients[i];
	}
	_clients.clear();

	// Fermer socket serveur
	if (_sockfd != -1) {
		if (DEBUG_MODE)
			std::cout << RED << "Closing server." << RESET << std::endl;
		close(_sockfd);
	}
}

#pragma endregion

#pragma region "CHANNEL"

std::string	Server::channelNameIsCorrect(std::string name) {
	if (name[0] != '#')
		throw std::invalid_argument("Channel name needs to start with # symbol");
	for (size_t i = 1; i < name.size(); ++i) {
		if (isForbidden(name[i])) {
			std::string error = "Channel name has forbidden character: " + std::string(1, name[i]);
			throw std::invalid_argument(error);
		}
		name[i] = tolower(name[i]);
	}
	if(name.size() >= 50)
		throw std::invalid_argument("Channel name must be less than 50 characters");
	return name;
}

void	Server::createChannel(const std::string& name, const std::string& topic, Client* author) {
	try {
		std::string channelName = channelNameIsCorrect(name);
		
		std::map<std::string, Channel*>::iterator it;
		for (it = _channels.begin(); it != _channels.end(); ++it) {
			if (it->first == name) {
				throw std::invalid_argument("Channel name already exists");
			}
		}

		Channel*	chan = new Channel(channelName, topic);
		_channels[channelName] = chan;

		std::time_t now = std::time(0);
		std::tm* localtime = std::localtime(&now);
		std::ostringstream oss;
		oss << std::setfill('0')
			<< std::setw(2) << localtime->tm_mday << "/"
			<< std::setw(2) << (localtime->tm_mon + 1) << "/"
			<< (localtime->tm_year + 1900) << " "
			<< std::setw(2) << localtime->tm_hour << ":"
			<< std::setw(2) << localtime->tm_min << ":"
			<< std::setw(2) << localtime->tm_sec;
		std::string date = oss.str();

		chan->setCreationTime(date);

		if (!author)
			chan->setAuthor("server");
		else
			chan->setAuthor(author->getNickname());
	} catch (std::exception &e) {
		if (author)
			sendMessage(*author, std::string(e.what()) + "\r\n");
		if (DEBUG_MODE)
			std::cerr << RED << "Channel name is incorrect: " << e.what() << RESET << std::endl;
	}
}

void	Server::createChannelWithoutTopic(const std::string& name, Client* author) {
	try {
		std::string channelName = channelNameIsCorrect(name);

		std::map<std::string, Channel*>::iterator it;
		for (it = _channels.begin(); it != _channels.end(); ++it) {
			if (it->first == name) {
				throw std::invalid_argument("Channel name already exists");
			}
		}

		Channel*	chan = new Channel(channelName);
		_channels[channelName] = chan;

		std::time_t now = std::time(0);
		std::tm* localtime = std::localtime(&now);
		std::ostringstream oss;
		oss << std::setfill('0')
			<< std::setw(2) << localtime->tm_mday << "/"
			<< std::setw(2) << (localtime->tm_mon + 1) << "/"
			<< (localtime->tm_year + 1900) << " "
			<< std::setw(2) << localtime->tm_hour << ":"
			<< std::setw(2) << localtime->tm_min << ":"
			<< std::setw(2) << localtime->tm_sec;
		std::string date = oss.str();

		chan->setCreationTime(date);

		if (!author)
			chan->setAuthor("server");
		else
			chan->setAuthor(author->getNickname());
	} catch (std::exception &e) {
		if (author)
			sendMessage(*author, std::string(e.what()) + "\r\n");
		if (DEBUG_MODE)
			std::cerr << RED << "Channel name is incorrect: " << e.what() << RESET << std::endl;
	}
}

void	Server::deleteChannel(const std::string& name) {
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it != _channels.end()) {
		delete it->second;
		_channels.erase(it);
	}
}

void	Server::leaveChannel(const std::string& name, Client& client) {
	try {
		Channel* chan = this->getChannel(name);
		if (chan)
			chan->leave(&client);
		else 
			throw Channel::ChannelError();
	} catch (Channel::ChannelError &e) {
		if (DEBUG_MODE)
			std::cerr << RED << e.what() << RESET << std::endl;
	}
}

#pragma endregion

#pragma region "HANDLE_CLIENT"

void	Server::handleNewClient() {
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	// Accepte nouvelle connexion
	int client_fd = accept(_sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd < 0) {
		std::cerr << RED << "Client connection error" << RESET << std::endl;
		return;
	}

	// Vérifier le nombre de clients connectés
	if (_clients.size() >= _maxClients) {
		std::string msg = "ERROR :Server full. Try again later.\r\n";
		send(client_fd, msg.c_str(), msg.length(), 0);
		close(client_fd);
		return;
	}

	// On met le socket de ce client en mode non-bloquant
	if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
		// Si ca echoue, on ne peut pas gerer ce client. On le ferme et on l'ignore
		std::cerr << RED << "Error fcntl() on socket client #" << client_fd << RESET << std::endl;
		close(client_fd);
		return;
	}
	std::cout	<< GREEN
				<< "New client " << client_fd
				<< " from " << inet_ntoa(client_addr.sin_addr)
				<< ":" << ntohs(client_addr.sin_port) 
				<< RESET << std::endl;

	Client* client = new Client(client_fd, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	_clients.push_back(client);
}

bool Server::handleClientMessage(int client_fd) {
	char buffer[1024];
	ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

	// Si read renvoie 0 ou moins, le client s'est deconnecte
	if (bytes_read <= 0) {
		std::cout << RED << "Client #" << client_fd << " disconnected." << RESET << std::endl;
		removeClient(client_fd);
		return false;
	}

	buffer[bytes_read] = '\0';
	if (DEBUG_MODE)
		std::cout << "Message recieved from client # :" << client_fd << ": " << "'" << buffer << "'" << std::endl;

	Client* client = getClient(client_fd);
	if (!client)
		return false;

	client->setBuffer(buffer);
	std::string					buffToProcess;
	if (buffToProcess.size() >= 510)
		buffToProcess = client->getBuffer().substr(0, 510) + "\r\n";
	else 
		buffToProcess = client->getBuffer();
	if (buffToProcess.find('\n') == std::string::npos)
		return (false);

	std::vector<std::string>	commands = splitCmd(buffToProcess);
	client->unsetBuffer();

	for (std::vector<std::string>::iterator it_cmd = commands.begin(); it_cmd != commands.end(); ++it_cmd) {
		if (it_cmd->empty()) continue;

		if (DEBUG_MODE)
			std::cout << "Processing from client #" << client_fd << ": '" << *it_cmd << "'" << std::endl;
		std::vector<std::string> tokens = tokenizer(*it_cmd);
		if (tokens.empty()) continue;

		std::string commandName = tokens[0];
		for (size_t i = 0; i < commandName.length(); ++i)
			commandName[i] = std::toupper(commandName[i]);
		
		// Recherche de la commande dans notre map de fonctions handle
		std::map<std::string, CommandFunction>::iterator it_handler = _commands.find(commandName);

		if (it_handler != _commands.end()) {
			if (!client->getIsAuthenticated() && commandName != "PASS" && commandName != "NICK" && commandName != "USER" && commandName != "CAP") {
				sendMessage(*client, ":localhost 451 " + client->getNickname() + " :You have not registered\r\n");
				continue; // On passe a la commande suivante sans rien faire d'autre
			}
			// La commande existe, on appelle la fonction correspondante
			CommandFunction handler = it_handler->second;
			handler(*this, *client, tokens);
		}
		// Gerer CAP (CAP END est juste ignore)
		else if (commandName == "CAP") {
			if (tokens.size() > 1 && tokens[1] == "LS")
				sendMessage(*client, ":localhost CAP * LS :\r\n");
		}
		else if (commandName == "QUIT") {
			std::cout << RED << "Client #" << client_fd << " disconnected." << RESET << std::endl;
			removeClient(client->getFd());
			return false; 
		} else // Commande inconnue
			sendMessage(*client, ":localhost 421 " + client->getNickname() + " " + commandName + " :Unknown command\r\n");
	}
	return true;
}

#pragma endregion

#pragma region "FOR_CLIENT"

void Server::removeClient(int client_fd) {
	Client* client_to_remove = NULL;
	int client_index = -1;

	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getFd() == client_fd) {
			client_to_remove = _clients[i];
			client_index = i;
			break;
		}
	}
	// Si le client n'a pas ete trouve, on arrete.
	if (!client_to_remove) {
		std::cerr << RED << "Error: attempt to delete unkown client (fd: " << client_fd << ")" << RESET << std::endl;
		return;
	}
	// On parcourt la map de tous les canaux du serveur
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		Channel* current_channel = it->second;
		// On verifie s'il est membre de ce canal.
		if (current_channel->isPartOfChannel(client_to_remove->getNickname())) {
			current_channel->leave(client_to_remove);
			if (DEBUG_MODE)
				std::cout << "Client " << client_to_remove->getNickname() << " removed from canal " << current_channel->getName() << std::endl;
		}
	}
	close(client_fd);
	delete client_to_remove;
	// Retirer le client du vecteur _clients
	if (client_index != -1)
		_clients.erase(_clients.begin() + client_index);

	if (DEBUG_MODE)
		std::cout << RED << "Client fd #" << client_fd << " deleted." << RESET << std::endl;
}

Client*	Server::getClient(int client_fd, std::string nickname, std::string username) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		if (_clients[i]->getFd() == client_fd || _clients[i]->getNickname() == nickname || _clients[i]->getUsername() == username) {
			if (DEBUG_MODE) {
				std::cout << "getClient  " << client_fd << " " << _clients[i]->getHostClient() << ":" << _clients[i]->getPortClient() << std::endl;
				std::cout << "getClient fd: '" << _clients[i]->getFd() << "'" << std::endl;
				std::cout << "getClient nickname: '" << _clients[i]->getNickname() << "'" << std::endl;
				std::cout << "getClient username: '" << _clients[i]->getUsername() << "'" << std::endl;
				std::cout << "getClient isAuthenticated: '" << _clients[i]->getIsAuthenticated() << "'" << std::endl;
			}
			return _clients[i];
		}
	}
	return NULL;
}

Client*	Server::getClientByNickname(const std::string& nickname) {
	for (size_t i = 0; i < _clients.size(); ++i) {
		 if (_clients[i]->getNickname() == nickname) {
			return _clients[i];
		}
	}
	return NULL;
}

void	Server::addUserInfo(int client_fd, std::string nickname, std::string username, std::string hostClient, int portClient) {
	Client* client = getClient(client_fd, nickname, username);
	if (client && client->getNickname().empty() && client->getUsername().empty()) {
		client->setNickname(nickname);
		client->setUsername(username);
		client->setIsAuthenticated(false);

		if (DEBUG_MODE) {
			std::cout << "addUserInfo  " << client_fd << " " << hostClient << ":" << portClient << std::endl;
			std::cout << "addUserInfo fd: '" << client->getFd() << "'" << std::endl;
			std::cout << "addUserInfo nickname: '" << client->getNickname() << "'" << std::endl;
			std::cout << "addUserInfo username: '" << client->getUsername() << "'" << std::endl;
			std::cout << "addUserInfo isAuthenticated: '" << client->getIsAuthenticated() << "'" << std::endl;
		}
	}
}

void	Server::updateUserInfo(int client_fd, std::string nickname, std::string username, bool isAuthenticated, std::string hostClient, int portClient) {
	Client* client = getClient(client_fd, nickname, username);
	if (client) {
		client->setNickname(nickname);
		client->setUsername(username);
		client->setIsAuthenticated(isAuthenticated);

		if (DEBUG_MODE) {
			std::cout << "updateUserInfo  " << client_fd << " " << hostClient << ":" << portClient << std::endl;
			std::cout << "updateUserInfo fd: '" << client->getFd() << "'" << std::endl;
			std::cout << "updateUserInfo nickname: '" << client->getNickname() << "'" << std::endl;
			std::cout << "updateUserInfo username: '" << client->getUsername() << "'" << std::endl;
			std::cout << "updateUserInfo isAuthenticated: '" << client->getIsAuthenticated() << "'" << std::endl;
		}
	}
}

#pragma endregion

#pragma region "EXCEPTIONS"

const char*	Server::WrongPort::what() const throw () { return "Port is not a valid number"; }
const char*	Server::ConnectionFailure::what() const throw() { return "Cannot connect to the server"; }

#pragma endregion
#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h> // pour sockaddr_in
#include <arpa/inet.h>  // pour inet_addr(), inet_ntoa et ntohs
#include <unistd.h>     // pour close()
#include <signal.h>
#include <cstring>      // pour memset
#include <fcntl.h>
#include <poll.h>
#include <cerrno>

#include "Client.hpp"
#include "Channel.hpp"
#include "CommandHandler.hpp"
#include "config.h"

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"

extern volatile sig_atomic_t    g_shutdown;

class Channel;
class Server;
class Client;

typedef bool (*CommandFunction)(Server&, Client&, const std::vector<std::string>&); // Type "CommandFunction": pointeur vers fonction handle

class Server {
    private:
        unsigned int                            _port;
        std::string                             _password;
        int                                     _sockfd;
        unsigned long                           _maxClients;
        std::vector<Client*>                    _clients;
        std::map<std::string, Channel*>         _channels;
        std::map<std::string, CommandFunction>  _commands;

        void initializeCommands();
        Server();

    public:
        Server(std::string port, std::string password);
        ~Server();
        Server(const Server &copy);
        Server &operator=(const Server &src);

        int                             getSockFd() const;
        std::string                     getPassword() const;
        const std::vector<Client*>&     getClients() const;
        Client*                         getClient(const int client_fd);
        Client*                         getClient(int client_fd, std::string nickname, std::string username);
        Client*                         getClientByNickname(const std::string& nickname);
        std::map<std::string, Channel*> getChannels() const;
        Channel*	                    getChannel(const std::string& name);
        unsigned long                   getMaxclients();

        void                            serverCreateAndConnect();
        void                            closeServer();

        void    	                    createChannel(const std::string& name, const std::string& topic, Client* author);
        void	                        createChannelWithoutTopic(const std::string& name, Client* author);
        void		                    deleteChannel(const std::string& name);
        void		                    leaveChannel(const std::string& name, Client& client);
        std::string                     channelNameIsCorrect(std::string name);

        void                            handleNewClient();
        bool                            handleClientMessage(int client_fd);
        void                            removeClient(int client_fd);
        void                            addUserInfo(int client_fd, std::string nickname, std::string username, std::string hostClient, int portClient);
        void                            updateUserInfo(int client_fd, std::string nickname, std::string username, bool isAuthenticated, std::string hostClient, int portClient);

        class WrongPort : public std::exception { public: virtual const char* what() const throw ();};
        class ConnectionFailure : public std::exception { public: virtual const char* what() const throw();};
};

#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "Client.hpp"
#include "CommandHandler.hpp"
#include "config.h"

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE 	"\033[0;34m"

class Server;

class Channel {
	private:
		std::string					_name;
		std::string					_topic;
		std::string					_type;
		std::string					_author;
		std::string					_creationTime;
		std::map<std::string, bool>	_modes;
		std::map<Client*, int>		_usersMap;
		std::vector<Client*>		_invited;
		ssize_t						_nbMaxUser;
		std::string					_key;
		Channel();

	public:
		Channel(std::string name);	
		~Channel();
		Channel(std::string name, std::string topic);
		Channel(const Channel &copy);
		Channel &operator=(const Channel &src);

		std::string					getName();
		std::string					getTopic();
		std::map<std::string, bool>	getModes();
		std::map<Client*, int>		getUsersMap();
		std::vector<Client*>		getInvited();
		ssize_t						getNbMaxUser();
		int							getLevel(Client* c);
		std::string					getCreationTime();
		std::string					getAuthor();
		std::string					getType();
        Client*                     getClientByNickname(const std::string& nickname);
		int							getSpecificMode(std::string mode);
		std::string					getKey();
		std::string					getNicknameWithPrefix(Client* c);

		void						setName(std::string newName);
		void						setTopic(std::string newTopic);
		void						setMode(std::string mode, bool state);
		void						setType(std::string newType);
		void						setCreationTime(std::string newCreationTime);
		void						setAuthor(std::string newAuthor);
		void						setLevel(Client* c, int level);
		void						setNbMaxUser(ssize_t n);
		void						setInvited(Client* c);
		void						unsetInvited(Client *c);
		void						setKey(std::string newKey);
		void						unsetKey();

		bool						isInvited(Client* c);
		void						join(Client* client);
		void						leave(Client* client);
		void						checkOperatorPresence();
		void						sendMessageToChannel(std::string& message);
		std::string					userListCreate();
		bool						isPartOfChannel(std::string name);
		bool						hasClient(Client* client) const;
		void						sendMessageToAllTheOthers(const std::string& message, Client* notToThisUser);

		class	ChannelError : public std::exception { public : virtual const char* what() const throw(); };
};

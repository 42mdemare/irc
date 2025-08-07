#include "../../includes/CommandHandler.hpp"

bool	hasKey(Channel* chan) {
	if (chan->getKey() == "") 	{
		if (DEBUG_MODE)
			std::cout << "no key" << std::endl;
		return false;
	}
	return true;
}

bool	checkKey(Channel* chan, std::string key) {
	if (DEBUG_MODE)
		std::cout << "chan key = " << chan->getKey() << " key = " << key << std::endl;

	if (chan->getKey() != key) {
		if (DEBUG_MODE)
			std::cout << "wrong key" << std::endl;
		return false;
	}
	return true;
}

int		userChannelCount(Channel* chan) {
	std::map<Client*, int> userMap = chan->getUsersMap();
	int nbUsers = userMap.size();
	if (DEBUG_MODE)
		std::cout << "nbUsers = " << nbUsers << std::endl;
	return nbUsers;
}

bool handleJoin(Server &server, Client &c, const std::vector<std::string> &token) {
    if (token.size() < 2) {
        sendMessage(c, ERR_NEEDMOREPARAMS(token[0]));
        return false;
    }
	if (token[1] == "0") {
		leaveAllChannels(c, server);
		return true;
	}

	std::string channelToJoin = token[1];
	for (size_t i = 0; i < channelToJoin.size(); ++i)
		channelToJoin[i] = tolower(channelToJoin[i]);

	Channel* chan = server.getChannel(channelToJoin);
	if (chan) {
		if (chan->hasClient(&c)) {
			sendMessage(c, ERR_USERONCHANNEL(chan->getNicknameWithPrefix(&c), chan->getNicknameWithPrefix(&c), chan->getName()));
			return false;
		}
		if (chan->getSpecificMode("+l") == true) {
			if (DEBUG_MODE)
				std::cout << RED << "l active" << RESET << std::endl;
			if (userChannelCount(chan) >= chan->getNbMaxUser()) {
				sendMessage(c, ERR_CHANNELISFULL(c.getNickname(), channelToJoin));
				return false;	
			}
		}
		if (chan->getSpecificMode("+k") == true) {
			if (DEBUG_MODE)
				std::cout << RED << "k active" << RESET << std::endl;
			if (hasKey(chan)) {
				if (DEBUG_MODE)
					std::cout << RED << "hasKey" << RESET << std::endl;
				if (token.size() == 3) {
					if (DEBUG_MODE) {
						std::cout << RED << "token.size() == 3" << RESET << std::endl;
						std::cout << RED << "token[2] = " << token[2] << RESET << std::endl;
					}
					if (!checkKey(chan, token[2])) {
						if (DEBUG_MODE)
							std::cout << RED << "wrong key" << RESET << std::endl;
						sendMessage(c, ERR_BADCHANNELKEY(c.getNickname(), channelToJoin));
						return false;
					}
				} else if (token.size() > 3) {
					sendMessage(c, ERR_TOOMANYPARAMS());
					return false;
				} else {
					sendMessage(c, ERR_NEEDMOREPARAMS(token[0]));
					return false;
				}
			}
		}
	}
	joinChannel(channelToJoin, c, server);
	return true;
}

void	joinChannel(const std::string& constChannelToJoin, Client& client, Server& server) {
	if (DEBUG_MODE)
		std::cout << "joinChannel(), std::string channelToJoin = " << constChannelToJoin << std::endl;

	std::string channelToJoin = constChannelToJoin;
	for (size_t i = 0; i < channelToJoin.size(); ++i)
		channelToJoin[i] = tolower(channelToJoin[i]);
	Channel* chan = server.getChannel(channelToJoin);

	if (chan) {
		std::map<Client*, int> clients = chan->getUsersMap();
		std::map<Client*, int>::iterator it;

		if (chan->getSpecificMode("+i")) {
			if (!chan->isInvited(&client)) {
				sendMessage(client, ERR_INVITEONLYCHAN(client.getNickname(), chan->getName()));
				return ;
			}
		}
		// Parcourir les users de ce canal
		for (it = clients.begin(); it != clients.end(); ++it) {
			// Si client deja dans ce canal, lui envoyer un message d'erreur
			if ((it->first)->getNickname() == client.getNickname()) {
				sendMessage(client, ERR_USERONCHANNEL(chan->getNicknameWithPrefix(&client), chan->getNicknameWithPrefix(&client), chan->getName()));
				return ;
			}
		}
		try {
			chan->join(&client);
		} catch (Channel::ChannelError &e) {
			if (DEBUG_MODE)
				std::cerr << RED << e.what() << RESET << std::endl;
		}
	} else { // Creer le canal puis le rejoindre
		server.createChannelWithoutTopic(channelToJoin, &client);
		chan = server.getChannel(channelToJoin);
		if (chan) {
			try {
				chan->join(&client);
			} catch (Channel::ChannelError &e) {
				if (DEBUG_MODE)
					std::cerr << RED << "Could not join a channel newly created with command JOIN: " << e.what() << RESET << std::endl;
			}
		}
	}
}

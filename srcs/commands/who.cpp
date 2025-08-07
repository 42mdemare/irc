#include "../../includes/CommandHandler.hpp"

bool	handleWho(Server &s, Client &c, const std::vector<std::string> &token) {
	if (token.size() < 2) {
		std::string message = "WHO";
		sendMessage(c, ERR_NEEDMOREPARAMS(message));
		return false;
	}

	std::string channelToFind = token[1];
	for (size_t i = 0; i < channelToFind.size(); ++i)
		channelToFind[i] = tolower(channelToFind[i]);

	Channel *channel = s.getChannel(channelToFind);
	if (!channel) {
		sendMessage(c, ERR_NOSUCHCHANNEL(c.getNickname(), channelToFind));
		return false;
	}
	std::map<Client*, int> usersMap = channel->getUsersMap();
	for (std::map<Client*, int>::iterator it = usersMap.begin(); it != usersMap.end(); ++it) {
		Client *client = it->first;
		std::string userName = client->getUsername();
		std::string nickName = client->getNickname();
		std::string realName = client->getRealname();
		std::string userHost = client->getHostClient();
		if (it->second == 0) { // utilisateur
			std::string userType = "";
			sendMessage(c, RPL_WHOREPLY(channel->getName(), userName, userHost, nickName, userType, realName));
		} else if (it->second == 1) { // operateur
			std::string userType = "@";
			sendMessage(c, RPL_WHOREPLY(channel->getName(), userName, userHost, nickName, userType, realName));
		}
	}
	if (DEBUG_MODE)
		std::cout << RED << "endofwho c.getUsername() = " << c.getUsername() << " channel->getName() = " << channel->getName() << RESET << std::endl;

	sendMessage(c, RPL_ENDOFWHO(c.getUsername(), channel->getName()));
	return true;
}

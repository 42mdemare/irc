// La commande TOPIC permet a un operateur d'afficher ou de (re)definir le sujet d'un canal
// TOPIC <channel> [<topic>]
// <channel>: Chaine a modifier/visualiser.
// <topic>: Si present, le nouveau sujet a definir sur le canal.

#include "../../includes/CommandHandler.hpp"

static int	getLevel(Client &c, Channel* chan) {
	std::map<Client*, int>::iterator it;
	std::map<Client*, int> _users = chan->getUsersMap();
	for (it = _users.begin(); it != _users.end(); ++it) {
		if (it->first == &c)
			return it->second;
	}
	return -1;
}

bool	handleTopic(Server &server, Client &c, const std::vector<std::string> &token) {
	std::vector<std::string>::const_iterator	it;
	std::string									channel;

	if (DEBUG_MODE)
		std::cout << "handleTopic() fonction" << std::endl;

	try {
		// 1er token: la commande TOPIC
		it = token.begin() + 1;

		// 2e token: le nom du canal
		if (it == token.end()) {
			std::string message = "TOPIC";
			sendMessage(c, ERR_NEEDMOREPARAMS(message));
			throw std::invalid_argument("wrong argument");
		}
		channel = *it;
		for (size_t i = 0; i < channel.size(); ++i)
			channel[i] = tolower(channel[i]);
		Channel* chan = server.getChannel(channel);
		if (!chan) {
			sendMessage(c, ERR_NOSUCHCHANNEL(c.getNickname(), channel));
			throw std::invalid_argument("wrong argument");
		}

		// Verifier que le client appartient au canal
		if (!chan->isPartOfChannel(c.getNickname())) {
			sendMessage(c, ERR_NOTONCHANNEL(c.getNickname(), channel));
			throw std::invalid_argument("wrong argument");
		}

		// Changer de topic ou juste l'afficher?
		++it;
		if (it == token.end()) { // pas de 3e token: juste afficher le topic
			
			if (chan->getTopic().empty())
				sendMessage(c, RPL_NOTOPIC(c.getNickname(), chan->getName()));
			else
				sendMessage(c, RPL_TOPIC(c.getNickname(), chan->getName(), chan->getTopic()));
			return true;
		} else {
			std::string newTopic = *it;
			++it;
			if (newTopic[0] == ':')
				newTopic = newTopic.substr(1, newTopic.size() - 1);
			else
				newTopic = newTopic.substr(0, newTopic.size());
			if (chan->getSpecificMode("+t")) { // si mode +t
				if (getLevel(c, chan) != 1) { // si client pas operateur: ne peut pas changer le topic
					sendMessage(c, ERR_CHANOPRIVSNEEDED(chan->getName()));
					throw std::invalid_argument("wrong level");
				}
			}
			// autres cas: +t et client operateur, ou alors -t: changer le topic
			chan->setTopic(newTopic);
			std::string message = c.getNickname() + "!" + c.getUsername() + "@" + c.getHostClient() + " TOPIC " + chan->getName() + ": " + newTopic + "\r\n";
			chan->sendMessageToChannel(message);
			sendMessage(c, RPL_TOPIC(c.getNickname(), chan->getName(), chan->getTopic()));
		}
		return true;
	} catch (std::invalid_argument &e) {
		if (DEBUG_MODE)
			std::cerr << RED << e.what() << RESET << std::endl;
		return false;
	}
}

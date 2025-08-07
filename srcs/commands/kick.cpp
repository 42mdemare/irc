// La commande KICK permet a un operateur de canal de supprimer un utilisateur de son canal.
// KICK <canal> <nickname> :Ceci est la raison
// ou bien sans raison: KICK <canal> <nickname>

#include "../../includes/CommandHandler.hpp"

static Client*	getClient(std::string& name, Channel* chan) {
	std::map<Client*, int>::iterator it;
	std::map<Client*, int> _users = chan->getUsersMap();
	for (it = _users.begin(); it != _users.end(); ++it) {
		if ((it->first)->getNickname() == name)
			return it->first;
	}
	return NULL;
}

bool	handleKick(Server &server, Client &c, const std::vector<std::string> &token) {
	std::vector<std::string>::const_iterator	it;
	std::string									channel;
	std::string									user;
	std::string									reason;

	try {
		// 1er token: la commande KICK
		it = token.begin() + 1;

		// 2e token: le nom du canal
		if (it == token.end()) {
			std::string message = "kick";
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

		// Verifier que le kickeur appartient au canal
		if (!chan->isPartOfChannel(c.getNickname())) {
			sendMessage(c, ERR_NOTONCHANNEL(c.getNickname(), channel));
			throw std::invalid_argument("wrong argument");
		}

		// Verifier que le kickeur a le droit de kicker
		if (chan->getLevel(&c) != 1) {
			sendMessage(c, ERR_CHANOPRIVSNEEDED(channel));
			throw std::invalid_argument("wrong level");
		}

		// 3e token: l'utilisateur expulse
		++it;
		if (it == token.end()) {
			std::string message = "kick";
			sendMessage(c, ERR_NEEDMOREPARAMS(message));
			throw std::invalid_argument("wrong argument");
		}
			
		user = *it;
		Client *userToKick = getClient(user, chan);
		if (!userToKick) {
			sendMessage(c, ERR_USERNOTINCHANNEL(user, channel));
			throw std::invalid_argument("wrong user");
		}
		if (userToKick->getNickname() == c.getNickname()) {
			sendMessage(c, ERR_CANNOTKICKYOURSELF(c.getNickname()));
			throw std::invalid_argument("wrong user");
		}

		// 4e token potentiel: la raison
		++it;
		if (it == token.end()) {
			// Expulser sans raison
			std::string youreKickedOutMessage = "You have been kicked out from " + chan->getName() + " by " + c.getNickname() + "\r\n";
			sendMessage(*userToKick, youreKickedOutMessage);
			
			chan->leave(userToKick);
			
			std::string message = userToKick->getNickname() + " has been kicked out from " + chan->getName() + " by " + c.getNickname() + "\r\n";
			chan->sendMessageToAllTheOthers(message, userToKick);
			return (true);
		} else {
			// Reconstituer la raison et kicker
			while (it != token.end()) {
				reason += *it + " ";
				++it;
			}
			if (DEBUG_MODE)
				std::cout << reason << std::endl;
			if (reason[0] == ':')
				reason = reason.substr(1, reason.size() - 2);
			else
				reason = reason.substr(0, reason.size() - 1);
			std::string youreKickedOutMessage;
			if (reason.empty())
				youreKickedOutMessage = "You have been kicked out from " + chan->getName() + " by " + c.getNickname() + "\r\n";
			else
				youreKickedOutMessage = "You have been kicked out from " + chan->getName() + " by " + c.getNickname() + " (Reason: " + reason + ")\r\n";
			sendMessage(*userToKick, youreKickedOutMessage);

			chan->leave(userToKick);

			std::string message;
			if (reason.empty())
				message = userToKick->getNickname() + " has been kicked out from " + chan->getName() + " by " + c.getNickname() + "\r\n";
			else
				message = userToKick->getNickname() + " has been kicked out from " + chan->getName() + " by " + c.getNickname() + " (Reason: " + reason + ")\r\n";
			chan->sendMessageToAllTheOthers(message, userToKick);
		}
	} catch (std::invalid_argument &e) {
		if (DEBUG_MODE)
			std::cerr << RED << e.what() << RESET << std::endl;
	} catch (Channel::ChannelError &e) {
		if (DEBUG_MODE)
			std::cerr << RED << e.what() << RESET << std::endl;
	}
	return true;
}

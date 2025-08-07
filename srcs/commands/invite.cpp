// La commande INVITE permet d'inviter quelqu'un sur un canal. Seulement si le channel est en mode +i (invite).
// /INVITE <nickname> <channel>
// <channel>: si aucun canal est donné, le canal actif sera utilisé.

#include "../../includes/CommandHandler.hpp"

bool	handleInvite(Server &s, Client &c, const std::vector<std::string> &token) {
	if (token.size() != 3) {
		sendMessage(c, ERR_NEEDMOREPARAMS(token[0]));
		return false;
	}

	std::string channel = token[2];
	for (size_t i = 0; i < channel.size(); ++i)
		channel[i] = tolower(channel[i]);
	std::string target = token[1];
	std::string fullMsg = ":" + c.getPrefix() + " INVITE " + target + " :" + channel + "\r\n";

	if (DEBUG_MODE)
		std::cout << "commande = " << token[0] << " nick = " << token[1] << " canal = " << token[2] << std::endl;

	Channel* chan = s.getChannel(channel);
	if (!chan) {
		sendMessage(c, ERR_NOSUCHCHANNEL(c.getNickname(), channel));
		return false;
	}

	if (!chan->hasClient(&c)) {
		sendMessage(c, ERR_NOTONCHANNEL(c.getNickname(), channel));
		return false;
	}

	if (chan->getSpecificMode("+i") && chan->getLevel(&c) != 1) {
		sendMessage(c, ERR_CHANOPRIVSNEEDED(channel));
		return false;
	}

	Client* dest = s.getClientByNickname(target);
	if (!dest || !dest->getIsAuthenticated()) {
		sendMessage(c, ERR_NOSUCHNICK(c.getNickname(), target));
		return false;
	}

	if (chan->hasClient(dest)) {
		sendMessage(c, ERR_USERONCHANNEL(chan->getNicknameWithPrefix(&c), chan->getNicknameWithPrefix(dest), channel));
		return false;
	}

	if (!chan->isInvited(dest))
		chan->setInvited(dest);

	sendMessage(c, RPL_INVITING(c.getNickname(), target, chan->getName()));
	sendMessage(*dest, fullMsg);

	return true;
}

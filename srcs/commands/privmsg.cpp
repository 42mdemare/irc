// La comande PRIVMSG sert a envoyer un message prive
// PRIVMSG <target> :<message>
// PRIVMSG foo,bar[,anon] :Hi there pour les utilisateurs
// PRIVMSG #help,#home[,#irc] :Hi there pour les chaînes

#include "../../includes/CommandHandler.hpp"

bool	handlePrivmsg(Server &s, Client &c, const std::vector<std::string> &token) {
	if (token.size() < 3) {
		// On a besoin d'au moins une cible et un mot de message.
		if (token.size() < 2)
			sendMessage(c, ERR_NORECIPIENT(c.getNickname(), "PRIVMSG"));
		else
			sendMessage(c, ERR_NOTEXTTOSEND(c.getNickname()));
		return false;
	}

	// Recup les cibles separees par des virgules
	std::vector<std::string> targets;
	std::string msg;
	size_t msg_start_index = 0;

	// Parcourt les tokens pour trouver le debut du message
	for (size_t i = 1; i < token.size(); ++i) {
		if (token[i][0] == ':') {
			msg_start_index = i;
			break;
		}
		std::stringstream ss(token[i]);
		std::string tgt;
		while (std::getline(ss, tgt, ',')) {
			if (!tgt.empty()) {
				if (DEBUG_MODE)
					std::cout << "handlePrivmsg tgt = " << tgt << std::endl;
				targets.push_back(tgt);
			}
		}
	}

	// Si aucune cible n'a ete trouvee
	if (targets.empty()) {
		sendMessage(c, ERR_NORECIPIENT(c.getNickname(), "PRIVMSG"));
		return false;
	}

	// Si on n'a pas trouve de message commencant par ':', c'est une erreur
	if (msg_start_index == 0) {
		sendMessage(c, ERR_NOTEXTTOSEND(c.getNickname()));
		return false;
	}

	// Reconstruire le message
	msg = token[msg_start_index];
	msg.erase(0, 1); // Supprimer le ':' au debut du message
	for (size_t i = msg_start_index + 1; i < token.size(); ++i)
		msg += " " + token[i];

	if (msg.empty()) {
		sendMessage(c, ERR_NOTEXTTOSEND(c.getNickname()));
		return false;
	}
	if (DEBUG_MODE) {
		std::cout << "Message to send: '" << msg << "' to targets:" << std::endl;
		for(size_t i = 0; i < targets.size(); ++i) std::cout << "- " << targets[i] << std::endl;
	}

	// Envoi du message a chaque cible
	for (std::vector<std::string>::iterator it = targets.begin(); it != targets.end(); ++it) {
		std::string target = *it;
		std::string fullMsg = ":" + c.getPrefix() + " PRIVMSG " + target + " :" + msg + "\r\n";

		if (target[0] == '#') { // Si canal
			for (size_t i = 0; i < target.size(); ++i)
				target[i] = tolower(target[i]);
			Channel* chan = s.getChannel(target);
			if (!chan) {
				sendMessage(c, ERR_NOSUCHCHANNEL(c.getNickname(), target));
				continue;
			}

			if (!chan->hasClient(&c)) {
				sendMessage(c, ERR_CANNOTSENDTOCHAN(c.getNickname(), target));
				continue;
			}
		   
			chan->sendMessageToAllTheOthers(fullMsg, &c);
		} else { // Si utilisateur
			Client* dest = s.getClientByNickname(target);
			if (!dest || !dest->getIsAuthenticated()) {
				sendMessage(c, ERR_NOSUCHNICK(c.getNickname(), target));
				continue;
			}
			sendMessage(*dest, fullMsg);
			sendMessage(c, fullMsg);
		}
	}
	return true;
}

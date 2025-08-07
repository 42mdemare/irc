// La commande NICK permet a un utilisateur de definir et de modifier son pseudonyme.
// Lorsqu'elle est envoyee par un utilisateur a un serveur, il s'agit d'une demande de changement de pseudonyme (que le serveur peut accepter ou refuser).
// Lorsque le message NICK est envoye par le serveur, il indique qu'un changement a eu lieu.

// NICK <nickname>
// Si la longueur <nickname> est superieure a celle autorisee par le serveur pour les surnoms, elle est tronquee silencieusement 
// (l'utilisateur doit rechercher et definir le nouveau surnom a partir de la reponse)

#include "../../includes/CommandHandler.hpp"

bool	isForbidden(char c) {
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
		|| c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '^' || c == '{' || c == '}')
		return false;
	return true;
}

bool	forbiddenChar(const std::string &nick) {
	for (size_t i = 0; i < nick.size(); i++) {
		if (isForbidden(nick[i]))
			return true;
	}
	return false;
}

bool handleNICK(Server &s, Client &c, const std::vector<std::string>& token) {
	if (token.size() < 2 || token[1].empty()) {
		sendMessage(c, ERR_NONICKNAMEGIVEN(c.getNickname()));
		return false;
	}

    const std::string& newNick = token[1];

	if (newNick.size() > 9 || forbiddenChar(newNick)) {
		sendMessage(c, ERR_ERRONEUSNICKNAME(c.getNickname()));
		return false;
	}

    // On verifie si le nickname est deja pris par un autre client.
	for (std::vector<Client*>::const_iterator it = s.getClients().begin(); it != s.getClients().end(); ++it) {
		Client *otherClient = *it;
        // On s'assure de ne pas se comparer a soi-meme.
		if (otherClient != &c && otherClient->getNickname() == newNick) {
			sendMessage(c, ERR_NICKNAMEINUSE(newNick));
        	return false;
        }
    }

	if (c.getNickname().size() > 0) {
		std::string msg = ":" + c.getPrefix() + " NICK :" + newNick + "\r\n";
		sendMessage(c, msg);
	}
	c.setNickname(newNick);
	if (DEBUG_MODE)
		std::cout << "Client #" << c.getFd() << ": nickname set to: " << c.getNickname() << std::endl;
    
    attemptAuthentication(s, c);
	return true;
}

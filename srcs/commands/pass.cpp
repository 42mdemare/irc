// La commande PASS permet a l'utilisateur de fournir un mot de passe de connexion au serveur.
// Cette commande ne peut pas etre utilisee une fois l'enregistrement de la connexion termine.
// PASS <password>
// Il s'agit d'un mot de passe de connexion et ne concerne pas de potentiels comptes d'utilisateurs

#include "../../includes/CommandHandler.hpp"

bool handlePASS(Server &s, Client &c, const std::vector<std::string> &token) {
    // Verifier si l'utilisateur est deja enregistre
    if (c.getIsAuthenticated()) {
        sendMessage(c, ERR_ALREADYREGISTRED(c.getNickname()));
        return false;
    }

	if (token.size() < 2) {
		std::string message = "PASS";
		sendMessage(c, ERR_NEEDMOREPARAMS(message));
		return false;
	}

	if (strcmp(token[1].c_str(), s.getPassword().c_str()) == 0) {
        c.setIsPass(true);
        attemptAuthentication(s, c);
		return true;
	} else {
		sendMessage(c, ERR_PASSWDMISMATCH(c.getNickname()));
		return false;
    }
}

#include "../../includes/CommandHandler.hpp"

bool handleUser(Server &s, Client &c, const std::vector<std::string> &token) {
	(void)s;
    if (c.getIsAuthenticated()) {
        sendMessage(c, ERR_ALREADYREGISTRED(c.getNickname()));
        return false;
    }

	if (token.size() < 5) {
		std::string message = "USER";
		sendMessage(c, ERR_NEEDMOREPARAMS(message));
		return false;
	}

	c.setUsername(token[1]);

	// La specification IRC dit que le dernier parametre doit commencer par ':'
	if (token[4][0] != ':') {
		sendMessage(c, ERR_REALNAMESTART());
		return false;
	}

    // Reconstruction correcte du realname
    std::string realname = token[4].substr(1); // On prend le premier mot et on enleve le ':'
    for (size_t i = 5; i < token.size(); ++i) { // On commence la boucle a l'element SUIVANT
        realname += " " + token[i];
    }
	c.setRealname(realname);
	if (DEBUG_MODE)
		std::cout << "Client #" << c.getFd() << " USER set, realname: " << c.getRealname() << std::endl;

    attemptAuthentication(s, c);
	return true;
}

#include "../../includes/CommandHandler.hpp"

bool	handlePong(Server &s, Client &c, const std::vector<std::string> &token) {
	(void)s;
	if (token.size() < 2) {
		sendMessage(c, ERR_NOORIGIN(c.getNickname()));
		return false;
	} else {
		sendMessage(c, ":localhost PONG localhost\r\n");
		return true;
	}
}

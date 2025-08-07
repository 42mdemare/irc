// La commande MODE permet de definir les modes d'un canal ou d'un utilisateur.
// /MODE <your nickname>|<channel> [<mode> [<mode parameters>]]
// Si le pseudo/canal cible est omis, le pseudo/canal actif sera utilise.
// On peut specifier plusieurs modes dans une seule commande et les ajouter en utilisant le signe + pour definir
// ou le signe - pour desactiver

#include "../../includes/CommandHandler.hpp"

static std::string	getModesString(Channel* chan) {
	std::string modesString;
	std::map<std::string, bool> modes = chan->getModes();

	for (std::map<std::string, bool>::iterator it = modes.begin(); it != modes.end(); ++it) {
		if (it->first == "+t" && it->second == true)
			modesString += "t";
		if (it->first == "+i" && it->second == true)
			modesString += "i";
		if (it->first == "+k" && it->second == true)
			modesString += "k";
		if (it->first == "+l" && it->second == true)
			modesString += "l";
		if (it->first == "+o" && it->second == true)
			modesString += "o";
	}
	return modesString;
}

static bool	hasLetter(std::string str) {
	for (size_t i = 0; i < str.size(); ++i) {
		if (isalpha(str[i]))
			return true;
	}
	return false;
}

bool handleMode(Server &s, Client &c, const std::vector<std::string> &token) {
	if (token.size() < 2) {
		sendMessage(c, ERR_NEEDMOREPARAMS(token[0]));
		return false;
	}

	std::string channelName = token[1];
	if (s.getClientByNickname(token[1]))
		return (false);

	for (size_t i = 0; i < channelName.size(); ++i)
		channelName[i] = tolower(channelName[i]);

	Channel* chan = s.getChannel(channelName);
	if (!chan) {
		sendMessage(c, ERR_NOSUCHCHANNEL(c.getNickname(), channelName));
		return false;
	}

	if (!chan->hasClient(&c)) {
		sendMessage(c, ERR_NOTONCHANNEL(c.getNickname(), channelName));
		return false;
	}

	if (token.size() == 2) { // Commande MODE <#channel>
		std::string message = getModesString(chan);
		sendMessage(c, RPL_CHANNELMODEIS(c.getNickname(), channelName, message));
		return true;
	}

	if (chan->getLevel(&c) != 1) {
		sendMessage(c, ERR_CHANOPRIVSNEEDED(chan->getName()));
		return false;
	}

	bool adding = true;
	size_t paramIndex = 3;
	std::string modeString = token[2];
	if (DEBUG_MODE)
		std::cout << "modeString: " << modeString << std::endl;

	char cMode;
	bool display = true;
	std::string	modeToKeep = modeString;
	std::vector<std::string> paramToKeep;

	for (size_t i = 0; i < modeString.size(); ++i) {
		if (chan->getName() == "#general" && modeString[i] == 'i') // On ignore un changement de mode i pour le canal #general
			continue;
			
		cMode = modeString[i];

		if (cMode == '+') {
			adding = true;
			continue;
		} else if (cMode == '-') {
			adding = false;
			continue;
		}

		switch (cMode) {
			case 'i':
				chan->setMode("+i", adding);
				break;

			case 't':
				chan->setMode("+t", adding);
				break;

			case 'k':
				if (adding) {
					if (paramIndex >= token.size())
					{
						std::string cmd = "MODE +k";
						sendMessage(c, ERR_NEEDMOREPARAMS(cmd));
						eraseChar(modeToKeep, 'k');
						continue ;
					}
					else
					{
						std::string key = token[paramIndex++];
						if (forbiddenChar(key))
						{
							sendMessage(c, ERR_INVALIDKEY(key));
							eraseChar(modeToKeep, 'k');
							continue ;
						}
						chan->setMode("+k", adding);
						chan->setKey(key);
						paramToKeep.push_back(key);
					}
				} else {
					chan->setMode("+k", adding);
					chan->unsetKey();
				}
				break;

			case 'l': {
				if (adding) {
					if (paramIndex >= token.size()) {
						std::string cmd = "MODE +l";
						sendMessage(c, ERR_NEEDMOREPARAMS(cmd));
						eraseChar(modeToKeep, 'l');
						continue ;
					}
					else if (token[paramIndex].empty() || chkDigit(token[paramIndex]))
					{
						std::string cmd = "MODE +l";
						sendMessage(c, ERR_NEEDMOREPARAMS(cmd));
						eraseChar(modeToKeep, 'l');
						continue ;
					}
					else
					{
						int limit = std::atoi(token[paramIndex].c_str());
						chan->setMode("+l", adding);
						chan->setNbMaxUser(limit);
						paramToKeep.push_back(token[paramIndex]);
						paramIndex++;
					}
				} else {
					chan->setMode("+l", adding);
					chan->setNbMaxUser(-1);
				}
				break;
			}
			case 'o': {
				if (paramIndex >= token.size()) {
					std::string cmd = "MODE +o";
					sendMessage(c, ERR_NEEDMOREPARAMS(cmd));
					eraseChar(modeToKeep, 'o');
					continue ;
				} else {
					std::string param = token[paramIndex++];
					Client* client = chan->getClientByNickname(param);
					if( !client) {
						sendMessage(c, ERR_USERNOTINCHANNEL(param, chan->getName()));
						eraseChar(modeToKeep, 'o');
						display = false;
					} else {
						chan->setMode("+o", adding);
						chan->setLevel(client, adding);
						std::string message = NEWOPER(chan->getName(), chan->getNicknameWithPrefix(client));
						chan->sendMessageToChannel(message);
						sendMessage(*client, RPL_NAMREPLY(chan->getNicknameWithPrefix(client), chan->getType(), chan->getName(), chan->userListCreate()));
						sendMessage(*client, RPL_ENDOFNAMES(chan->getNicknameWithPrefix(client), chan->getName()));
						paramToKeep.push_back(param);
					}
				}
				break;
			}
			case 'b':
				break;
			default:
				sendMessage(c, ERR_UNKNOWNMODE(chan->getNicknameWithPrefix(&c), cMode, chan->getName()));
				break;
		}
	}

	if (chan->getName() == "#general") { // Retirer le i en cas de #general
		eraseChar(modeToKeep, 'i');
	}

	eraseChar(modeToKeep, 'b');

	if (modeToKeep.size() < 2)
		return (false);

	std::string fullModeLine = ":" + c.getPrefix() + " MODE " + channelName + " " + modeToKeep;
	for (size_t i = 0; i < paramToKeep.size(); ++i)
		fullModeLine += " " + paramToKeep[i];
	fullModeLine += "\r\n";

	if (hasLetter(modeString) && display == true) // Verifie que c'est pas juste des +- (comme on a retire les i dans le cas de #general)
		chan->sendMessageToChannel(fullModeLine);

	return true;
}

void	eraseChar(std::string &mode, char toErase)
{
	std::string::size_type pos = mode.find(toErase);
		if (pos != std::string::npos)
			mode.erase(pos, 1);
}
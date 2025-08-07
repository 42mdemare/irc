// La commande PART indique que le client souhaite quitter un ou plusieurs canaux .
// En reponse, le serveur envoie un ou plusieurs messages PART pour indiquer que la requete a abouti.
// PART <channel>{,<channel>} [<reason>]
// <channel> : nom du ou des canaux a rejoindre, separes par des virgules.
// <reason>: Raison facultative qui est affichee aux autres utilisateurs du canal.

#include "../../includes/CommandHandler.hpp"

void	leaveAllChannels(Client& c, Server& server) {
	std::map<std::string, Channel*> channels = server.getChannels();
	std::map<std::string, Channel*>::iterator it_channels;
	
	// Parcourir tous les canaux existant
	for (it_channels = channels.begin(); it_channels != channels.end(); ++it_channels) {
		Channel* chan_tmp = it_channels->second;
		std::map<Client*, int> users = chan_tmp->getUsersMap();
		std::map<Client*, int>::iterator it_users;
		
		// Parcourir tous les clients du channel
		for (it_users = users.begin(); it_users != users.end(); ++it_users) {
			Client client_tmp = *it_users->first;
			
			// Si le client de la commande est trouve, alors quitte le canal
			if (client_tmp.getUsername() == c.getUsername()) {
				try {
					std::string message = ":" + c.getNickname() + "!" + c.getUsername() + "@" + c.getHostClient() + " PART " + chan_tmp->getName() + "\r\n";
					sendMessage(c, message);
					chan_tmp->leave(&c);
				} catch (Channel::ChannelError &e) {
					std::cerr << RED << e.what() << RESET << std::endl;
				}
			}
		}
	}
}

static char		toLowerChar(char c) { return static_cast<char>(std::tolower(static_cast<unsigned char>(c))); }

bool handlePart(Server &s, Client &c, const std::vector<std::string> &token) {
	if (token.size() < 2) {
		sendMessage(c, ERR_NEEDMOREPARAMS(token[0]));
		return false;
	}

	for (size_t i = 1; i < token.size(); ++i) {
		std::string param = token[i];
		std::stringstream ss(param);
		std::string channel_name;

		// extraire les noms de canaux
		while (std::getline(ss, channel_name, ',')) {
			std::transform(channel_name.begin(), channel_name.end(), channel_name.begin(), toLowerChar);
			if (channel_name.empty())
				continue;

			if (channel_name[0] == ':')
				channel_name = channel_name.substr(1);

			Channel* chan = s.getChannel(channel_name);
			if (!chan) {
				sendMessage(c, ERR_NOSUCHCHANNEL(c.getNickname(), channel_name));
				continue;
			}

			if (!chan->hasClient(&c)) {
				sendMessage(c, ERR_NOTONCHANNEL(c.getNickname(), channel_name));
				continue;
			}
			chan->leave(&c);
		}
	}
	return true;
}
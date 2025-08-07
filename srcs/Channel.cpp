#include "../includes/Channel.hpp"

#pragma region "FORME_CANONIQUE"

Channel::Channel() {
	if (DEBUG_MODE)
		std::cout << GREEN << "Default constructor Channel called" << RESET << std::endl;
	_modes["+t"] = false; // topic non protege car non defini
	_modes["+i"] = false;
	_modes["+k"] = false;
	_modes["+l"] = false;
	_modes["+o"] = false;
	_type = "=";
	_name = "";
	_topic = "";
	_nbMaxUser = -1;
	_key = "";
}

Channel::Channel(std::string name): _name(name) {
	if (DEBUG_MODE)
		std::cout << GREEN << "Default constructor Channel called" << RESET << std::endl;
	_modes["+t"] = false; // topic non protege car non defini
	_modes["+i"] = false;
	_modes["+k"] = false;
	_modes["+l"] = false;
	_modes["+o"] = false;
	_type = "=";
	_topic = "";
	_nbMaxUser = -1;
	_key = "";
}

Channel::Channel(std::string name, std::string topic): _name(name), _topic(topic) {
	if (DEBUG_MODE)
		std::cout << GREEN << "Constructor Channel with name <" << _name << "> and topic <" << _topic << "> called" << RESET << std::endl;
	_modes["+t"] = true; // topic protege car defini
	_modes["+i"] = false;
	_modes["+k"] = false;
	_modes["+l"] = false;
	_modes["+o"] = false;
	_type = "=";
	_nbMaxUser = -1;
}

Channel::~Channel() {
	if (DEBUG_MODE)
		std::cout << RED << "Default destructor Channel called" << RESET << std::endl;
}

Channel::Channel(const Channel &copy) {
	if (DEBUG_MODE)
		std::cout << BLUE << "Channel Copy Constructor called" << RESET << std::endl;
	_name = copy._name;
	_topic = copy._topic;
	_author = copy._author;
	_creationTime = copy._creationTime;
	_invited = copy._invited;
	_nbMaxUser = copy._nbMaxUser;
	_usersMap = copy._usersMap;
	_modes = copy._modes;
	_type = copy._type;
	_nbMaxUser = copy._nbMaxUser;
	_key = copy._key;
}

Channel &Channel::operator=(const Channel &src) {
	if (DEBUG_MODE)
		std::cout << BLUE << "Channel Assigment operator called" << RESET << std::endl;
	if (this != &src) {
		_name = src._name;
		_topic = src._topic;
		_author = src._author;
		_creationTime = src._creationTime;
		_invited = src._invited;
		_nbMaxUser = src._nbMaxUser;
		_usersMap = src._usersMap;
		_modes = src._modes;
		_type = src._type;
		_nbMaxUser = src._nbMaxUser;
		_key = src._key;
	}
	return *this;
}

#pragma endregion

#pragma region "GUETTERS"

std::string						Channel::getName() { return _name; }
std::string						Channel::getTopic() { return _topic; }
std::map<std::string, bool>		Channel::getModes() { return _modes; }
std::map<Client*, int>			Channel::getUsersMap() { return _usersMap; }
std::string						Channel::getType() { return _type; }
std::string						Channel::getCreationTime(){ return _creationTime; }
std::string						Channel::getAuthor(){ return _author; }
int								Channel::getLevel(Client* c) {
	for (std::map<Client*, int>::iterator it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first == c)
			return it->second;
	}
	return -1;
}
int								Channel::getSpecificMode(std::string mode) {
	std::map<std::string, bool>::iterator it;
	for (it = _modes.begin(); it != _modes.end(); ++it) {
		if (it->first == mode)
			return it->second;
	}
	return -1;
}
std::vector<Client*>			Channel::getInvited() { return _invited; }
ssize_t							Channel::getNbMaxUser() { return _nbMaxUser; }
std::string						Channel::getKey(){ return _key; }
Client*                         Channel::getClientByNickname(const std::string& nickname)
{
	for (std::map<Client*, int>::iterator it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first->getNickname() == nickname) {
			return it->first;
		}
	}
	return NULL;
}
std::string						Channel::getNicknameWithPrefix(Client* c) {
	if (!c)
		throw ChannelError();
	for (std::map<Client*, int>::iterator it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first == c) {
			if (it->second == 1)
				return "@" + c->getNickname();
		}
	}
	return c->getNickname();
}

#pragma endregion

#pragma region "SETTERS"

void						Channel::setName(std::string newName) { _name = newName; }
void						Channel::setTopic(std::string newTopic) { _topic = newTopic; }
void						Channel::setMode(std::string mode, bool state) { _modes[mode] = state; }
void						Channel::setType(std::string newType) { _type = newType; }
void						Channel::setCreationTime(std::string newCreationTime) { _creationTime = newCreationTime; }
void						Channel::setAuthor(std::string newAuthor) { _author = newAuthor; }
void						Channel::setLevel(Client* c, int level) {
	for (std::map<Client*, int>::iterator it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first == c) {
			it->second = level;
			return ;
		}
	}
}
void						Channel::setNbMaxUser(ssize_t n) { _nbMaxUser = n; }
void						Channel::setInvited(Client* c){ _invited.push_back(c); }
void						Channel::unsetInvited(Client *c) {
	std::vector<Client*>::iterator it;
	if (c) {
		for (it = _invited.begin(); it != _invited.end(); ++it) {
			if ((*it)->getNickname() == c->getNickname()) {
				_invited.erase(it);
				break;
			}
		}
	}
}
void						Channel::setKey(std::string newKey) { _key = newKey; }
void						Channel::unsetKey() { _key = ""; }

#pragma endregion

#pragma region "METHODES"

bool	Channel::isInvited(Client* c) {
	std::vector<Client*>::iterator it;
	for (it = _invited.begin(); it != _invited.end(); ++it) {
		if ((*it)->getNickname() == c->getNickname()) {
			return true;
		}
	}
	return false;
}

void	Channel::join(Client* client) {
	if (!client)
		throw ChannelError();
	_usersMap.insert(std::make_pair(client, 0));

	if (this->_usersMap.size() == 1) {
		this->setLevel(client, 1); // le 1er membre d'un canal est designe operateur
		std::string message = NEWOPER(_name, getNicknameWithPrefix(client));
		this->sendMessageToChannel(message);
	}

	std::cout << getNicknameWithPrefix(client) << " joined " << _name << std::endl;

	std::string message = ":" + client->getPrefix() + " JOIN :" + _name + "\r\n";
	sendMessageToChannel(message);

	if (!_topic.empty())
		sendMessage(*client, RPL_TOPIC(getNicknameWithPrefix(client), _name, _topic));
	sendMessage(*client, RPL_NAMREPLY(getNicknameWithPrefix(client), _type, _name, userListCreate()));
	sendMessage(*client, RPL_ENDOFNAMES(getNicknameWithPrefix(client), _name));

	// Si le client etait invite a ce canal, retirer l'invitation une fois que le client a rejoint le canal
	if (this->isInvited(client))
		this->unsetInvited(client);
}

void	Channel::leave(Client* client) {
	if (!client)
		throw ChannelError();

	std::string message = ":" + getNicknameWithPrefix(client) + "!" + client->getUsername() + "@" + client->getHostClient() + " PART " + _name + "\r\n";
	sendMessageToAllTheOthers(message, client);

	if (_usersMap.find(client) != _usersMap.end()) {
		_usersMap.erase(client);
		std::cout << getNicknameWithPrefix(client) << " left " << _name << std::endl;
	}
	checkOperatorPresence(); // Verifier qu'il y a bien un operateur dans le canal, et si non, en definir un
}

void	Channel::checkOperatorPresence() {
	std::map<Client*, int>::iterator it;
	bool	op = false;
	for (it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->second == 1) {
			op = true; // il y a bien un operateur
			break;
		}
	}
	if (op == false) { // s'il n'y a pas d'operateur
		it = _usersMap.begin();
		if (it != _usersMap.end()) {
			it->second = 1; // definir le 1er de la map (donc le plus ancien ayant rejoint le canal) comme operateur
			if (it->first) {
				std::string message = NEWOPER(_name, getNicknameWithPrefix(it->first));
				this->sendMessageToChannel(message);
			}
		}
	}
}

void	Channel::sendMessageToChannel(std::string& message) {
	for (std::map<Client*, int>::iterator it = _usersMap.begin(); it != _usersMap.end(); it++) {
		Client *client = it->first;
		if (client)
			sendMessage(*client, message);
	}
}

std::string	Channel::userListCreate() {
	std::string user_list;
	std::map<Client*, int>::iterator it;
	for (it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first) {
			user_list += getNicknameWithPrefix(it->first) + " ";
		}
	}
	user_list = user_list.substr(0, user_list.size() - 1);
	return user_list;
}

bool	Channel::isPartOfChannel(std::string name) {
	std::map<Client*, int>::iterator it;
	for (it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first) {
			if ((it->first)->getNickname() == name || getNicknameWithPrefix(it->first) == name)
				return true;
		}
	}
	return false;
}

bool Channel::hasClient(Client* client) const {
	if (!client)
		return false;
	std::map<Client*, int>::const_iterator it;
	for (it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first == client)
			return true;
	}
	return false;
}

void Channel::sendMessageToAllTheOthers(const std::string& message, Client* notToThisUser) {
	std::map<Client*, int>::iterator it;
	for (it = _usersMap.begin(); it != _usersMap.end(); ++it) {
		if (it->first && it->first != notToThisUser) {
			sendMessage(*it->first, message);
		}
	}
}

#pragma endregion

#pragma region "EXCEPTIONS"

const char*	Channel::ChannelError::what() const throw () { return "Client couldn't join"; }

#pragma endregion

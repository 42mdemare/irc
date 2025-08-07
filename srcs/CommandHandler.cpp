#include "../includes/CommandHandler.hpp"

void sendMessage(Client &c, std::string message) { send(c.getFd(), message.c_str(), message.size(), 0); }

std::vector<std::string>	splitCmd(std::string &buffer) {
	std::vector<std::string>	cmd;
	std::string					line;
    std::stringstream 			buffer_stream(buffer);

	int	i = 0;
    while (std::getline(buffer_stream, line, '\n')) {
        if (line.empty())
            continue;
		if (*(line.rbegin()) == '\r')
			cmd.push_back(line.substr(0, line.size() - 1));
		else
			cmd.push_back(line.substr(0, line.size()));
		if (DEBUG_MODE)
	        std::cout << "[cmd]: '" << cmd[i] << "'" << std::endl;
		i++;
    }
	return cmd;
}

std::vector<std::string>	tokenizer(std::string cmd) {
	std::vector<std::string>	token;
	std::string					line;
    std::stringstream 			buffer_stream(cmd);

	int	i = 0;
    while (std::getline(buffer_stream, line, ' ')) {
        if (line.empty())
            continue;
		token.push_back(line);
		if (line.find(':') != std::string::npos)
			break ;
		if (DEBUG_MODE)
	        std::cout << "[token]: '" << token[i] << "'" << std::endl;
		i++;
    }
	if (std::getline(buffer_stream, line)) {
		token[i] += " " + line;
		if (DEBUG_MODE)
	        std::cout << "[token]: '" << token[i] << "'" << std::endl;
	}
	return token;
}

void attemptAuthentication(Server& s, Client& c) {
    if (c.getIsAuthenticated())
        return;

    if (c.getIsPass() && !c.getNickname().empty() && !c.getRealname().empty()) {
        c.setIsAuthenticated(true);

		std::time_t now = std::time(0);
		std::tm* localtime = std::localtime(&now);

		std::ostringstream oss;
		oss << std::setfill('0')
			<< std::setw(2) << localtime->tm_mday << "/"
			<< std::setw(2) << (localtime->tm_mon + 1) << "/"
			<< (localtime->tm_year + 1900) << " "
			<< std::setw(2) << localtime->tm_hour << ":"
			<< std::setw(2) << localtime->tm_min << ":"
			<< std::setw(2) << localtime->tm_sec;

			sendMessage(c, RPL_WELCOME(c.getNickname(), c.getPrefix()));
			sendMessage(c, RPL_YOURHOST(c.getNickname()));
			sendMessage(c, RPL_CREATED(c.getNickname(), oss.str()));
			sendMessage(c, RPL_MYINFO(c.getNickname()));
			sendMessage(c, asciiArt() + "\r\n");
			sendMessage(c, RPL_MOTDSTART(c.getNickname()));
			sendMessage(c, RPL_MOTD(c.getNickname()));
			sendMessage(c, RPL_ENDOFMOTD(c.getNickname()));

			joinChannel("#general", c, s);
	}
}

int chkDigit(std::string input) {
	for (size_t i = 0; i < input.length(); i++) {
		if (!isdigit(input[i]))
			return 1;	
	}
	return 0;
}
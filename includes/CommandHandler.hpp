#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include "Client.hpp"
#include "Server.hpp"
#include "config.h"

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE  	"\033[0;34m"

#pragma region "RPL"

#define RPL_WELCOME(nickname, prefix)						":localhost 001 " + (nickname) + " :Welcome to the IRC Network " + (prefix) + "\r\n"
#define RPL_YOURHOST(nickname)								":localhost 002 " + (nickname) + " :Your host is ft_irc\r\n"
#define RPL_CREATED(nickname, Date)							":localhost 003 " + (nickname) + " :This server was created " + (Date) + "\r\n"
#define RPL_MYINFO(nickname)								":localhost 004 " + (nickname) + " :server info\r\n"

#define RPL_ENDOFWHO(nickname, channel)						":localhost 315 " + (nickname) + " " + (channel) + " :End of /WHO list.\r\n"

#define RPL_CHANNELMODEIS(nickname, channel, modes)			":localhost 324 " + (nickname) + " " + (channel) + " +" + (modes) + "\r\n"

#define RPL_NOTOPIC(nickname, channel)						":localhost 331 " + (nickname) + " " + (channel) + " :No topic is set for " + (channel) + "\r\n"
#define RPL_TOPIC(nickname, channel, topic)					":localhost 332 " + (nickname) + " " + (channel) + " :" + (topic) + "\r\n"

#define RPL_INVITING(sender, nickname, channel)				":localhost 341 " + (sender) + " " + (nickname) + " " + (channel) + "\r\n"

#define RPL_WHOREPLY(channel, user, host, nickname, userType, realname)	\
":localhost 352 " + (channel) + " " + (user) + " " + (host) + " localhost " + \
(nickname) + " H" + (userType) + " :0 " + (realname) + "\r\n"

#define RPL_NAMREPLY(nickname, type, channel, user_list)	":localhost 353 " + (nickname) + " " + (type) + " " + (channel) + " :" + (user_list) + "\r\n"

#define RPL_ENDOFNAMES(nickname, channel)					":localhost 366 " + (nickname) + " " + (channel) + " :End of NAMES list\r\n"

#define RPL_MOTDSTART(nickname)								":localhost 375 " + (nickname) + " :- Message of the Day -\r\n"
#define RPL_MOTD(nickname)									":localhost 375 " + (nickname) + " :- Bienvenue sur ce serveur ft-IRC!\r\n"
#define RPL_ENDOFMOTD(nickname)								":localhost 375 " + (nickname) + " :End of MOTD.\r\n"

#define NEWOPER(channel, nickname)							":localhost " + (channel) + " :" + (nickname) + " has been set as a new operator.\r\n"

#pragma endregion

#pragma region "ERR"

#define ERR_NOSUCHNICK(nickname, target)					":localhost 401 " + (nickname) + " " + (target) + " :No such nick/channel\r\n"
#define ERR_NOSUCHCHANNEL(nickname, channel)				":localhost 403 " + (nickname) + " " + (channel) + " :No such channel\r\n"
#define ERR_CANNOTSENDTOCHAN(nickname, channel)				":localhost 404 " + (nickname) + " " + (channel) + " :Cannot send to channel\r\n"
#define ERR_NOORIGIN(nickname) 								":localhost 409 " + (nickname) + " :No origin specified\r\n"

#define ERR_NORECIPIENT(nickname, command)					":localhost 411 " + (nickname) + " :No recipient given (" + (command) + ")\r\n"
#define ERR_NOTEXTTOSEND(nickname)							":localhost 412 " + (nickname) + " :No text to send\r\n"

#define ERR_NONICKNAMEGIVEN(nickname)						":localhost 431 :No nickname given\r\n"
#define ERR_ERRONEUSNICKNAME(nickname)						":localhost 432 " + (nickname) + " :Erroneous nickname\r\n"
#define ERR_NICKNAMEINUSE(nickname)							":localhost 433 * " + (nickname) + " :Nickname is already in use\r\n"

#define ERR_USERNOTINCHANNEL(nickname, channel)				":localhost 441 " + (nickname) + " " + (channel) + " :They aren't on that channel\r\n"
#define ERR_NOTONCHANNEL(nickname, channel)					":localhost 442 " + (nickname) + " " + (channel) + " :You're not on that channel\r\n"
#define ERR_USERONCHANNEL(sender, nickname, channel)	":localhost 443 " + (sender) + " " + (nickname) + " " + (channel) + " :is already on channel\r\n"

#define ERR_NEEDMOREPARAMS(cmd) 							":localhost 461 " + (cmd) + " :Not enough parameters\r\n"
#define ERR_ALREADYREGISTRED(nickname)						":localhost 462 " + (nickname) + " :Unauthorized command (already registered)\r\n"
#define ERR_PASSWDMISMATCH(nickname)						":localhost 464 " + (nickname) + " :Password incorrect\r\n"

#define ERR_CHANNELISFULL(nickname, channel)				":localhost 471 " + (nickname) + " " + (channel) + " :Cannot join channel (+l)\r\n"
#define ERR_UNKNOWNMODE(nickname, char, channel)			":localhost 472 " + (nickname) + " " + (char) + " :is unknown mode char to me for " + (channel) + "\r\n"

#define ERR_INVITEONLYCHAN(nickname, channel)				":localhost 473 " + (nickname) + " " + (channel) + " :Cannot join channel (+i) - must be invited \r\n"
#define ERR_BADCHANNELKEY(nickname, channel)				":localhost 475 " + (nickname) + " " + (channel) + " :Cannot join channel (+k)\r\n"

#define ERR_CHANOPRIVSNEEDED(channel)						":localhost 482 " + (channel) + " :You're not channel operator\r\n"

#define ERR_TOOMANYPARAMS()									":localhost :Too many parameters\r\n"
#define ERR_CANNOTKICKYOURSELF(nickname)					":localhost " + (nickname) + " :You cannot kick yourself from the channel\r\n"
#define ERR_REALNAMESTART()									":localhost :USER realname parameter must start with ':'\r\n"
#define ERR_INVALIDKEY(key)									":localhost " + (key) + " :Invalid key\r\n"

#pragma endregion

class Server;

void					 	sendMessage(Client &c, std::string message);
bool						handleMode(Server &s, Client &c, const std::vector<std::string> &token);
bool						handlePASS(Server &s, Client &c, const std::vector<std::string> &token);
bool						handleNICK(Server &s, Client &c, const std::vector<std::string>& token);
bool						handleUser(Server &s, Client &c, const std::vector<std::string> &token);
bool						handlePong(Server &s, Client &c, const std::vector<std::string> &token);
bool						handlePrivmsg(Server &s, Client &c, const std::vector<std::string> &token);
bool						handleJoin(Server &s, Client &c, const std::vector<std::string> &token);
bool						handleKick(Server &s, Client &c, const std::vector<std::string> &token);
bool						handleInvite(Server &s, Client &c, const std::vector<std::string> &token);
bool						handlePart(Server &s, Client &c, const std::vector<std::string> &token);
bool						handleTopic(Server &s, Client &c, const std::vector<std::string> &token);
bool						handleWho(Server &s, Client &c, const std::vector<std::string> &token);
void						attemptAuthentication(Server& s, Client& c);
std::vector<std::string>	splitCmd(std::string &buffer);
std::vector<std::string>	tokenizer(std::string cmd);
void						joinChannel(const std::string& constChannelToJoin, Client& client, Server& server);
void						leaveAllChannels(Client& c, Server& server);
bool						isForbidden(char c);
bool						forbiddenChar(const std::string &nick);
std::string					asciiArt(void);
int 						chkDigit(std::string input);
void						eraseChar(std::string &mode, char toErase);
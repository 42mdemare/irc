/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 14:12:31 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/11 10:02:14 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client()
{
	std::cout << GREEN << "Default constructor Client called" << RESET << std::endl;	
}

Client::~Client()
{
	std::cout << RED << "Default destructor Client called" << RESET << std::endl;
}

Client::Client(const Client &copy)
{
	std::cout << GREEN << "Client Copy Constructor called" << RESET << std::endl;
	*this = copy;
}

Client &Client::operator=(const Client &src)
{
	std::cout << BLUE << "Client Assigment operator called" << RESET << std::endl;
	if (this != &src)
	{
		this->_nickname = src.getNickname();
		this->_username = src.getUsername();
		this->_isAuthenticated = src.getIsAuthentificated();
		this->_operatorLvl = src.getOperatorLvl();
	}
	return (*this);
}

std::string Client::getNickname() const
{
	return (this->_nickname);
}

std::string Client::getUsername() const
{
	return (this->_username);
}

bool Client::getIsAuthentificated() const
{
	return (this->_isAuthenticated);
}

int Client::getOperatorLvl() const
{
	return (this->_operatorLvl);
}

void Client::setNickname(std::string nickname)
{
	if (nickname.c_str())
		this->_nickname = nickname;
}

void Client::setUsername(std::string username)
{
	if (username.c_str())
		this->_username = username;
}

void Client::setIsAuthentificated(bool isAuthentificated)
{
	this->_isAuthenticated = isAuthentificated;
}

void Client::setOperatorLvl(int operatorLvl)
{
	if (operatorLvl >= 0 && operatorLvl <= 2)
    {
        this->_operatorLvl = operatorLvl;
    }
}

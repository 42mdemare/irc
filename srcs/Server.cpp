/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:44:47 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/11 09:44:11 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

int chkDigit (std::string input)
{
	for (size_t i = 0; i < input.length(); i++)
	{
		if (!isdigit(input[i]))
			return (1);	
	}
	return (0);
}

Server::Server(std::string port, std::string password)
{
	std::cout << GREEN << "Default constructor Server called" << RESET << std::endl;
	std::cout << "PORT = " << port << std::endl;
	std::cout << "PASWORD = " << password << std::endl;
	std::cout << "g_shutdowm = " << g_shutdowm << std::endl;

	//Check if password is digit
	if (chkDigit(port) == 1)
	{
		g_shutdowm = 0;
		std::cerr << RED << "Error: port not a valid number => \"" << port << "\"" << RESET << std::endl;	
		std::cout << "g_shutdowm = " << g_shutdowm << std::endl;
	}
	
}

Server::~Server()
{
	std::cout << RED << "Default destructor Server called" << RESET << std::endl;
}

Server::Server(const Server &copy)
{
	std::cout << GREEN << "Server Copy Constructor called" << RESET << std::endl;
	*this = copy;
}

Server &Server::operator=(const Server &src)
{
	std::cout << BLUE << "Server Assigment operator called" << RESET << std::endl;
	if (this != &src)
	{
		this->_port = src._port;
		this->_password = src._password;
	}
	return (*this);
}

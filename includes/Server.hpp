/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 09:38:09 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/10 11:14:36 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <cstdlib>

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE  "\033[0;34m"

extern int g_shutdowm;

class Server
{
private:
	unsigned int _port;
	std::string _password;
	Server();

public:
	Server(std::string port, std::string password);
	~Server();
	Server(const Server &copy);

	Server &operator=(const Server &src);
	
};

#endif
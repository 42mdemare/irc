/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 12:38:01 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/10 11:19:05 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "includes/Server.hpp"

int g_shutdowm = 1;

int main(int argc, char **argv)
{
	std::string port;
	std::string password;
	std::string input;

	//Check numbers of arg
	if (argc != 3)
	{
		std::cerr << "Usage : ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	else
	{
		port = argv[1];
		password = argv[2];

		//try start server
		Server server(port, password);
		
		//Main loop
		while (g_shutdowm == 1)
		{
			//Get input
			std::getline(std::cin, input);
			if (input.length() > 1)
				std::cout << "input = " << input << std::endl;
			if (input == "EXIT")
			{
				//TODO : shutdown correctly the server
				g_shutdowm = 0;
			}
		}
		return (0);
	}
}

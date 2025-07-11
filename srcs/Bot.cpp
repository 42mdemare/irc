/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:43:02 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/11 09:46:31 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"

Bot::Bot()
{
	std::cout << GREEN << "Default constructor Bot called" << RESET << std::endl;	
}

Bot::~Bot()
{
	std::cout << RED << "Default destructor Bot called" << RESET << std::endl;
}

Bot::Bot(const Bot &copy)
{
	std::cout << GREEN << "Bot Copy Constructor called" << RESET << std::endl;
	*this = copy;
}

Bot &Bot::operator=(const Bot &src)
{
	std::cout << BLUE << "Bot Assigment operator called" << RESET << std::endl;
	if (this != &src)
	{
		
	}
	return (*this);
}

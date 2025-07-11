/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:43:05 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/11 09:46:55 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/CommandHandler.hpp"

CommandHandler::CommandHandler()
{
	std::cout << GREEN << "Default constructor CommandHandler called" << RESET << std::endl;	
}

CommandHandler::~CommandHandler()
{
	std::cout << RED << "Default destructor CommandHandler called" << RESET << std::endl;
}

CommandHandler::CommandHandler(const CommandHandler &copy)
{
	std::cout << GREEN << "CommandHandler Copy Constructor called" << RESET << std::endl;
	*this = copy;
}

CommandHandler &CommandHandler::operator=(const CommandHandler &src)
{
	std::cout << BLUE << "CommandHandler Assigment operator called" << RESET << std::endl;
	if (this != &src)
	{
		
	}
	return (*this);
}

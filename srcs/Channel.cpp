/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:43:00 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/10 15:44:36 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

Channel::Channel()
{
	std::cout << GREEN << "Default constructor Channel called" << RESET << std::endl;	
}

Channel::~Channel()
{
	std::cout << RED << "Default destructor Channel called" << RESET << std::endl;
}

Channel::Channel(const Channel &copy)
{
	std::cout << GREEN << "Channel Copy Constructor called" << RESET << std::endl;
	*this = copy;
}

Channel &Channel::operator=(const Channel &src)
{
	std::cout << BLUE << "Channel Assigment operator called" << RESET << std::endl;
	if (this != &src)
		*this = src;
	return (*this);
}

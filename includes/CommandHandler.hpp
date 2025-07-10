/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:39:23 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/10 15:44:10 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDHANDLER_HPP
# define COMMANDHANDLER_HPP

#include <iostream>

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE  "\033[0;34m"

class CommandHandler 
{
private:

public:
	CommandHandler();
	~CommandHandler();
	CommandHandler(const CommandHandler &copy);

	CommandHandler &operator=(const CommandHandler &src);
};

#endif
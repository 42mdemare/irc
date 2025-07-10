/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 15:38:22 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/10 15:44:16 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <iostream>

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE  "\033[0;34m"

class Channel 
{
private:

public:
	Channel();
	~Channel();
	Channel(const Channel &copy);

	Channel &operator=(const Channel &src);
};

#endif
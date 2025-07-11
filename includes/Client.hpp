/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdemare <mdemare@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 13:14:08 by mdemare           #+#    #+#             */
/*   Updated: 2025/07/11 10:02:20 by mdemare          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>

#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE  "\033[0;34m"

class Client 
{
private:
	std::string _nickname;
	std::string _username;
	bool _isAuthenticated;
	int _operatorLvl; //0 : user, 1 : moderator, 2 : admin ...

public:
	Client();
	~Client();
	Client(const Client &copy);

	Client &operator=(const Client &src);

	std::string getNickname() const;
	std::string getUsername() const;

	bool getIsAuthentificated() const;
	int getOperatorLvl() const;

	void setNickname(std::string nickname);
	void setUsername(std::string username);
	void setIsAuthentificated(bool isAuthentificated);
	void setOperatorLvl(int operatorLvl);
	
};

#endif
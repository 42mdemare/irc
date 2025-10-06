[![ft_irc](https://github.com/ayogun/42-project-badges/blob/main/badges/ft_irce.png)](https://github.com/ayogun/42-project-badges/blob/main/badges/ft_irce.png)

# 💬 42 ft_irc – 115 % ✅

**ft_irc** est un **projet de serveur de discussion multi-clients** inspiré du protocole Internet Relay Chat (**IRC**).  
L’objectif est de créer un **serveur et un client de discussion** entièrement fonctionnels utilisant des **sockets**, permettant à plusieurs utilisateurs de communiquer dans des **canaux en temps réel**, tout en explorant la **programmation réseau**, la **concurrence** et l’**architecture client-serveur**.  

---

### 🧠 Nouvelles compétences acquises  

- **Programmation réseau** → Utiliser des sockets TCP pour la communication entre le serveur et les clients.  
- **Concurrence et multiplexage** → Gérer plusieurs clients simultanément grâce à `poll`.  
- **Analyse de commandes** → Interpréter les commandes IRC (`NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, `QUIT`).  
- **Canaux et messagerie** → Créer des canaux, diffuser des messages et gérer les messages privés.  
- **Gestion des erreurs et sécurité** → Valider les commandes, éviter les crashs et gérer les connexions clients.  
- **Gestion de la mémoire** → Allocation dynamique pour les structures clients et les messages.  

---

## 🏗️ Exigences du projet  

### 🎯 Objectifs du serveur  
- Accepter plusieurs **connexions clients** simultanées.  
- Prendre en charge les **commandes IRC** suivantes :  
  - `NICK` → Définit le pseudonyme de l’utilisateur  
  - `USER` → Enregistre les informations de l’utilisateur  
  - `JOIN` → Rejoint un canal  
  - `PART` → Quitte un canal  
  - `PRIVMSG` → Envoie un message privé ou un message sur un canal  
  - `QUIT` → Se déconnecte du serveur  
- Maintenir des structures pour les **clients et les canaux**.  
- Diffuser les messages à tous les clients **du même canal**.  
- Gérer les **déconnexions** des clients proprement.  

---

### 🎮 Objectifs du client  
- Se connecter au serveur en utilisant un **nom d’hôte** et un **port**.  
- Envoyer des commandes IRC et recevoir des **messages en temps réel**.  
- Afficher les **messages de canaux**, **messages privés** et **notifications du serveur**.  

---

## ⚙️ Installation  

### Cloner le dépôt et compiler le projet :  
```bash
git clone git@github.com:axelhoizai/42-IRC.git && cd 42-IRC
make
```

---

### Démarrer le serveur :  
```bash
./ircserv 6667
```

---

### Se connecter avec un client (dans un nouveau terminal) :  
```bash
nc 127.0.0.1 6667
```

---

## 👥 Contributeurs  

- [Louise Collonge](https://github.com/louisecollonge)  
- [Mickael Demare](https://github.com/KaliStudio)  
- [Axel Hoizai](https://github.com/axelhoizai)


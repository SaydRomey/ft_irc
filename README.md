# [ft_irc](https://github.com/SaydRomey/42_ressources/blob/24d50367a9a7a41c9509a5f4731418098813f4f4/pdf/ft_irc_eng.pdf)
**Internet Relay Chat - 42**  
**Version:** 7.1  

## Summary
This project involves creating your own IRC server.  
You will use an actual IRC client to connect to your server and test it.  
Understanding solid standard protocols like those used in the Internet is a valuable skill for connected systems and networking.

---

## Introduction
**Internet Relay Chat** or **IRC** is a text-based communication protocol on the Internet.  
It offers real-time messaging that can be either public or private.  
Users can exchange direct messages and join group channels.  
IRC clients connect to IRC servers in order to join channels.  
IRC servers are connected together to form a network.

---

## General Rules
- Your program **must not crash** under any circumstances (even when memory is exhausted).  
Crashing or quitting unexpectedly results in a failing grade (0).
- You must provide a **Makefile** to compile your source files.  
The Makefile must not perform unnecessary relinking.
- The Makefile must include at least the following rules:
  - `all`, `clean`, `fclean`, `re`, and `$(NAME)`.
- Code must be compiled with `c++` using the following flags:
  - `-Wall -Wextra -Werror`
- Code must adhere to the **C++98 standard** and must still compile when adding `-std=c++98`.
- Prefer C++ features over their C equivalents (e.g., `<cstring>` instead of `<string.h>`).
- External libraries and Boost libraries are **forbidden**.

You are allowed to use C functions, but always prefer their C++ versions if possible.

---

## Mandatory Part
### **Program Name:**  
`ircserv`

### **Turn-In Files:**  
- `Makefile`, `*.{h, hpp, cpp, tpp, ipp}`
- Optional: configuration file.

### **Makefile:**  
- `NAME`, `all`, `clean`, `fclean`, `re`

### **Arguments:**  
- **port:** The listening port for incoming IRC connections.  
- **password:** The connection password required by any IRC client to connect.

### **External Functions Allowed:**
- `socket`, `close`, `setsockopt`, `getsockname`, `getprotobyname`, `gethostbyname`,  
`getaddrinfo`, `freeaddrinfo`, `bind`, `connect`, `listen`, `accept`,  
`htons`, `htonl`, `ntohs`, `ntohl`, `inet_addr`, `inet_ntoa`,  
`send`, `recv`, `signal`, `sigaction`, `lseek`, `fstat`, `fcntl`, `poll` (or equivalents).

### **Description:**  
Develop an IRC server in **C++98**.

You **mustn’t** develop a client.
You **mustn’t** handle server-to-server communication.

Your executable will be run as follows:
```bash
./ircserv <port> <password>
```
- **port:** The port number on which your IRC server will be listening to for incoming
IRC connections.  
- **password:** The connection password. It will be needed by any IRC client that tries to connect to your server.

> ℹ  
> Even if poll() is mentionned in the subject and the evaluation scale,  
> you can use any equivalent such as select(), kqueue(), or epoll().

#### Requirements:
- The server must handle **multiple clients simultaneously** and must not hang.
- **Forking is not allowed.**
- All I/O operations must be **non-blocking.**
- Use only **one** `poll()` (or equivalent) to handle all operations (read, write, listen, etc.).

```
⚠️  
Because you have to use non-blocking file descriptors,  
it is possible to use read/recv or write/send functions with no poll() (or equivalent),  
and your server wouldn’t be blocking.  
But it would consume more system resources.  
Thus, if you try to read/recv or write/send in any file descriptor  
without using poll() (or equivalent), your grade will be 0.
```

- Several IRC clients exist. You have to choose one of them as a reference.  
Your reference client will be used during the evaluation process.  
- Your reference client must be able to connect to your server without encountering any error.  
- Communication between client and server has to be done via TCP/IP (v4 or v6).  
- Using your reference client with your server must be similar to using it with any official IRC server.  
However, you only have to implement the following features:
  - Authentication.
  - Set a nickname and username.
  - Join channels.
  - Send and receive private messages using your reference client.
  - Forward messages sent from one client to a channel to all channel members.
  - Distinguish between *operators* and regular users.
  - Implement **channel operator** commands:
    - `KICK`: Remove a client from a channel.
    - `INVITE`: Invite a client to a channel.
    - `TOPIC`: View or modify the channel topic.
    - `MODE`: Modify channel modes:
      - `i`: *Set/remove* Invite-only channel.
      - `t`: *Set/remove* the restriction of the `TOPIC` command to  channel operators.
      - `k`: *Set/remove* the channel key (password).
      - `o`: *Give/take* channel operator privileges.
      - `l`: *Set/remove* the user limit to channel.

---

#### For macOS only
> ℹ  
> Since macOS doesn’t implement `write()` the same way as other Unix operating systems,  
> you are allowed to use `fcntl()`.  
> You must use file descriptors in **non-blocking** mode  
> in order to get a behavior similar to the one of other Unix operating systems.

```
⚠️  
However, you are allowed to use `fcntl()` only as follows:
fcntl(fd, F_SETFL, O_NONBLOCK);
**Any other flag is forbidden**
```

---

#### Test Example
Verify absolutely every possible error and issue  
(receiving partial data, low bandwidth, and so forth).

To ensure that your server correctly processes everything that you send to it,  
the following simple test using `nc` can be done:
```bash
$> nc 127.0.0.1 6667
com^Dman^Dd
```

Use **Ctrl+D** to send the command in several parts: **'com'**, then **'man'**, then **'d\n'**.  

In order to process a command, you have to first aggregate the received packets in order to rebuild it.

---

## Bonus Part
Extra features you can add to your IRC server so it looks even more like and actual IRC server:

### Additional Features:
- Handle **file transfer**.
- Add a **bot**.

```
⚠️  
The bonus part will only be assessed if the mandatory part is **perfectly implemented**.  
A perfect implementation means all mandatory requirements are met without any malfunctions.  
If you have not passed ALL the mandatory requirements,  
your bonus part will not be evaluated at all.
```
---

## Submission and Peer-Evaluation
- Submit the project via your **Git** repository as usual.  
- Only the files in your repository will be evaluated.  
- Double-check file names and paths before submission.

### Testing:
You are encouraged to create test programs for your project,  
even though they **won't be submitted and won't be graded**.  
These tests could be especially useful to test your server during the defense  
or for evaluating a peer’s project if you have to evaluate another `ft_irc` implementation one day.  
Indeed, you are free to use whatever tests you need during the evaluation process.

---

> ℹ  
> Your reference client will be used during the evaluation process.
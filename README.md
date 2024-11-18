# ft_irc
**Internet Relay Chat - 42**  
**Version:** 7.1  

## Summary
This project involves creating your own IRC server.  
You will use an actual IRC client to connect to your server and test it.  
Understanding solid standard protocols like those used in the Internet is a valuable skill for connected systems and networking.

---

## Introduction
**Internet Relay Chat (IRC)** is a text-based communication protocol that provides real-time messaging. It supports both public and private communication, allowing users to exchange direct messages or join group channels.  
IRC clients connect to IRC servers to participate in channels, while IRC servers interconnect to form a network.

---

## General Rules
- Your program **must not crash** under any circumstances (even when memory is exhausted). Crashing or quitting unexpectedly results in a failing grade (0).
- You must provide a **Makefile** to compile your source files. The Makefile must not perform unnecessary relinking.
- The Makefile must include at least the following rules:
  - `all`, `clean`, `fclean`, `re`, and `$(NAME)`.
- Code must be compiled with `c++` using the following flags:
  - `-Wall -Wextra -Werror`
- Code must adhere to the **C++98 standard** and must still compile when adding `-std=c++98`.
- Prefer C++ features over their C equivalents (e.g., `<cstring>` instead of `<string.h>`).
- External libraries and Boost libraries are **forbidden**.

---

## Mandatory Part
### **Program Name:**  
`ircserv`

### **Turn-In Files:**  
- `Makefile`, `*.{h, hpp, cpp, tpp, ipp}`
- Optional: configuration file.

### **Arguments:**  
```bash
./ircserv <port> <password>
```
- **port:** The listening port for incoming IRC connections.  
- **password:** The connection password required by any IRC client to connect.

### **External Functions Allowed:**
- `socket`, `close`, `setsockopt`, `getsockname`, `getprotobyname`, `gethostbyname`, 
`getaddrinfo`, `freeaddrinfo`, `bind`, `connect`, `listen`, `accept`, 
`htons`, `htonl`, `ntohs`, `ntohl`, `inet_addr`, `inet_ntoa`, 
`send`, `recv`, `signal`, `sigaction`, `lseek`, `fstat`, `fcntl`, `poll` (or equivalents).

### **Description:**  
Develop an IRC server in **C++98**.

#### Key Requirements:
- **No client implementation required.**
- **No server-to-server communication required.**
- The server must handle **multiple clients simultaneously** and must not hang.
- **Forking is not allowed.**
- All I/O operations must be **non-blocking.**
- Use **one `poll()` (or equivalent)** to handle all operations (read, write, listen, etc.).

#### Implementation Features:
- Authentication.
- Set a nickname and username.
- Join channels.
- Send and receive private messages.
- Forward messages to all channel members.
- Distinguish between operators and regular users.
- Implement operator commands:
  - `KICK`: Remove a client from a channel.
  - `INVITE`: Invite a client to a channel.
  - `TOPIC`: View or modify the channel topic.
  - `MODE`: Modify channel modes:
    - `i`: Invite-only channel.
    - `t`: Restrict `TOPIC` command to operators.
    - `k`: Set/remove channel password.
    - `o`: Manage operator privileges.
    - `l`: Set/remove user limit.

---

## Test Example
Verify error handling and behavior under adverse conditions  
(e.g., partial data or low bandwidth).

Example test using `nc`:
```bash
$> nc 127.0.0.1 6667
com^Dman^Dd
```

Use Ctrl+D to send the command in parts (com, then man, then d\n).  
Ensure the server processes and aggregates commands correctly.

---

## Bonus Part
### Additional Features:
- Handle **file transfers**.
- Add a **bot**.

**Note:**  
The bonus will only be assessed if the mandatory part is **perfectly implemented**.  
A perfect implementation means all mandatory requirements are met without any malfunctions.

---

## Submission and Peer-Evaluation
- Submit the project via your Git repository.  
- Only the files in your repository will be evaluated.  
- Double-check file names and paths before submission.

### Testing:
You are encouraged to create test programs for your project.  
While these tests are not graded, they are invaluable for validating your implementation during defense and peer evaluation.

---

## Notes for macOS Users
- Since macOS handles `write()` differently, you may use `fcntl()` as follows:
```cpp
  fcntl(fd, F_SETFL, O_NONBLOCK);
```
---

## Test Example
Verify absolutely every possible error and issue  
(receiving partial data, low bandwidth, and so forth).

To ensure that your server correctly processes everything you send to it,  
the following simple test using `nc` can be done:
```bash
$> nc 127.0.0.1 6667
com^Dman^Dd
$>
```

---

## Bonus Part
Here are the extra features you can add to your IRC server to make it even more like an actual IRC server:

- Handle **file transfers**.
- Add a **bot**.

**Important Note:**  
The bonus part will only be assessed if the mandatory part is **PERFECT**.  
Perfect means the mandatory part has been fully implemented and works without any malfunctions.  
If you fail to meet **ALL** the mandatory requirements, your bonus part will not be evaluated.

---

## Submission and Peer-Evaluation
Turn in your assignment in your Git repository as usual.  
Only the work inside your repository will be evaluated during the defense.  
Don’t hesitate to double-check the names of your files to ensure they are correct.

You are encouraged to create test programs for your project,  
even though they won’t be submitted and won’t be graded.  
These tests could be especially useful to test your server during the defense  
or for evaluating a peer’s project if you have to assess another `ft_irc` implementation one day.

Indeed, you are free to use whatever tests you need during the evaluation process.

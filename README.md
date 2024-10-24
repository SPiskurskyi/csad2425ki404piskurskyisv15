
# Details about repository

This repository contains a project for a hardware-based Tic-Tac-Toe 3x3 game. The game logic and decision-making will primarily run on the board [HW], while the application [SW] will serve as a GUI for users to interact with the game.
  
The HW part will also be described as a "server", the SW part as a "client". Both parts are made using C/C++
## Student details

| Student number | Student creds | Game | config format |

| :--: | :----------------: | :----------------: | :---: |

| 15 | Piskurskyi S. P. | tik-tac-toe 3x3 | INI |

## Technology Stack and Hardware Used

In this project, I am going to use a client-server architecture. The server, which will provide data to the client program, will be represented by an Arduino Uno. The board will perform calculations and return the results to the client program at the end. The language for the project development will be C/C++.

# Task details

## Task 1

Main goals to this task is to create repository with main branch **develop**. Then create branch with name **feature/develop/task1** and create `README.md` file with all descriptions. Also create **TAG** and make pull request.

## Task 2

- Create a simple communication schema SW(client) <-> UART <-> HW(server).
- The client should send a message to the server. The server should modify the message and send it back to the client.
- Create YML file with next features:
    a. build all binaries (create scripts in folder ci/ if need);
    b. run tests;
    c. create artifacts with binaries and test reports;

# TBD: How to build
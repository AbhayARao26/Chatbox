# ChatBox: A Terminal-Based Chatbot in C

ChatBox is a simple yet effective terminal-based chatbot implemented in C. It provides users with the ability to create accounts, send messages, and perform various operations. The chatbot utilizes a priority queue to sort messages based on the sender's name.

## Features

- **User Management:**
  - Create user accounts with a maximum limit of 10 users.
  - Display a list of available users.

- **Messaging:**
  - Send messages between users.
  - Display and sort messages.
  - Filter messages by sender and receiver.
  - Search messages by keyword.

- **User Interface:**
  - Implemented using the [curses library](https://invisible-island.net/ncurses/) for a terminal-based user interface.

## Getting Started

### Prerequisites

- GCC Compiler
- Curses Library

### Usage

1. User Creation:
   Select option 1 to create a user account.
   Enter the user's first name when prompted.
2. Display Users:
   Select option 2 to display a list of available users.
3. Sending Messages:
   Select option 3 to send a message.
   Choose the sender and receiver from the displayed user list.
   Enter your message.
4. Display Messages:
   Select option 4 to display all messages.
5. Filter and Search Messages:
   Use options 5 and 6 to filter messages by sender/receiver or search by keyword.
6. Sorting Messages:
   Select option 7 to sort messages based on the sender's name using a priority queue.
7. Exit:
   Select option 8 to exit the ChatBox.

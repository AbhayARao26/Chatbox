#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curses.h>

#define MAX_USERS 10
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LENGTH 256
#define MAX_HEAP_SIZE 100

typedef struct {
    char username[20];
    int userID;
} User;

typedef struct {
    char senderName[20];
    char receiverName[20];
    char message[MAX_MESSAGE_LENGTH];
    char timestamp[20];
} Message;

typedef struct {
    Message messages[MAX_HEAP_SIZE];
    int size;
} PriorityQueue;

// Function to compare two messages based on their sender names
int compareMessagesBySender(const void *a, const void *b) {
    return strcmp(((Message *)a)->senderName, ((Message *)b)->senderName);
}

// Function to initialize a priority queue
void initPriorityQueue(PriorityQueue *pq) {
    pq->size = 0;
}

// Function to push a message into the priority queue
void push(PriorityQueue *pq, Message *message) {
    if (pq->size < MAX_HEAP_SIZE) {
        pq->messages[pq->size++] = *message;
        qsort(pq->messages, pq->size, sizeof(Message), compareMessagesBySender);
    } else {
        printw("\nPriority queue is full.\n");
    }
}

// Function to pop the message with the earliest sender name from the priority queue
Message pop(PriorityQueue *pq) {
    Message result = pq->messages[0];
    for (int i = 1; i < pq->size; i++) {
        pq->messages[i - 1] = pq->messages[i];
    }
    pq->size--;
    return result;
}

// Function to sort messages by sender using a priority queue
void sortMessagesBySenderUsingPriorityQueue(Message messages[], int messageCount) {
    PriorityQueue pq;
    initPriorityQueue(&pq);

    for (int i = 0; i < messageCount; i++) {
        push(&pq, &messages[i]);
    }

    for (int i = 0; i < messageCount; i++) {
        messages[i] = pop(&pq);
    }
}

void createUser(User users[], int *userCount);
void displayUsers(User users[], int userCount);
void selectUser(User users[], int userCount, char *selectedUserName, int isSender);
void sendMessage(User users[], int userCount, Message messages[], int *messageCount);
void saveMessageToCSV(Message *message);
void displayMessages(Message messages[], int messageCount);
void filterMessagesByUser(Message messages[], int messageCount);
void searchMessagesByWord(Message messages[], int messageCount);

int main() {
    initscr();
    keypad(stdscr, TRUE);
    int userCount = 0;
    int messageCount = 0;
    int option;

    User users[MAX_USERS];
    Message messages[MAX_MESSAGES];

    do {
        clear();
        printw("1. Create User\n2. Display Users\n3. Send Message\n");
        printw("4. Display Messages\n5. Filter Messages by User\n");
        printw("6. Search Messages by Word\n7. Sort Messages by Sender\n8. Exit\n");
        printw("Enter your choice: ");
        refresh();

        option = getch();

        switch (option) {
            case '1':
                createUser(users, &userCount);
                break;

            case '2':
                displayUsers(users, userCount);
                break;

            case '3':
                sendMessage(users, userCount, messages, &messageCount);
                break;

            case '4':
                displayMessages(messages, messageCount);
                break;

            case '5':
                filterMessagesByUser(messages, messageCount);
                break;

            case '6':
                searchMessagesByWord(messages, messageCount);
                break;

            case '7':
                sortMessagesBySenderUsingPriorityQueue(messages, messageCount);
                printw("\nMessages sorted based on sender's name using priority queue.\n");
                break;

            case '8':
                printw("\nExiting...\n");
                break;

            default:
                printw("Invalid option. Please try again.\n");
        }

        refresh();
        getch();
    } while (option != '8');

    endwin();

    return 0;
}

void createUser(User users[], int *userCount) {
    if (*userCount < MAX_USERS) {
        printw("\nEnter firstname: ");
        refresh();
        scanw("%s", users[*userCount].username);
        users[*userCount].userID = *userCount + 1;
        (*userCount)++;
        printw("\nUser created successfully.\n");
    } else {
        printw("\nMaximum number of users reached.\n");
    }
}

void displayUsers(User users[], int userCount) {
    if (userCount == 0) {
        printw("\nNo users available.\n");
    } else {
        printw("\nUsers:\n");
        for (int i = 0; i < userCount; i++) {
            printw("%d. %s\n", i + 1, users[i].username);
        }
    }
    refresh();
}

void selectUser(User users[], int userCount, char *selectedUserName, int isSender) {
    int i;

    printw("\nAvailable Users:\n");
    for (i = 0; i < userCount; i++) {
        printw("%d. %s\n", i + 1, users[i].username);
    }

    if (isSender) {
        printw("\nEnter the sender's user number: ");
    } else {
        printw("\nEnter the receiver's user number: ");
    }

    int userNumber;
    scanw("%d", &userNumber);

    if (userNumber < 1 || userNumber > userCount) {
        printw("\nInvalid user number. Please try again.\n");
        *selectedUserName = '\0';
    } else {
        strcpy(selectedUserName, users[userNumber - 1].username);
    }
    refresh();
}

void sendMessage(User users[], int userCount, Message messages[], int *messageCount) {
    char senderName[20], receiverName[20];
    char messageText[MAX_MESSAGE_LENGTH];

    selectUser(users, userCount, senderName, 1);
    if (senderName[0] != '\0') {
        selectUser(users, userCount, receiverName, 0);
        if (receiverName[0] != '\0') {
            printw("\nEnter your message: ");
            refresh();
            scanw(" %[^\n]", messageText);

            if (*messageCount < MAX_MESSAGES) {
                strncpy(messages[*messageCount].senderName, senderName, sizeof(messages[*messageCount].senderName));
                strncpy(messages[*messageCount].receiverName, receiverName, sizeof(messages[*messageCount].receiverName));
                strncpy(messages[*messageCount].message, messageText, MAX_MESSAGE_LENGTH);

                time_t rawtime;
                struct tm *info;
                time(&rawtime);
                info = localtime(&rawtime);
                strftime(messages[*messageCount].timestamp, sizeof(messages[*messageCount].timestamp), "%Y-%m-%d %H:%M:%S", info);

                saveMessageToCSV(&messages[*messageCount]);

                (*messageCount)++;
                printw("\nMessage sent successfully.\n");
            } else {
                printw("\nMaximum number of messages reached.\n");
            }
        }
    }
    refresh();
}

void saveMessageToCSV(Message *message) {
    FILE *file = fopen("messages.csv", "a");
    if (file != NULL) {
        fprintf(file, "\n%s,%s,%s,%s\n", message->senderName, message->receiverName, message->message, message->timestamp);
        fclose(file);
    } else {
        printw("\nError opening the file.\n");
    }
    refresh();
}

void displayMessages(Message messages[], int messageCount) {
    if (messageCount == 0) {
        printw("\nNo messages available.\n");
    } else {
        printw("\nMessages:\n");
        for (int i = 0; i < messageCount; i++) {
            printw("Sender: %s\n", messages[i].senderName);
            printw("Receiver: %s\n", messages[i].receiverName);
            printw("Message: %s\n", messages[i].message);
            printw("Timestamp: %s\n", messages[i].timestamp);
            printw("\n");
        }
    }
    refresh();
}

void filterMessagesByUser(Message messages[], int messageCount) {
    if (messageCount == 0) {
        printw("\nNo messages available.\n");
    } else {
        char senderName[20], receiverName[20];
        printw("\nEnter the sender's name: ");
        scanw("%s", senderName);
        printw("\nEnter the receiver's name: ");
        scanw("%s", receiverName);

        printw("\nFiltered Messages:\n");
        int found = 0;
        for (int i = 0; i < messageCount; i++) {
            if ((strcmp(messages[i].senderName, senderName) == 0) &&
                (strcmp(messages[i].receiverName, receiverName) == 0)) {
                printw("Sender: %s\n", messages[i].senderName);
                printw("Receiver: %s\n", messages[i].receiverName);
                printw("Message: %s\n", messages[i].message);
                printw("Timestamp: %s\n", messages[i].timestamp);
                printw("\n");
                found = 1;
            }
        }
        if (!found) {
            printw("\nNo messages sent by %s to %s\n", senderName, receiverName);
        }
    }
    refresh();
}

void searchMessagesByWord(Message messages[], int messageCount) {
    if (messageCount == 0) {
        printw("\nNo messages available.\n");
    } else {
        char searchWord[20];
        printw("\nEnter the word to search for: ");
        scanw("%s", searchWord);

        printw("\nSearch Results:\n");
        int found = 0;
        for (int i = 0; i < messageCount; i++) {
            if (strstr(messages[i].message, searchWord) != NULL) {
                printw("Sender: %s\n", messages[i].senderName);
                printw("Receiver: %s\n", messages[i].receiverName);
                printw("Message: %s\n", messages[i].message);
                printw("Timestamp: %s\n", messages[i].timestamp);
                printw("\n");
                found = 1;
            }
        }
        if (!found) {
            printw("Word not found.\n");
        }
    }
    refresh();
}

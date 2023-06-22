#ifndef SERVER_H_
#define SERVER_H_

#define FILE_MEMORY_KEY 2023
#define FILENAME_MEMORY_KEY 2024
#define NEW_USER_FLAG 2025

#define CLIENT_SEMAPHORE_NAME "user"

void userListener(void);

#endif
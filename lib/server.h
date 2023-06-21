#ifndef SERVER_H_
#define SERVER_H_

#define FILE_MEMORY_KEY 2023
#define FILENAME_MEMORY_KEY 2024

#define SERVER_SEMAPHORE_NAME "IC STORE"
#define CLIENT_SEMAPHORE_NAME "user"

#define MAX_USER_COUNT 10

void userListener(void);

#endif
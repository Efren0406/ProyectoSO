#ifndef SERVER_H_
#define SERVER_H_

#define SHARED_MEMORY_KEY 2023
#define SERVER_SEMAPHORE_NAME "IC STORE"
#define MAX_USER_COUNT 10

void userListener(void *userNum);

#endif
#ifndef SERVER_H_
#define SERVER_H_

#define SHARED_MEMORY_KEY1 2023
#define SHARED_MEMORY_KEY2 2024
#define SHARED_MEMORY_KEY3 2025
#define SHARED_MEMORY_KEY4 2026

#define SERVER_SEMAPHORE_NAME "IC STORE"
#define MAX_USER_COUNT 10

void userListener(void *userNum);

#endif
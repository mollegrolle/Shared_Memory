// ==========================================================================
// Made by Leonard Grolleman 2021
//
// A communication application where two console applications can read and 
// write to a shared memory. 
//
// The user can choose whether a console application will act as a
// producer or consumer, the delay of which to write/read between
// each message, memory size of the shared memory, the number of 
// messages and a fixed size for every message or random sizes
//
// If the size of a message is half or more than the shared memory size, the
// message will be skipped. If a message were to exceed beyond the
// shared memory size limit, the message will be split into two. If there is
// no freespace available, the producer will hold on to its message till
// space is available. Consumer will always try to read as long as freespace
// is greater than 0
// ==========================================================================

#include <Windows.h>
#include <iostream>
#include <conio.h>
#include "ComLib.h"
#include "random_example.h"

//$(ProjectName)
//shared.exe producer 10 2 50 random
//shared.exe consumer 10 2 50 random

//exe role delayMs memSize numMsg msgSize

#define WAIT_MS 100

int main(int argc, char** argv) {

	if (argc == 6)
	{
		std::string exe = argv[0];
		std::string role = argv[1];
		unsigned int delayMs = atoi(argv[2]);
		unsigned int memorySizeKb = atoi(argv[3]) * 1024;
		unsigned int numMessages = atoi(argv[4]);
		auto messageSizeBytes = argv[5];

		if (role == "producer" || role == "Producer")
		{
			ComLib myComlib("mySecret", memorySizeKb);

			size_t length;
			char* msg = nullptr;
			unsigned int count = 0;
			bool wait = false;

			// Loop until all messenges has been written or till ESC is pressed
			while (count < numMessages)
			{
				Sleep(delayMs);

				if (!wait)
				{
					length = strcmp(messageSizeBytes, "random") == 0 ? (rand() % 200)+1 : atoi(messageSizeBytes);

					msg = gen_random((int)length);
				}

				if (myComlib.send(msg, length))
				{
					count++;

					wait = false;
					printf_s("%i %.*s\n", (int)count, (int)length, msg);
					
					delete(msg);
				}
				else
				{
					//Sleep(WAIT_MS);

					wait = true;
					//std::cout << "Produce waiting..." << std::endl;
				}
			}
		}

		else if (role == "consumer" || role == "Consumer")
		{
			ComLib myComlib("mySecret", memorySizeKb);

			size_t length;
			unsigned int count = 0;
			char* msg = nullptr;

			// Loop until all messenges has been read or till ESC is pressed
			while (count < numMessages)
			{
				Sleep(delayMs);

				if (myComlib.recv(msg, length))
				{
					count++;

					printf_s("%i %.*s\n", (int)count, (int)length, msg);

					delete(msg);
				}
				else
				{
					//Sleep(WAIT_MS);

					//std::cout << "Consume waiting..." << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Error: Did not specify a correct role." << std::endl;
			std::cout << "Expected 'Producer' or 'Consumer'." << std::endl;
		}
	}
	else
	{
		std::cout << "Error: Expects the following in parameters: role, delay, memorySize, numMessages, messageSize" << std::endl;
		std::cout << std::endl;
		return 0;
	}

	return 0;
}
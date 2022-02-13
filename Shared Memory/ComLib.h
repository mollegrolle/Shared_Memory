#pragma once

#include <Windows.h>
//#include <tchar.h>
//#include <stdio.h>
#include <iostream>
//#include <dos.h>
#include <memory.h>


class ComLib
{
private:
	HANDLE hFileMap;
	char* mData;

	struct SharedHeader
	{
		size_t offset_head = 0;	// Location of head, where producer is writing
		size_t offset_tail = 0;	// Location of tail, where consumer is reading
		size_t freespace = 0;	// Current freespace in buffer
		size_t cBuffer = 0;		// Location to start of circular buffer
		size_t cBufferSize = 0;	// Buffer size
	} sh;

	struct MsgHeader
	{
		size_t id = 0;
		size_t totalSize = 0;
		size_t msgLength = 0;
		size_t pad = 0;
	} mh;

	HANDLE hMutex;

public:
	ComLib(const std::string& secret, const size_t& buffSize);

	bool send(const void* msg, const size_t length);

	bool recv(char*& msg, size_t& length);

	size_t nextLength();

	~ComLib();
};
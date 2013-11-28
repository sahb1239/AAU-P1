#define ERROR_OCCURRED -1
#define FILE_ROOMS "rooms.txt"

typedef struct {
	char room[40];
} HOUSE;

int readRooms (HOUSE rooms[]);
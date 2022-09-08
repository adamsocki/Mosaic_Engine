#include "examples/my_game_input.cpp"


enum EntityType{
	EntityType_Player,
	EntityType_Enemy,

	EntityType_PlayerBullet,

	EntityType_Count;
};


struct EntityInfo {
	int32 generation;
	int32 indexInBuffer;

	EntityType type;
};


struct EntityTypeBuffer {
	int32 count;
	int32 entitySize;
	int32 capacity;
	void* entities;
};

struct EntityManager {
	int32 entityCapacity;
	int32 count;
	EntityInfo* entities;

	EntityTypeBuffer buffers[EntityTypeCount];
};



struct MyData {

	EntityManager em;
};



void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;

	Data->em.entityCapacity = 100;




}

void MyGameUpdate() {


}
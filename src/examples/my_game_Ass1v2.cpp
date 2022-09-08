#include "examples/my_game_input.cpp"

enum EntityType {
	EntityType_Player,
	EntityType_Enemy,
	
	EntityType_PlayerBullet,

	EntityType_Count,
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

MyData* data = NULL;

void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;

	// Step 1: SET UP ENTITY MANAGER
	Data->em.entityCapacity = 100;
		// allocate memory for EntityManger
		// the structure of entities is based on the EntityInfo structure
	Data->em.entities = (EntityInfo*)malloc(sizeof(EntityInfo) * Data->em.entityCapacity);
		// initialize to zero ???
	memset(em->entities, 0, sizeof(EntityInfo) * em.entityCapacity);




}

void MyGameUpdate() {


}
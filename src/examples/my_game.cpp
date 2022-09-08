#include "my_game_input.cpp"

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

struct EntityHandle {
	EntityType type;
	int32 id;
	int32 generation;
};

struct EntityTypeBuffer {
	int32 count;
	int32 entitySizeByte;
	int32 capacity;

	void* entities;
};

struct EntityManager {
	int32 entityCapacity;
	int32 nextID;
	EntityInfo* entities;

	EntityTypeBuffer buffers[EntityType_Count];
};


struct Entity {
	vec2 position;
	Sprite* sprite;

	EntityHandle handle;
};

struct Player : Entity {
	real32 speed;
};

struct Enemy : Entity {

};

struct MyData {
	EntityManager em;

	Sprite playerSprite;
	Sprite enemySprite;
};

MyData* Data = NULL;

int32 freeList[100];
int32 freeListCount = 0;


void DeleteEntity(EntityManager* em, EntityHandle handle) {
	

	freeList[freeListCount] = handle.id;
	freeListCount++;

	EntityTypeBuffer* buffer = &em->buffers[handle.type];

	EntityInfo* info = &em->entities[handle.id];

	Entity* e = (Entity*)buffer->entities;
	
	e[info->indexInBuffer] = e[buffer->count - 1];

	buffer->count--;
}


EntityHandle AddEntity(EntityManager* em, EntityType type) {
	
	// NEED TO DO FREELIST HERE
	// calculate which index is coming up next that is free
	int32 nextFreeIndex = em->nextID;
	//		check to see if there are any free values that we can utilize
	//		these will only arrise when we have deleted some entity
	if (freeListCount > 0) {
		nextFreeIndex = freeList[freeListCount - 1];
		freeListCount--;
	}

	// Create handle for function's return and to be placed into the Entity within the buffer
	EntityHandle handle = {};
	
	// Find a new ID to add that will create a new EntityInfo
	EntityInfo* info = &em->entities[nextFreeIndex];
	
	// Add the type we are adding to the info's type
	info->type = type;

	// Fetch the Buffer using the type which contains our entities for each entity type
	EntityTypeBuffer* buffer = &em->buffers[type];

	// use the next count that is available in that buffer for the info that is being used
	info->indexInBuffer = buffer->count;

	// increase the buffer's count
	buffer->count++;
	em->nextID++;

	// generate the handle
	// Add the Generation from info to the handle ???
	handle.generation = info->generation;
	// Add the buffer's nextID to the handle
	handle.id = nextFreeIndex;
	// add the type to the handle
	handle.type = type;
	/*
	// get the entity buffer allocated with the handle
	Entity* e = (Entity *)buffer->entities;
	e[info->indexInBuffer].handle = handle;
	//ffer.entities[nextFreeIndex].handle = handle;
	*/
	return handle;

}

void *GetEntity(EntityManager* em, EntityHandle handle) {
	// use the HANDLE to find the entity INFO
	
	// make sure there are sufficent entity capacities avaialable
	if (handle.id >= em->entityCapacity) {
		return NULL;
	}

	//use the handle to retrive the info
	EntityInfo* info = &em->entities[handle.id];

	// make sure that the generation's match
	if (info->generation != handle.generation) {
		return NULL;
	}


	//retrieve the buffer that we are looking for by type
	EntityTypeBuffer* buffer = &em->buffers[info->type];
	// find the place for that specific buffer in the memory using the index in the buffer that is in the EntityInfo
	return ((u8*)buffer->entities + (buffer->entitySizeByte * info->indexInBuffer));
	
}

void InitializeEntityManager() {

	// Step 1: SET UP ENTITY MANAGER
	Data->em.entityCapacity = 200;
	// allocate memory for EntityManger
	// the structure of entities is based on the EntityInfo structure
	Data->em.entities = (EntityInfo*)malloc(sizeof(EntityInfo) * Data->em.entityCapacity);
	// initialize to zero ???
	memset(Data->em.entities, 0, sizeof(EntityInfo) * Data->em.entityCapacity);
	// Step 1: Set our NextID counter to 0;
	Data->em.nextID = 0;

	// Step 2: CREATE PLAYER BUFFER
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	// Determine the size of the Entity
	playerBuffer->entitySizeByte = sizeof(Player);
	// How many can be stored in here (will use for allocation below)
	playerBuffer->capacity = 10;
	// Set the counter equal to 0 so it starts at the beginning
	playerBuffer->count = 0;
	// Allocate the memory for the entities within the buffer 
	playerBuffer->entities = malloc(playerBuffer->entitySizeByte * playerBuffer->capacity);
	memset(playerBuffer->entities, 0, sizeof(EntityTypeBuffer) * playerBuffer->capacity);

	// Step 3: CREATE BASE BUFFER

	
	// Step 4: CREATE ENEMY BUFFER
	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	// Determine the size of the Entity
	enemyBuffer->entitySizeByte = sizeof(Enemy);
	// How many can be stored in here (will use for allocation below)
	enemyBuffer->capacity = 100;
	// Set the counter equal to 0 so it starts at the beginning
	enemyBuffer->count = 0;
	// Allocate the memory for the entities within the buffer
	enemyBuffer->entities = malloc(enemyBuffer->entitySizeByte * enemyBuffer->capacity);
	//memset(playerBuffer->entities, 0, sizeof(EntityTypeBuffer) * playerBuffer->capacity);
	memset(enemyBuffer->entities, 0, sizeof(EntityTypeBuffer) * enemyBuffer->capacity);
	


	
}

void LoadSprites() {
	LoadSprite(&Data->playerSprite, "data/galaga_ship.png");
	LoadSprite(&Data->enemySprite, "data/player_guy.png");
}


void PlayerInputMove(Player* p) {
	if (InputHeld(Input, Input_Up)) {
		p->position.y += p->speed * Game->deltaTime;
	}
	if (InputHeld(Input, Input_Down)) {
		p->position.y -= p->speed * Game->deltaTime;
	}
	if (InputHeld(Input, Input_Left)) {
		p->position.x -= p->speed * Game->deltaTime;
	}
	if (InputHeld(Input, Input_Right)) {
		p->position.x += p->speed * Game->deltaTime;
	}
}

void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;

	LoadSprites();

	// Step 1: Initialize Entity Manager 
	InitializeEntityManager();

	// Step 2: ADD PLAYER ENTITY
	EntityHandle playerHandle = AddEntity(&Data->em, EntityType_Player);
	
	// Step 3: GET PLAYER ENTITY
	Player* p = (Player *)GetEntity(&Data->em, playerHandle);
	// set player entity values
	p->position = V2(0, 0);
	p->speed = 2;
	p->sprite = &Data->playerSprite;


	// Step 4: Test Deletion of Enemy

	for (int i = 0; i < 8; i++) {
		EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
		Enemy* e = (Enemy*)GetEntity(&Data->em, enemyHandle);
		e->position = V2(1, i);
		e->sprite = &Data->enemySprite;
	}

}



void MyGameUpdate() {
	
	// LOGIC

	//		PLAYER LOGIC
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	int32 numOfPlayers = playerBuffer->count;
	Player* p = (Player*)playerBuffer->entities;
	PlayerInputMove(p);
	//MouseDirection();

	//		ENEMY LOGIC
	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	int32 numOfEnemies = enemyBuffer->count;
	Enemy* e = (Enemy*)enemyBuffer->entities;


	if (InputPressed(Input, Input_Space)) {
		EntityHandle enemyHandle;
		enemyHandle.id = 1;
		enemyHandle.type = EntityType_Enemy;
		DeleteEntity(&Data->em, enemyHandle);
	}
	if (InputPressed(Input, Input_UpArrow)) {
		EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
		Enemy* e = (Enemy*)GetEntity(&Data->em, enemyHandle);
		e->position = V2(1, 4);
		e->sprite = &Data->enemySprite;
	}

	// RENDER
	ClearColor(RGB(0.4f, 0.0f, 0.0f));

	//		RENDER BASE

	//		RENDER PLAYER
	for (int i = 0; i < numOfPlayers; i++) {
		DrawSprite(p[i].position, V2(1, 1), p[i].sprite);
	}

	//		RENDER ENEMY
	for (int i = 0; i < numOfEnemies; i++) {
		DrawSprite(e[i].position, V2(0.1f, 0.1f), e[i].sprite);
	}



}
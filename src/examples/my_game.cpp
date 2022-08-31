
//enum EntityType {
//	EntityType_Player,
//	EntityType_Enemy,
//	EntityType_Count
//};
//
//struct EntityInfo {
//	EntityType type;
//	int32 generation;
//	int32 indexInBuffer;
//};
//
//struct EntityHandle {
//	int32 id;
//	int16 generation;
//	EntityType type;
//};
//
//struct EntityManager {
//	int32 count;
//	int32 capacity;
//	EntityInfo* entities;
//	int32 nextFreeID;
//};
//
//struct EntityTypeBuffer {
//    int32 count;
//    int32 entitySize;
//    int32 capacity;
//    void *entities;
//	
//	//EntityTypeBuffer buffers[EntityType_Count];
//};
//
//
//EntityHandle AddEntity(EntityManager *entityManager, EntityType type) {
//	
//	EntityHandle entityHandle = {};
//
//	EntityInfo *entityInfo = entityManager->entities[entityManager->nextFreeID];
//	
//	
//	/*info->type = type;
//	
//	EntityTypeBuffer *buffer = &entityManager->buffers[type];
//	info->indexInBuffer = buffer->count;
//	
//	buffer->count++;
//	
//	entityHandle.id = entityManager->nextFreeID;
//	entityHandle.generation = info->generation;
//	
//	entityManager->nextID++;*/
//	
//	return entityHandle;
//}
//
//EntityManager *em = NULL;

enum EntityType {
	EntityType_Player,
	EntityType_Count   
};

struct EntityTypeBuffer {
	int32 count;
	int32 entitySize;
	int32 capacity;
	void* entities;
};

struct EntityManager {
	EntityTypeBuffer buffers[EntityType_Count];
};

struct MyData {
	Sprite sprite;
};

MyData* Data = NULL;
EntityManager* em = NULL;

void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;
	LoadSprite(&Data->sprite, "data/player_guy.png");
	
}

void MyGameUpdate() {
	// LOGIC
	
	// RENDER
	ClearColor(RGB(0.4f, 0.0f, 0.0f));
	DrawSprite(V2(0), V2(0.3f,0.3f), &Data->sprite);
}



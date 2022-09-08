

enum EntityType {
	EntityType_Player,
	EntityType_Enemy,
	EntityType_PlayerBullet,

	EntityType_Base,
	EntityType_Count,
};

struct EntityInfo {
	// the id number is implicit, it's just the index into array
	EntityType type;
	int32 generation;
	int32 indexInBuffer;
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

	EntityTypeBuffer buffers[EntityType_Count];
};

struct EntityHandle {
	int32 id;
	int32 generation;
	
	EntityType type;
};

struct MyData {
	EntityManager em;
};

struct Entity {
	vec2 position;
	Sprite* sprite;
	real32 width;
	real32 lifetime;
	real32 timeToDelete;
	EntityHandle handle;
};

struct Player: Entity {
	//Entity entity;
	//int32 index;
	//vec2 position;
	int32 hitPoints;
	real32 playerMovementTimer;
};

struct PlayerBullet : Entity {
	vec2 velocity;
	
};

struct Enemy : Entity {

};

struct FreeList {
	int32 generation;
	int32 count;
};

int32 freeList_Enemy[1000];
int32 freeListEnemy_Count = 0;

MyData* Data = NULL;

bool CollisionCircle(Circle c2, Circle c1) {
	bool collision = false;
	real32 distanceX = c1.center.x - c2.center.x;
	real32 distanceY = c1.center.y - c2.center.y;

	real32 radiiSum = c1.radius + c2.radius;
	real32 distance = Distance(c1.center.x, c1.center.y, c2.center.x, c2.center.y);
	DrawTextScreenPixel(&Game->monoFont, V2(540, 221), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%d", distance);
	if (Distance(c1.center.x, c1.center.y, c2.center.x, c2.center.y) <= Square(radiiSum)) {
		collision = true;
	}
	return collision;
}

void* GetEntity(EntityManager* em, EntityHandle handle) {
	//EntityInfo* info = GetEntityInfo(em, handle);

	EntityInfo* info = &em->entities[handle.id];
	
	if (info->generation != handle.generation) {
		return NULL;
	}
	/*
	if (info->type != handle.type) {
		return NULL;
	}*/
	//return info;
	EntityTypeBuffer* buffer = &em->buffers[info->type];
	return ((u8*)buffer->entities + (buffer->entitySize * info->indexInBuffer));
}

void DeleteEntity(EntityManager* em, EntityType type, int32 indexToRemove) {
	//EntityHandle handle = {};

	freeList_Enemy[freeListEnemy_Count] = indexToRemove;
	freeListEnemy_Count++;

	//EntityInfo* info = &em->entities[indexToRemove];
	//info->generation++;
	//info->

	EntityTypeBuffer* buffer = &em->buffers[type];
	
	Entity* e = (Entity*)buffer->entities;
	//info->indexInBuffer = buffer->count;
	e[indexToRemove] = e[buffer->count-1];

	buffer->count--;
}

EntityHandle AddEntity(EntityManager* em, EntityType type) {
	EntityHandle handle = {};

	
	int32 nextFreeIndex = em->count;

	if (freeListEnemy_Count > 0) {
		nextFreeIndex = freeList_Enemy[freeListEnemy_Count - 1];
		freeListEnemy_Count--;
	}
	freeListEnemy_Count++;

	EntityInfo* info = &em->entities[nextFreeIndex];
	info->type = type;

	EntityTypeBuffer* buffer = &em->buffers[type];
	info->indexInBuffer = buffer->count;

	buffer->count++;

	handle.id = nextFreeIndex;
	handle.generation = info->generation;

	em->count++;

	return handle;
}
	// ...
	// See if the freelist has anything in it, if so remove the last item (this is known as a PopBack) then we can reuse that ID. Otherwise add to the entity info.
	// Now we must add an entityInfo and set it up



void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;

	Data->em.entityCapacity = 20000;
	Data->em.entities = (EntityInfo*)malloc(sizeof(EntityInfo) * Data->em.entityCapacity);
	memset(Data->em.entities, 0, sizeof(EntityInfo) * Data->em.entityCapacity);

	Data->em.count = 0;

	EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base];

	baseBuffer->entitySize = sizeof(Entity);
	baseBuffer->capacity = 1000;
	baseBuffer->count = 0;
	baseBuffer->entities = malloc(baseBuffer->entitySize * baseBuffer->capacity);

	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	playerBuffer->entitySize = sizeof(Player);
	playerBuffer->capacity = 200;
	playerBuffer->count = 0;
	playerBuffer->entities = malloc(playerBuffer->entitySize * playerBuffer->capacity);

	EntityHandle playerHandle = AddEntity(&Data->em, EntityType_Player);

	Player* p = (Player*)GetEntity(&Data->em, playerHandle);

	p->position = V2(2, 2);
	p->handle = playerHandle;
	p->sprite = (Sprite*)malloc(sizeof(Sprite));
	p->playerMovementTimer = 0;
	LoadSprite(p->sprite, "data/player_guy.png");

	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	enemyBuffer->entitySize = sizeof(Enemy);
	enemyBuffer->capacity = 100000;
	enemyBuffer->count = 0;
	enemyBuffer->entities = malloc(enemyBuffer->entitySize * enemyBuffer->capacity);

	EntityTypeBuffer* playerBulletBuffer = &Data->em.buffers[EntityType_PlayerBullet];
	playerBulletBuffer->entitySize = sizeof(PlayerBullet);
	playerBulletBuffer->capacity = 100000;
	playerBulletBuffer->count = 0;
	playerBulletBuffer->entities = malloc(playerBulletBuffer->entitySize * playerBulletBuffer->capacity);

	for (int i = 0; i < 8; i++) {
		EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
		Enemy* e = (Enemy*)GetEntity(&Data->em, enemyHandle);
		e->position = V2(4 - (2*i), -3 + i);
		e->handle = enemyHandle;
		//e->sprite = (Sprite*)malloc(sizeof(Sprite));
		LoadSprite(e->sprite, "data/player_bolt.png");
		e->width = 1;
	}
}

void MyGameUpdate() {
	// LOGIC
	
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	int32 numOfPlayers = playerBuffer->count;
	EntityTypeBuffer* enemyBuffer = &Data->em.buffers[EntityType_Enemy];
	int32 numOfEnemies = enemyBuffer->count;
	EntityTypeBuffer* playerBulletBuffer = &Data->em.buffers[EntityType_PlayerBullet];
	int32 numOfPlayerBullets = playerBulletBuffer->count;

	for (int i = 0; i < numOfPlayers; i++) {
		Player* p = (Player *)playerBuffer[i].entities;

		//PlayerInput(p);
		// MOVE PLAYER INPUT DETECTION
		
		if (InputPressed(Input, Input_Up)) {
			p->position.y += 0.3f;
		}
		if (InputPressed(Input, Input_Down)) {
			p->position.y -= 0.3f;
		}
		if (InputPressed(Input, Input_Right)) {
			p->position.x += 0.3f;
		}
		if (InputPressed(Input, Input_Left)) {
			p->position.x -= 0.3f;
		}

		if (InputHeld(Input, Input_Up)) {
			p->playerMovementTimer += Game->deltaTime;
			if (p->playerMovementTimer > 0.2f) {
				p->position.y += 0.3f;
				p->playerMovementTimer = 0;
			}
		}
		if (InputPressed(Input, Input_Down)) {
			p->position.y -= 0.3f;
		}
		if (InputPressed(Input, Input_Right)) {
			p->position.x += 0.3f;
		}
		if (InputPressed(Input, Input_Left)) {
			p->position.x -= 0.3f;
		}
		
	}


	// PlayerBullet
	// Collision Detection to Enemy from Bullet
	for (int i = 0; i < numOfPlayerBullets; i++) {
		bool collision = false;
		PlayerBullet* pb = (PlayerBullet*)playerBulletBuffer->entities;
		
		// POSITION CHANGE FOR BULLETS
		pb[i].position.x += Game->deltaTime * pb[i].velocity.x;
		pb[i].lifetime += Game->deltaTime;
		Circle bullet_circle;
		bullet_circle.center = pb[i].position;
		bullet_circle.radius = pb[i].width;


		// COLLISION DETECTION TO ENEMY
		for (int j = 0; j < numOfEnemies; j++) {
			Enemy* e = (Enemy*)enemyBuffer->entities;
			Circle enemy_circle;
			enemy_circle.center = e[j].position;
			enemy_circle.radius = e[j].width;
 			if (CollisionCircle(enemy_circle, bullet_circle)) {
				DeleteEntity(&Data->em, EntityType_Enemy, j);
				DeleteEntity(&Data->em, EntityType_PlayerBullet, i);
				collision = true;
				break;
			}
		}
		if (collision) {
			numOfPlayerBullets--;
			break;
		}
	}
	
	if (InputPressed(Input, Input_Space)) {

		EntityHandle playerBulletHandle = AddEntity(&Data->em, EntityType_PlayerBullet);
		PlayerBullet* pb = (PlayerBullet*)GetEntity(&Data->em, playerBulletHandle);
		Player* p = (Player*)playerBuffer[0].entities;
		pb->position = p->position;
		pb->handle = playerBulletHandle;
		pb->sprite = (Sprite*)malloc(sizeof(Sprite));
		pb->velocity = V2(10, 0);
		pb->lifetime = 0;
		pb->timeToDelete = 2.1f;
		LoadSprite(pb->sprite, "data/player_bolt.png");
		pb->width = 1;
	}

	
	// TESTCODE for add and delete
	/*
	if (InputPressed(Input, Input_Space)) {
		DeleteEntity(&Data->em, EntityType_Enemy, 6);
	}
	if (InputPressed(Input, Input_Z)) {
		EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
		Enemy* e = (Enemy*)GetEntity(&Data->em, enemyHandle);
		e->position = V2(4,-1);
		e->handle = enemyHandle;
		e->sprite = (Sprite*)malloc(sizeof(Sprite));
		LoadSprite(e->sprite, "data/player_bolt.png");
	}
	*/
	// RENDER
	ClearColor(RGB(0.4f, 0.0f, 0.0f));
	DrawTextScreenPixel(&Game->monoFont, V2(540, 221), 13.0f, RGB(1.0f, 1.0f, 1.0f), "%d", numOfEnemies);
		//	Player
	for (int i = 0; i < numOfPlayers; i++) {
		Player* p = (Player*)playerBuffer->entities;
		DrawSprite(p[i].position, V2(1, 1), p[i].sprite);
	}
		//	Enemy
	for (int i = 0; i < numOfEnemies; i++) {
		Enemy* e = (Enemy*)enemyBuffer->entities;
		DrawSprite(e[i].position, V2(1, 1), e[i].sprite);
	}

		// PlayerBullet
	for (int i = 0; i < numOfPlayerBullets; i++) {
		PlayerBullet* pb = (PlayerBullet*)playerBulletBuffer->entities;
		
		if (pb[i].lifetime > pb[i].timeToDelete) {
			DeleteEntity(&Data->em, EntityType_PlayerBullet, i);
		}
		else {
 			DrawSprite(pb[i].position, V2(1, 1), pb[i].sprite);
		}
		
	}
}


	//EntityHandle playerHandle = Data->em.buffers[EntityType_Player].handle;
		//EntityHandle playerHandle = &Data->em.buffers
		//Player* p = &Data->em.
		// LOGICData->em.buffers[EntityType_Player].entities
	//int32 numOfPlayers = sizeof(Data->em.buffers[EntityType_Player].entities);
	
	//for (int i = 0; i < Data->em.buffers[EntityType_Player].entities; )
	//Player* p = .han;
	//Player* p = NULL;
	
	/*
	for (int i = 0; i < 1; i++) {
		p = (Player *) &Data->em.buffers[i].entities;
		//p->position.y = 4;
	}
	*/
/*
	if (InputPressed(Input, Input_Up)) {
		EntityHandle enemyHandle = AddEntity(&Data->em, EntityType_Enemy);
		Enemy* e = (Enemy*)GetEntity(&Data->em, enemyHandle);
		e->position.x = posE;
		posE++;
		e->sprite = (Sprite*)malloc(sizeof(Sprite));
		LoadSprite(e->sprite, "data/player_bolt.png");
	}

	// RENDER
	ClearColor(RGB(0.4f, 0.0f, 0.0f));
	//DrawSprite(p->position, V2(0.3f, 0.3f), p->sprite);

	for (int i = 0; i < posE; i++) {
		//E
		//Enemy* e = (Enemy *)Data->em.buffers[i].entities;
		//Enemy *e = (Enemy *)Get
		//DrawSprite(e->position, V2(0.3f, 0.3f), e->sprite);
	}
	*/


	/*
	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];

	playerBuffer->entitySize = sizeof(Player);
	playerBuffer->capacity = 4;
	playerBuffer->count = 0;
	playerBuffer->entities = malloc(playerBuffer->entitySize * playerBuffer->capacity);


	playerBuffer->count = AddEntity(EntityType_Player, playerBuffer->entities);
	//MyData* data2 = Data;
	Player* p = (Player*)GetEntity(EntityType_Player, 0);
	p->position.x = 3;
	p->position.y = 1;
	p->index = 0;
	p->hitPoints = 10;
	p->sprite = (Sprite *)malloc(sizeof(Sprite));

	LoadSprite(p->sprite, "data/galaga_ship.png");
	*/

/*
	p->index = 0;
	p->position.x = 3;
	p->position.y = 4;
	
	*/

	//Data->em.buffers[EntityType_Player].entities = 
	// 
	// Player* p = (Player*)Data->em.buffers[EntityType_Player].entities;
//
//Player *p;

//p->position.x = 8;
//LoadSprite(p->sprite, "data/player_guy.png");
//Data->em.buffers[EntityType_Player].entities = p;
	/*
	Player* p3 = (Player*)Data->em.buffers[EntityType_Player].entities;
	p3->index = 0;
	p3->position.x = 3;
	p3->position.y = 4;
	p3->hitPoints = 10;
	//p3->sprite = 
	*/
	//LoadSprite(&Data->sprite, "data/player_guy.png");
	//Data->em.buffers[EntityType_Player].entities = malloc(sizeof(Player) * 1);
	//p->position.y = 3;
	//LoadSprite(&Data->sprite, "data/galaga_ship.png");
	//OpenGL_InitTexture(&Data->sprite);
	//LoadSprite(p->sprite, "data/player_guy.png");
	//OpenGL_InitTexture(p3->sprite);
	//LoadSprite(playerBuffer->entities[playerBuffer->count], "data/player_guy.png");
	//Data->em
	/*
	em = (EntityManager*)Game->myData;
	memset(Game->myData, 0, sizeof(MyData));

	em->buffers[EntityType_Player].entities = (Player *)malloc(sizeof(Player) * 1);
	//memset(em->buffers, 0, sizeof(Player));
	
	p->index = AddEntity(EntityType_Player, em);
	//em->buffers[EntityType_Player].
	p = (Player*)em->buffers[EntityType_Player].entities;
	p->position.x = 8;
	
	//p2->size.x = 0.3f;
	//p2->size.y = 0.3f;

	*/
	//Player *p3 = Data->em.buffers[EntityType_Player].entities[]
	//Player* p3 = (Player*)Data->em.buffers[EntityType_Player].entities;
	//LoadSprite(p3->sprite, "data/player_guy.png");
	//Player* p = (Player*)GetEntity(EntityType_Player, 0);
	// LOGIC
    //	EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
		//Player *p3 = (Player*)&Data->em.buffers[EntityType_Player].entities;
		// 
	//Data = (MyData*)Game->myData;
	//Data->em.buffers[EntityType_Player];
	//EntityTypeBuffer* buffer = &em->buffers[info->type];

	//EntityTypeBuffer* playerBuffer1 = &Data->em.buffers[0];
	//Player* p4 = (Player *) playerBuffer1[0].entities;
	//Player *p3 = (Player*)&Data->em.b;
	//p3[0];
	
/*
	if (InputPressed(Input, Input_Up)) {
		p->position.y++;
	}
	if (InputPressed(Input, Input_Down)) {
		p->position.y--;
	}
	

	if (InputPressed(Input, Input_Right)) {
		p->position.x++;
	}
	
	if (InputPressed(Input, Input_Left)) {
		p->position.x--;
	}
	
	// RENDER
	//GetEntity(EntityType_Player, p.index);
	
	ClearColor(RGB(0.4f, 0.0f, 0.0f));
	DrawSprite(p->position, V2(0.3f, 0.3f), p->sprite);
}
*/


/*
void* GetEntity(EntityManager* em, EntityHandle handle) {
}
*/
/*
EntityInfo* info = &em->entities[handle.id];
//return info;
//
EntityTypeBuffer *buffer = &em->buffers[info->type];
return buffer->entities + (buffer->entitySizeInBytes * info->indexInBuffer);
// use the ID to look up the entity info, then use the info
// to look up our buffer and data
*/

/*
int32 AddEntity(EntityType type, void* data) {
	// Look up the buffer in type
	//if (type)
	EntityTypeBuffer* buffer = &Data->em.buffers[type];
	// count tells us where in the buffer we should add our data
	int32 currentCount = buffer->count;
	// copy data into buffer
	buffer[currentCount].entities = data;
	// increment count
	int32 index;
	index = buffer->count++;
	// return index to our data in the buffer
	index = buffer->count;
	return index;
}
*/

/*
void *GetEntity(EntityType type, int32 index) {
	EntityTypeBuffer* buffer = &Data->em.buffers[type];
	//buffer[ind]
	// look up the buffer using type then use the index into data
	//buffer[index];
	return buffer[index].entities;
}
*/
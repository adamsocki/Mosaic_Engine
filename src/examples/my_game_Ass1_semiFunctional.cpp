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
	EntityTypeBuffer buffers[EntityType_Player];
};

struct MyData {
	EntityManager em;
};

struct Player {
	int32 index;
	vec2 position;
	int32 hitPoints;
	Sprite* sprite;
};

MyData* Data = NULL;

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

void *GetEntity(EntityType type, int32 index) {
	EntityTypeBuffer* buffer = &Data->em.buffers[type];
	//buffer[ind]
	// look up the buffer using type then use the index into data 
	//buffer[index];
	return buffer[index].entities;
}

void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;

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
}


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




void MyGameUpdate() {
	Player* p = (Player*)GetEntity(EntityType_Player, 0);
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
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
	Sprite sprite;
};

struct Player {
	int32 index;
	//vec2 size;
	vec2 position;
	int32 hitPoints;
	Sprite* sprite;
};

MyData* Data = NULL;
EntityManager* em = NULL;

Player p;
//int32 playerCount = 1;

int32 AddEntity(EntityType type, EntityManager* data) {

	
	// Look up the buffer in type
	EntityTypeBuffer* buffer = &data->buffers[type];
	// count tells us where in the buffer we should add our data
	int32 currentCount = buffer->count;
	// copy data into buffer
	buffer->entities = data;
	// increment count
	int32 index;
	index = buffer->count++;
	
	// return index to our data in the buffer
	return index;
}


void GetEntity(EntityType type, int32 index) {
	
	EntityTypeBuffer* buffer = &em->buffers[type];
	// look up the buffer using type then use the index into data 
	buffer[index];
}


void MyInit() {
	Game->myData = malloc(sizeof(MyData));
	memset(Game->myData, 0, sizeof(MyData));

	Data = (MyData*)Game->myData;
	LoadSprite(&Data->sprite, "data/player_guy.png");

	em = (EntityManager*)Game->myData;
	
	em->buffers[EntityType_Player].entities = (Player *)malloc(sizeof(Player) * 1);
	memset(em->buffers, 0, sizeof(Player));

	
	p.index = AddEntity(EntityType_Player, em);
	p.index = AddEntity(EntityType_Player, em);
	//em->buffers[EntityType_Player].
	Player* p2 = (Player*)em->buffers[0].entities;
	p2->position.x = 3;
	//p2->size.x = 0.3f;
	//p2->size.y = 0.3f;
}

void MyGameUpdate() {
	// LOGIC
	Player* p3 = (Player*)em->buffers[0].entities;

	
	if (InputPressed(Input, Input_Up)) {
		p3->position.y++;
	}
	if (InputPressed(Input, Input_Down)) {
		p3->position.y--;
	}
	if (InputPressed(Input, Input_Right)) {
		p3->position.x++;
	}
	if (InputPressed(Input, Input_Left)) {
		p3->position.x--;
	}
	
	
	// RENDER
	//GetEntity(EntityType_Player, p.index);
	
	
	ClearColor(RGB(0.4f, 0.0f, 0.0f));
	DrawSprite(p3->position, V2(0.3f, 0.3f), &Data->sprite);
}



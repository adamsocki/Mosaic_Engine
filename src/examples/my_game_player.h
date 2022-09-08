

void PlayerInput(Player* p) {

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

};
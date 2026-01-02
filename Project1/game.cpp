#define is_down(b) input->buttons[b].is_down
#define is_pressed(b) (input->buttons[b].is_down&&input->buttons[b].changed)
#define is_released(b) (!input->buttons[b].is_down&&input->buttons[b].changed)




enum GAME_MODE {
	GM_MENU,
	GM_GAME,
	GM_PAUSE,
	GM_GAME_OVER,
	GM_SETTINGS,
};
GAME_MODE current_gamemode=GM_MENU;
int hot_button=0;
bool enemy_is_ai;


float player_1_pos = 0.f;
float player_2_pos = 0.f;
float arena_half_size_x = 0.85f, arena_half_size_y = 0.45f;
float player_half_size_x = 0.015f, player_half_size_y = 0.07f;
float speed = 0.5f; // Speed of player movement

float ball_pos_x = 0.f, ball_pos_y = 0.f, ball_speed_x=0.3f, ball_speed_y=0.f, speed_up_factor=1.07f;
int player_1_score = 0, player_2_score = 0;
int collision_count = 0;
int score_limit = 10;
int winner;

//float player_p = 0.f, player_dp = 0.f; 


void simulate_game(Input* input, float dt) {
	render_background();
	clear_screen(0xFFFFFFFF); // Clear the screen to white
	//drawing arena
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x0000ffff);

	if (current_gamemode==GM_GAME) {

		//drawing score boxes
		draw_numbers(player_1_score, -.6f, .25f, .02f, 0x00ff00ff);
		draw_numbers(player_2_score, .6f, .25f, .02f, 0x00ff00ff);

		draw_text("PLAYER ", -0.8f, 0.4f, .02, 0x000000);
		draw_numbers(1, -0.5f, .4f, .02f, 0x00ff00ff);
		draw_text("PLAYER ", 0.5f, 0.4f, .02, 0x000000);
		draw_numbers(1, 0.8f, .4f, .02f, 0x00ff00ff);

		//drawing hitboxes
		draw_rect(-.8, player_1_pos, player_half_size_x, player_half_size_y, 0x00ff6000);
		draw_rect(.8, player_2_pos, player_half_size_x, player_half_size_y, 0x00ff6000);

		//drawing ball
		draw_rect(ball_pos_x, ball_pos_y, 0.01, 0.01, 0xFF0000FF);

		//movement physics
		if (is_down(BUTTON_UP)) player_2_pos += speed * dt;
		if (is_down(BUTTON_DOWN)) player_2_pos -= speed * dt;

		if (!enemy_is_ai) {
			if (is_down(BUTTON_W)) player_1_pos += speed * dt;
			if (is_down(BUTTON_S)) player_1_pos -= speed * dt;
			//float player_ddp = 0.f;
		}
		else {
			if (ball_pos_y > player_1_pos) player_1_pos += speed * dt;
			if (ball_pos_y < player_1_pos) player_1_pos -= speed * dt;
		}

		//
		//if (is_down(BUTTON_UP)) player_ddp+=.20;
		//if (is_down(BUTTON_DOWN)) player_ddp-=.20;

		//player_ddp -= player_dp * 0.05f; // Damping
		//player_p = player_p + player_dp * dt + player_ddp * dt * dt / 2;
		//player_dp = player_dp + player_ddp * dt;


		//collision check
		if (player_1_pos + player_half_size_y > arena_half_size_y) player_1_pos = arena_half_size_y - player_half_size_y;
		if (player_1_pos - player_half_size_y < -arena_half_size_y) player_1_pos = -arena_half_size_y + player_half_size_y;
		if (player_2_pos + player_half_size_y > arena_half_size_y) player_2_pos = arena_half_size_y - player_half_size_y;
		if (player_2_pos - player_half_size_y < -arena_half_size_y) player_2_pos = -arena_half_size_y + player_half_size_y;

		ball_pos_x += ball_speed_x * dt;
		ball_pos_y += ball_speed_y * dt; 

		// Ball collision with players
		if (ball_pos_x >= .8 - player_half_size_x && ball_pos_y <= player_2_pos + player_half_size_y && ball_pos_y >= player_2_pos - player_half_size_y) {
			ball_speed_x = -ball_speed_x * speed_up_factor; // Reverse ball direction
			collision_count++;
			if (is_down(BUTTON_UP)) ball_speed_y += 0.1f; // Add some vertical speed when hitting the player
			else if (is_down(BUTTON_DOWN)) ball_speed_y -= 0.1f; // Add some vertical speed when hitting the player
		}
		if (ball_pos_x <= -.8 + player_half_size_x && ball_pos_y <= player_1_pos + player_half_size_y && ball_pos_y >= player_1_pos - player_half_size_y) {
			ball_speed_x = -ball_speed_x * speed_up_factor; // Reverse ball direction
			collision_count++;
			if (is_down(BUTTON_UP)) ball_speed_y += 0.1f; // Add some vertical speed when hitting the player
			else if (is_down(BUTTON_DOWN)) ball_speed_y -= 0.1f;
		}
		//vertical bound collision check
		if (ball_pos_y >= arena_half_size_y - 0.01f) {
			ball_speed_y = -ball_speed_y; // Reverse ball direction on top wall
			ball_pos_y = arena_half_size_y - 0.01f; // Prevent ball from going out of bounds
		}
		if (ball_pos_y <= -arena_half_size_y + 0.01f) {
			ball_speed_y = -ball_speed_y; // Reverse ball direction on bottom wall
			ball_pos_y = -arena_half_size_y + 0.01f; // Prevent ball from going out of bounds
		}
		//restarting game
		if (ball_pos_x + .01f > arena_half_size_x) {
			ball_pos_x = 0.f, ball_pos_y = 0.f, ball_speed_x = -ball_speed_x / pow(speed_up_factor, collision_count), ball_speed_y = 0.f;
			player_1_score++;
			collision_count = 0; // Reset collision count after scoring
		}
		else if (ball_pos_x - .01f < -arena_half_size_x) {
			ball_pos_x = 0.f, ball_pos_y = 0.f, ball_speed_x = -ball_speed_x / pow(speed_up_factor, collision_count), ball_speed_y = 0.f;
			player_2_score++;
			collision_count = 0;
		}


		//pausing the game
		if (is_pressed(BUTTON_ESCAPE)) {
			current_gamemode = GM_PAUSE;
		}

		if(player_1_score>= score_limit || player_2_score >= score_limit) {
			current_gamemode = GM_GAME_OVER;
			if (player_1_score > player_2_score) winner = 1;
			else winner = 2;
		}

	}
	else if (current_gamemode == GM_MENU) {
		draw_text("PING PONG", -0.49f, 0.3f, .02f, 0x00ffffff);

		if(is_pressed(BUTTON_LEFT)){
			hot_button=(hot_button+2)%3;
		}
		else if(is_pressed(BUTTON_RIGHT)){
			hot_button=(hot_button+1)%3;
		}
		if (is_pressed(BUTTON_ENTER)) {
			if (hot_button == 0 || hot_button == 1) {
				current_gamemode = GM_GAME;
				if (hot_button == 0) enemy_is_ai = true; // AI mode
				else enemy_is_ai = false; // Player vs Player mode
			}
			else {
				current_gamemode = GM_SETTINGS; // Settings mode

			}
		}

		if (hot_button == 0) {
			draw_rect(-0.4f, 0.0f, 0.1f, 0.1f, 0x00ff00ff); // Draw green button
			draw_rect(0.f, 0.0f, 0.1f, 0.1f, 0x00ff0000);
			draw_rect(0.4f, 0.0f, 0.1f, 0.1f, 0x00ff0000);
			draw_numbers(1, -0.4f, 0.0f, 0.03f, 0xFFFFFFFF); // Draw "1" on the green button
		}
		else if(hot_button==1) {
			draw_rect(-0.4f, 0.0f, 0.1f, 0.1f, 0x00ff0000);
			draw_rect(0.f, 0.0f, 0.1f, 0.1f, 0x00ff00ff);
			draw_rect(0.4f, 0.0f, 0.1f, 0.1f, 0x00ff0000);
			draw_numbers(2, 0.f, 0.0f, 0.03f, 0xFFFFFFFF); // Draw "2" on the red button
		}
		else if(hot_button==2) {
			draw_rect(-0.4f, 0.0f, 0.1f, 0.1f, 0x00ff0000);
			draw_rect(0.f, 0.0f, 0.1f, 0.1f, 0x00ff0000);
			draw_rect(0.4f, 0.0f, 0.1f, 0.1f, 0x00ff00ff);
			draw_numbers(3, 0.4f, 0.0f, 0.03f, 0xFFFFFFFF); // Draw "2" on the red button
		}

		//draw_numbers(9999, 0, 0, 0.05f, 0xFFFFFFFF);
	}

	else if (current_gamemode == GM_PAUSE) {
		draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x001f1f1f); // Draw a semi-transparent overlay
		if(is_pressed(BUTTON_ESCAPE)) {
			current_gamemode = GM_GAME;
		}
		draw_numbers(player_1_score, -.6f, .35f, .02f, 0x00ff00ff);
		draw_numbers(player_2_score, .6f, .35f, .02f, 0x00ff00ff);
		draw_rect(-.8, player_1_pos, player_half_size_x, player_half_size_y, 0x00ff6000);
		draw_rect(.8, player_2_pos, player_half_size_x, player_half_size_y, 0x00ff6000);
		draw_rect(ball_pos_x, ball_pos_y, 0.01, 0.01, 0xFF0000FF);
		draw_text("PAUSED", -0.3f, 0.1f, 0.02f, 0xFFFFFF);
	}

	else if (current_gamemode == GM_SETTINGS) {

		int digits = 1; 
		int temp_score_limit = score_limit;
		while (temp_score_limit / 10 > 0) {
			digits++;
			temp_score_limit /= 10;
		}
		draw_rect((1-digits)*0.1f, 0, (digits * 2 + 1) * 0.05f, 4 * 0.05f, 0x001f1f1f); // Draw a semi-transparent overlay for the settings menu
		/*if (score_limit < 10) {
			draw_rect(0, 0, 0.2f, 0.2f, 0x001f1f1f);
		}
		else {
			draw_rect(-.1f, 0, 5*0.05f, 0.2f, 0x001f1f1f);
		}*/
		/*else {
			if (score_limit == 21 || score_limit == 31 || score_limit == 41 || score_limit == 51 || score_limit == 61 || score_limit == 71 || score_limit == 81 || score_limit == 91) {
				draw_rect(-.0, 0, 0.2f, 0.2f, 0x001f1f1f);
			}
			draw_rect(-.06, 0, 0.2f, 0.2f, 0x001f1f1f);
		}*/
		//draw_rect(0, 0, 0.2f, 0.2f, 0x001f1f1f);

		draw_numbers(score_limit, 0, 0, 0.05f, 0xFFFFFFFF);
		if (is_pressed(BUTTON_UP)) {
			score_limit++;
		}
		else if (is_pressed(BUTTON_DOWN)) {
			score_limit--;
		}
		if(is_pressed(BUTTON_ESCAPE)) {
			current_gamemode = GM_MENU;
		}
	}

	else if(current_gamemode==GM_GAME_OVER) {
		//draw_rect(0, 0, arena_half_size_x, arena_half_size_x, 0x001f1f1f);
		draw_text("GAME OVER", -0.5f, 0.5f, 0.005f, 0xFFFFFF);
		draw_text("WINNER", -0.6f, 0.5f, 0.005f, 0xFFFFFF);
		draw_text("PLAYER", 0.4f, 0.5f, 0.005f, 0xFFFFFF);
		draw_numbers(winner, 0.5f, 0, 0.01f, 0x00ff00ff);
		if (is_pressed(BUTTON_ESCAPE)) {
			current_gamemode = GM_MENU;
			player_1_score = 0;
			player_2_score = 0;
			ball_pos_x = 0.f, ball_pos_y = 0.f, ball_speed_x = 0.3f, ball_speed_y = 0.f;
			collision_count = 0;
		}
	}

	
}
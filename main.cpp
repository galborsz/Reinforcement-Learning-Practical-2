#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <string>
#include <iomanip>
#include <fstream>

using namespace sf;
using namespace std;

//	Define parameters
const double GAMMA = 1;
const double ALPHA = 0.5;
const double EPSILON = 0.1;
const double N_EPISODES = 2000;
const int N_ACTIONS = 2;
const int N_STATES = 960000;
const int X_MAX = 800;
const int Y_MAX = 600;
const int LIFE_MAX = 2;

struct Pair {
     int value1;
     int value2;
};

struct State {
    int x_distance;
    int y_distance;
	int life;
	bool operator < (const State& state) const{
       return tie(x_distance, y_distance, life) < tie(state.x_distance, state.y_distance, state.life); 
    }
};

/*int num_range_first(){
	static int i = 0;
    return i++;
}

int num_range_second(){
	static int i = 0;
    return i++;
}*/

// Define states
State create_state(int xdif, int ydif, int life){
	State state;
	state.x_distance = xdif;
	state.y_distance = ydif;
	state.life = life;
    return state;
}
/*vector<int> X_DISTANCE(1000);
generate(X_DISTANCE.begin(), X_DISTANCE.end(), num_range_first);
vector<int> Y_DISTANCE(150);
generate(Y_DISTANCE.begin(), Y_DISTANCE.end(), num_range_second);
vector<int> LIFE = {0, 1};*/
//int N_STATES = X_DISTANCE.size() * Y_DISTANCE.size() * LIFE.size();

// Define actions
//vector<int> ACTIONS = {0, 1}; //where 0 represents "Do nothing" and 1 represents "Click"

map< State, map< int, double > >  initialize_map() {
	map< State, map< int, double > > m;
	for(int x=0; x<X_MAX; x++){
		for(int y=0; y<Y_MAX; y++){
			for(int l=0; l<LIFE_MAX; l++) {
				State state = create_state(x, y, l);
				for(int a=0; a<N_ACTIONS; a++) {
					m[state][a] = 0;
				}
			}
		}
	}
	
	return m;
}

// Define action-value table as a map (key string: state, key int: action)
map< State, map< int, double > > Q_TABLE = initialize_map();

void write_map(map< State, map< int, double > > m) {
	for(int x=0; x<X_MAX; x++){
		for(int y=0; y<Y_MAX; y++){
			for(int l=0; l<LIFE_MAX; l++) {
				State state = create_state(x, y, l);
				for(int a=0; a<N_ACTIONS; a++) {
					cout << "map: " << m[state][a] << endl;
				}
			}
		}
	}
}

void write(vector<int> vec) {
    for (double e : vec) {
        cout << " " << e;
    }
    cout << endl;
}

int argmax(vector<double> Q) {
    int imax = 0;
    double max = -1;
    for (int i=0; i<Q.size(); ++i) {
        if (Q[i] > max) {
            max = Q[i];
            imax = i;
    }   }
    return imax;
}

// ALGORITHMS

/* Performs greedy policy. With prob epsilon pick action
    belonging to maximum action-value. With prob 1-epsilon
    pick a random action. */
/*int greedy_policy(string state, vector<vector<double> > q_table) {
	int action = argmax(q_table[state]);
	cout << "action: " << action << endl;

	double random = (double)rand() / RAND_MAX;
	cout << "random: " << random << endl;
	if (random < EPSILON){
		action = rand() % N_ACTIONS;	
	}

	return action;
}*/

// Computes next state and reward
/*void next_position(string state, int action) {
	// x is the row number and y is the column number
	(ax, ay) = ACTIONS[action];

	row = max(0, x + ax + self.WIND_V[y])
	row = min(self.N_ROWS-1, row)
	col = max(0, y + ay + self.WIND_H[x])
	col = min(self.N_COLUMNS-1, col)
	
	if (row, col) in self.FORBIDEN_CELLS: # Si anem a una cel·la prohibida, ens quedem on estàvem
		next_state = (x, y)
	else: 
		next_state = (row, col)

	next_state = self.get_state(next_state)

	if next_state == self.FIN_STATE:
		return next_state, 0
	return next_state, -1

}*/

// Q-learning always pick next_action with maximum action-value (q-value)
/*Pair q_learning(string state, int action){
	// Choose A from S
	int next_action = greedy_policy(state);

	// Take action A, observe R, S'
	Pair next_position = next_position(state, next_action);
	int next_state = next_position->value1;
	int reward = next_position->value2;


	// max Q(S',a) for all a
	vector<double> q;
	for (int a : N_ACTIONS) {
		q.push_back(Q_TABLE[next_state][a]);
	}
	double maxQ = argmax(q)

	// Update Q(S,A)
	Q_TABLE[state][next_action] = Q_TABLE[state][next_action] + ALPHA * (reward + GAMMA * maxQ - Q_TABLE[state][next_action]);

	// return next state and next action
	Pair next = {next_state, next_action}

	return next;
}*/



// rect rect collision detection helper function
bool collides(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2) {
		return true;
	}
	return false;
}

int choose_action() {
	double r = (double)rand() / RAND_MAX;
    double p = 0.5;
    if (r <= p) return 1;
    return 0;
}

int main() {

	/*Pair xy = get_XY("12_13_0");
	cout << "value x: " << xy.value1 << endl;
	cout << "value y: " << xy.value2 << endl;*/

	// create the window and set general settings. Plant the seeds
	RenderWindow window(VideoMode(1000, 600), "Floppy Bird");
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(false);

	// all sounds and their buffers will rest in this singleton struct
	struct Sounds {
		SoundBuffer chingBuffer;
		SoundBuffer hopBuffer;
		SoundBuffer dishkBuffer;
		Sound ching;
		Sound hop;
		Sound dishk;
	} sounds;

	// load sounds
	sounds.chingBuffer.loadFromFile("./audio/score.wav");
	sounds.hopBuffer.loadFromFile("./audio/flap.wav");
	sounds.dishkBuffer.loadFromFile("./audio/crash.wav");
	sounds.ching.setBuffer(sounds.chingBuffer);
	sounds.hop.setBuffer(sounds.hopBuffer);
	sounds.dishk.setBuffer(sounds.dishkBuffer);

	// all textures remain in here. Flappy has 3 textures, which will repeadetly draw, creating the illusion of flying.
	struct Textures {
		Texture flappy[3];
		Texture pipe;
		Texture background;
		Texture gameover;
	} textures;

	// load textures
	textures.background.loadFromFile("./images/background.png");
	textures.pipe.loadFromFile("./images/pipe.png");
	textures.gameover.loadFromFile("./images/gameover.png");
	textures.flappy[0].loadFromFile("./images/flappy1.png");
	textures.flappy[1].loadFromFile("./images/flappy2.png");
	textures.flappy[2].loadFromFile("./images/flappy3.png");

	// flappy singleton struct.
	// v = vertical speed
	// frame = which texture to print
	struct Flappy {
		double v = 0;
		int frame = 0;
		Sprite sprite;
	} flappy;

	// initial position, scale
	flappy.sprite.setPosition(250, 300);
	flappy.sprite.setScale(2, 2);

	// resizable array of sprites representing pipes
	vector<Sprite> pipes;

	// waiting = game not started yet
	// started = playing
	// gameover = game is over
	enum GameState { waiting,
					 started,
					 gameover };


	// game settings in singleton game struct
	// frames counts total frames passed since the beginning of time
	struct Game {
		int score = 0;
		int highscore = 0;
		int frames = 0;
		GameState gameState = waiting;
		Sprite background[3];
		Sprite gameover;
		Text pressC;
		Text scoreText;
		Text highscoreText;
		Font font;
	} game;

	// load font, set positions, scales etc
	game.font.loadFromFile("./fonts/flappy.ttf");
	game.background[0].setTexture(textures.background);
	game.background[1].setTexture(textures.background);
	game.background[2].setTexture(textures.background);
	game.background[0].setScale(1.15625, 1.171875);
	game.background[1].setScale(1.15625, 1.171875);
	game.background[2].setScale(1.15625, 1.171875);
	game.background[1].setPosition(333, 0);
	game.background[2].setPosition(666, 0);
	game.gameover.setTexture(textures.gameover);
	game.gameover.setOrigin(192 / 2, 42 / 2);
	game.gameover.setPosition(500, 125);
	game.gameover.setScale(2, 2);
	game.pressC.setString("Press C to continue");
	game.pressC.setFont(game.font);
	game.pressC.setFillColor(Color::White);
	game.pressC.setCharacterSize(50);
	game.pressC.setOrigin(game.pressC.getLocalBounds().width / 2, 0);
	game.pressC.setPosition(500, 250);
	game.scoreText.setFont(game.font);
	game.scoreText.setFillColor(Color::White);
	game.scoreText.setCharacterSize(75);
	game.scoreText.move(30, 0);
	game.highscoreText.setFont(game.font);
	game.highscoreText.setFillColor(Color::White);
	game.highscoreText.move(30, 80);

	// Initial state
	State current_state = create_state(flappy.sprite.getPosition().x, flappy.sprite.getPosition().y, 1);
	
	// main loop
	while (window.isOpen()) {

		// Set random seed 
		srand(100);

		// update score
		flappy.sprite.setTexture(textures.flappy[1]);
		game.scoreText.setString(to_string(game.score));
		game.highscoreText.setString("HI " + to_string(game.highscore));

		// update flappy
		float fx = flappy.sprite.getPosition().x;
		float fy = flappy.sprite.getPosition().y;
		float fw = 34 * flappy.sprite.getScale().x;
		float fh = 24 * flappy.sprite.getScale().y;

		// DETERMINE THE ACTION, a, IN STATE, s, BASED ON Q MATRIX

		// int a = greedy_policy(state, Q_TABLE); 

		// TAKE THE ACTION, a, AND OBSERVE THE OUTCOME STATE, s, AND REWARD, r

		// flap the wings if playing
		if (game.gameState == waiting || game.gameState == started) {

			// change the texture once in 6 frames
			if (game.frames % 6 == 0) {
				flappy.frame += 1;
			}
			if (flappy.frame == 3) {
				flappy.frame = 0;
			}
		}

		flappy.sprite.setTexture(textures.flappy[flappy.frame]);

		// move flappy
		if (game.gameState == started) {
			flappy.sprite.move(0, flappy.v);
			flappy.v += 0.5;
		}

		// HERE UPDATE Q VALUES
		// if hits ceiling, stop ascending
		// if out of screen, game over
		if (game.gameState == started) {
			if (fy < 0) {
				flappy.sprite.setPosition(250, 0);
				flappy.v = 0;
			} else if (fy > 600) {
				flappy.v = 0;
				game.gameState = gameover;
				sounds.dishk.play();
			}
		}

		// count the score
		for (vector<Sprite>::iterator itr = pipes.begin(); itr != pipes.end(); itr++) {
			if (game.gameState == started && (*itr).getPosition().x == 250) {
				game.score++;
				sounds.ching.play();

				if (game.score > game.highscore) {
					game.highscore = game.score;
				}

				break;
			}
		}

		// generate pipes (tubes)
		if (game.gameState == started && game.frames % 150 == 0) {
			int random = rand();
			int r = random % 275 + 75;
			int gap = 150;

			// lower pipe
			Sprite pipeL;
			pipeL.setTexture(textures.pipe);
			pipeL.setPosition(800, r + gap);
			pipeL.setScale(2, 2);

			// upper pipe
			Sprite pipeU;
			pipeU.setTexture(textures.pipe);
			pipeU.setPosition(800, r);
			pipeU.setScale(2, -2);

			// push to the array
			pipes.push_back(pipeL);
			pipes.push_back(pipeU);
		}

		// move pipes
		if (game.gameState == started) {
			for (vector<Sprite>::iterator itr = pipes.begin(); itr != pipes.end(); itr++) {
				(*itr).move(-3, 0);
			}
		}

		// remove pipes if offscreen
		if (game.frames % 100 == 0) {
			vector<Sprite>::iterator startitr = pipes.begin();
			vector<Sprite>::iterator enditr = pipes.begin();

			for (; enditr != pipes.end(); enditr++) {
				if ((*enditr).getPosition().x > -104) {
					break;
				}
			}

			pipes.erase(startitr, enditr);
		}

		// HERE UPDATE Q VALUES
		// collision detection
		if (game.gameState == started) {
			for (vector<Sprite>::iterator itr = pipes.begin(); itr != pipes.end(); itr++) {

				float px, py, pw, ph;

				if ((*itr).getScale().y > 0) {
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y;
					pw = 52 * (*itr).getScale().x;
					ph = 320 * (*itr).getScale().y;
				} else {
					pw = 52 * (*itr).getScale().x;
					ph = -320 * (*itr).getScale().y;
					px = (*itr).getPosition().x;
					py = (*itr).getPosition().y - ph;
				}

				if (collides(fx, fy, fw, fh, px, py, pw, ph)) {
					game.gameState = gameover;
					sounds.dishk.play();
				}
			}
		}

		// events
		Event event;
		while (window.pollEvent(event)) {

			// bye bye
			if (event.type == Event::Closed) {
				window.close();
			}
			
			// flap
			else if (event.type == Event::KeyPressed &&
					   event.key.code == Keyboard::Space) {
				if (game.gameState == waiting) {
					game.gameState = started;
				}

				if (game.gameState == started) {
					flappy.v = -8;
					sounds.hop.play();
				}

			// restart
			} else if (event.type == Event::KeyPressed &&
					   event.key.code == Keyboard::C &&
					   game.gameState == gameover) {
				game.gameState = waiting;
				flappy.sprite.setPosition(250, 300);
				game.score = 0;
				pipes.clear();
			}
		}

		// clear, draw, display
		window.clear();
		window.draw(game.background[0]);
		window.draw(game.background[1]);
		window.draw(game.background[2]);
		window.draw(flappy.sprite);


		// draw pipes
		for (vector<Sprite>::iterator itr = pipes.begin(); itr != pipes.end(); itr++) {
			window.draw(*itr);
		}

		// draw scores
		window.draw(game.scoreText);
		window.draw(game.highscoreText);

		// gameover. press c to continue
		if (game.gameState == gameover) {
			window.draw(game.gameover);

			if (game.frames % 60 < 30) {
				window.draw(game.pressC);
			}
		}
		window.display();

		// dont forget to update total frames
		game.frames++;
	}

	return 0;
}

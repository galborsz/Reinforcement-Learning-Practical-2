
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <string>
#include <iomanip>
#include <fstream>

#include "sarsa_agent.hpp"

using namespace sf;
using namespace std;

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

//https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
void printProgress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

void saveData(vector<double> data, string fileName){
    // Create and open a text file
    ofstream MyFile(fileName);
    ofstream myfile;
    myfile.open(fileName);
    for(int i=0; i<data.size(); i++){
        myfile << data[i] << endl;
    }
    // Close the file
    MyFile.close();
}


// all textures remain in here. Flappy has 3 textures, which will repeadetly draw, creating the illusion of flying.
struct Textures {
	Texture flappy[3];
	Texture pipe;
	Texture background;
	Texture gameover;
} textures;

// all sounds and their buffers will rest in this singleton struct
struct Sounds {
	SoundBuffer chingBuffer;
	SoundBuffer hopBuffer;
	SoundBuffer dishkBuffer;
	Sound ching;
	Sound hop;
	Sound dishk;
} sounds;

// flappy singleton struct.
// v = vertical speed
// frame = which texture to print
struct Flappy {
	double v = 0;
	int frame = 0;
	Sprite sprite;
} flappy;

// resizable array of sprites representing pipes
vector<Sprite> pipes;

// waiting = game not started yet
// started = playing
// gameover = game is over
enum GameState { started,
				 gameover };


// game settings in singleton game struct
// frames counts total frames passed since the beginning of time
struct Game {
	int score = 0;
	int highscore = 0;
	int frames = 0;
	GameState gameState = started;
	Sprite background[3];
	Sprite gameover;
	Text pressC;
	Text scoreText;
	Text highscoreText;
	Font font;
} game;

// rect rect collision detection helper function
bool collides(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2) {
		return true;
	}
	return false;
}

 void calculate_state_xy(int &xdif, int &ydif){
	int xpos = flappy.sprite.getPosition().x;
	int ypos = flappy.sprite.getPosition().y;

	float hpipe, xpipe, ypipe;
	if (pipes.empty()){
		//xdif = 10000;
		//ydif = 10000;
		cout << "pipes is empty" << endl;
		return;
	}
	//std::cout << "/* cal */" << '\n';
	for (vector<Sprite>::iterator itr = pipes.begin(); itr != pipes.end(); itr++) {
		ypipe = (*itr).getPosition().y;
		xpipe = (*itr).getPosition().x + 52 * (*itr).getScale().x; //EDIT
		if (xpipe > xpos){
			break;
		}
	}

	xdif = xpipe - xpos;
	ydif = ypipe - ypos;
}

int main() {

	// create the window and set general settings. Plant the seeds
	RenderWindow window(VideoMode(1000, 600), "Flappy Bird");
	window.setFramerateLimit(100);
	window.setKeyRepeatEnabled(false);
	srand(time(0));

	// load sounds
	sounds.chingBuffer.loadFromFile("./audio/score.wav");
	sounds.hopBuffer.loadFromFile("./audio/flap.wav");
	sounds.dishkBuffer.loadFromFile("./audio/crash.wav");
	sounds.ching.setBuffer(sounds.chingBuffer);
	sounds.hop.setBuffer(sounds.hopBuffer);
	sounds.dishk.setBuffer(sounds.dishkBuffer);

	// load textures
	textures.background.loadFromFile("./images/background.png");
	textures.pipe.loadFromFile("./images/pipe.png");
	textures.gameover.loadFromFile("./images/gameover.png");
	textures.flappy[0].loadFromFile("./images/flappy1.png");
	textures.flappy[1].loadFromFile("./images/flappy2.png");
	textures.flappy[2].loadFromFile("./images/flappy3.png");


	// initial position, scale
	flappy.sprite.setPosition(250, 300);
	flappy.sprite.setScale(2, 2);

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

	//INITIALIZE LEARNING BOT
	sarsa_agent *agent = new sarsa_agent();

	//declaring needed variables
	int xdif, ydif;
	double reward;

	//show window
	int iteration = 0;
	int iteration_limit = 26000;
	bool disp = false;
	bool greedy = true;
    bool start = true;
    vector<double> total_score (iteration_limit);

	// main loop
	while (iteration != iteration_limit) { //window.isOpen()

		if (iteration == iteration_limit-1) {
			greedy = true;
			sounds.ching.play();
		}

		if (game.gameState == gameover) {
            total_score.push_back(game.score);
			game.gameState = started;
            start = true;
			flappy.sprite.setPosition(250, 300);
			game.frames = 0; //EDIT
			game.score = 0;
			pipes.clear();
			iteration++;
			double percentage = ((double)iteration/(double)iteration_limit);
			printProgress(percentage);
		}


		// update score
		flappy.sprite.setTexture(textures.flappy[1]);
		game.scoreText.setString(to_string(game.score));
		game.highscoreText.setString("HI " + to_string(game.highscore));

		// flap the wings if playing
		if (game.gameState == started) {
			// change the texture once in 6 frames
			if (game.frames % 6 == 0) {
				flappy.frame += 1;
			}
			if (flappy.frame == 3) {
				flappy.frame = 0;
			}
		}

		flappy.sprite.setTexture(textures.flappy[flappy.frame]);


		// count the score
		for (vector<Sprite>::iterator itr = pipes.begin(); itr != pipes.end(); itr++) {
			if (game.gameState == started && (*itr).getPosition().x == 250) {
				game.score++;
				//sounds.ching.play();

				if (game.score > game.highscore) {
					game.highscore = game.score;
					cout << "Iteration: " << iteration << ", Score: " << to_string(game.score) << ", HighScore: " << to_string(game.highscore) << endl;
				}
				break;
			}
		}

		// generate pipes
		if (game.gameState == started && game.frames % 150 == 0) { //EDIT so that pipes generate right away
			int r = rand() % 275 + 75;
			//r = 300;
			int gap = 150;

			// lower pipe
			Sprite pipeL;
			pipeL.setTexture(textures.pipe);
			pipeL.setPosition(1000, r + gap);
			pipeL.setScale(2, 2);

			// upper pipe
			Sprite pipeU;
			pipeU.setTexture(textures.pipe);
			pipeU.setPosition(1000, r);
			pipeU.setScale(2, -2);

			// push to the array
			pipes.push_back(pipeL);
			pipes.push_back(pipeU);
		}

        if (start){
            //Initialize S
            calculate_state_xy(xdif, ydif);
            //Choose A from S using policy derived from Q (e.g. e-greedy)
            if(greedy) agent->choose_action_greedy(xdif, ydif, flappy.v);
            else agent->choose_action(xdif, ydif, flappy.v);
            start = false;
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

///////////////////////////////////////////////////////////////////////////////////////////////

		//move flappy according to agent policy
		//Take action A
		if (game.gameState == started) {
			if (agent->act()) {
				flappy.v = -8;
				//sounds.hop.play();
			}
			flappy.sprite.move(0, flappy.v);
			flappy.v += 0.5;
		}

		// update flappy
		float fx = flappy.sprite.getPosition().x;
		float fy = flappy.sprite.getPosition().y;

		// if hits ceiling, stop ascending
		// if out of screen, game over
		if (game.gameState == started) {
			if (fy < 0) {
				flappy.sprite.setPosition(250, 0);
				flappy.v = 0;
				game.gameState = gameover; //EDIT
			} else if (fy > 600) {
				flappy.v = 0;
				game.gameState = gameover;
				//sounds.dishk.play();
			}
		}

		fx = flappy.sprite.getPosition().x;
		fy = flappy.sprite.getPosition().y;
		float fw = 34 * flappy.sprite.getScale().x;
		float fh = 24 * flappy.sprite.getScale().y;


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

				if (collides(fx, fy, fw, fh, px, py, pw, ph)) { //EDIT
					game.gameState = gameover;
					//sounds.dishk.play();

				}
			}
		}

		// gameover. restarts game immediately //EDIT
		//Observe R after taking action A
		if (game.gameState == gameover) {
			reward = -1000;
		} else {
			reward = 0;
		}

		//recalculating xdif and ydif
		//Observe S' after taking action A
		calculate_state_xy(xdif, ydif);

		//update qlearning_agent
		agent->update_qtable_sarsa(xdif, ydif, flappy.v, reward, greedy);

		// events
		Event event;
		while (window.pollEvent(event)) {
			// bye bye
			if (event.type == Event::Closed) {
				window.close();
			} else if (event.type == Event::KeyPressed &&
					   event.key.code == Keyboard::D) {
				disp = true;
			} else if (event.type == Event::KeyPressed &&
					   event.key.code == Keyboard::F) {
				 disp = false;
			} else if (event.type == Event::KeyPressed &&
 					   event.key.code == Keyboard::A) {
 				 greedy = true;
				 std::cout << "greedy" << '\n';
			} else if (event.type == Event::KeyPressed &&
 					   event.key.code == Keyboard::S) {
 				 greedy = false;
				 std::cout << "explore" << '\n';
			 } else if (event.type == Event::KeyPressed &&
  					   event.key.code == Keyboard::P) {
 				 std::cout << "xdif" << xdif << "ydif" << ydif << '\n';
  			}
		}

///////////////////////////////////////////////////////////////////////////////////////////////

		if (disp) {
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

			window.display();
		}

		// dont forget to update total frames
		game.frames++;

	}

	agent->print_count();
    saveData(total_score, "total_score_sarsa.txt");

	delete agent;

	return 0;
}

// Badger
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define AUTO_CHOOSE -1

#ifdef _WIN32
	#include <windows.h>

	void sleep(float seconds){
		Sleep(seconds);
	}
	void clear(){
		std::system("cls");
	}
#else
	#include <chrono>
	#include <thread>

	void sleep(float seconds){
		std::this_thread::sleep_for(std::chrono::milliseconds((unsigned)(seconds*1000)));
	}
	void clear(){
		std::system("clear");
	}
#endif

template<typename T>size_t getLength(const T* arr){
	size_t length;
	for(T a:arr){length++;}
	return length;
}
size_t getLength(const std::string* arr){
	size_t a=0;
	while(!arr[a].empty())++a;
	return a;
}

bool isNum(const char c){
	return c>='0'&&c<='9';
}

class ASCIIAnimator {
public:
	void animateText(const std::vector<std::string>& textArray, float delay=.5) {
		for (const std::string& text : textArray) {
			std::cout << text << std::endl;
			sleep(delay);
		}
	}
	void slowType(std::string msg,float delay=.05){
		for(char c:msg){
			std::cout<<c<<std::flush;
			sleep(delay);
		}
	}
	void showFile(std::string file_name,float delay=.01){
		std::ifstream file(file_name);
		std::string line;
		while(std::getline(file,line)){
			std::cout<<line<<std::endl;
			sleep(delay);
		}
		file.close();
	}
	// FIXME: make loop forever, but also print other text on screen.
	void showArt(std::string files[],float delay=AUTO_CHOOSE){
		float totalAnimateTime=1;
		unsigned int numFiles=getLength(files);
		if(delay==AUTO_CHOOSE){delay=totalAnimateTime/numFiles;}
		for(int a=0;a<numFiles;a++){
			showFile(files[a]);
		}
	}
};

class DialogueOption {
public:
	DialogueOption(const std::string& text, int optionNumber) : text(text), optionNumber(optionNumber) {}

	const std::string& getText() const {
		return text;
	}

	int getOptionNumber() const {
		return optionNumber;
	}

private:
	std::string text;
	int optionNumber;
};

class Dialogue {
public:
	Dialogue(const std::vector<std::string>& textArray) : textArray(textArray) {}

	void display(ASCIIAnimator& animator, float delay = .5) {
		animator.animateText(textArray, delay);
	}

	void addOption(const std::string& text) {
		options.emplace_back(text, options.size() + 1);
	}

	const std::vector<DialogueOption>& getOptions() const {
		return options;
	}

private:
	std::vector<std::string> textArray;
	std::vector<DialogueOption> options;
};

class Game {
public:
	void start() {
		ASCIIAnimator animator;

		std::vector<Dialogue> dialogues = readDialoguesFromFile("dialogue.txt");

		// Display dialogues and handle choices
		int selectedOption;
		for (Dialogue dialogue : dialogues) {
			dialogue.display(animator);
			selectedOption = getPlayerChoice(dialogue.getOptions());
			handleChoice(selectedOption, dialogue);
		}
	}

private:
	std::vector<Dialogue> readDialoguesFromFile(const std::string& filename) {
		std::ifstream file(filename);
		std::vector<Dialogue> dialogues;

		if (file.is_open()) {
			std::vector<std::string> currentText;
			Dialogue currentDialogue({}); // Initialize with an empty vector

			std::string line;
			while (std::getline(file, line)) {
				if (line.empty()) {
					if (!currentText.empty()) {
						currentDialogue = Dialogue(currentText);
						dialogues.push_back(currentDialogue);
						currentText.clear();
					}
				} else {
					if (isNum(line[0]) && line[1]+line[2]=='.'+' ') {
						// This line is an option
						currentText.clear();
						currentDialogue.addOption(line.substr(line.find('.')+2)); // Add the line as an option
					} else {
						// This line is part of the dialogue text
						currentText.push_back(line);
						currentDialogue = Dialogue(currentText);
					}
				}
			}

			dialogues.push_back(currentDialogue);

			file.close();
		} else {
			std::cerr << "Error: Unable to open dialogue file: " << filename << std::endl;
		}

		return dialogues;
	}


	int getPlayerChoice(const std::vector<DialogueOption>& options) {
		int selectedOption = -1;

		// Display options
		// FIXME: options shouldn't show w/o this, but they do, so options treated as dialogue rn.
		for (const DialogueOption& option : options) {
			std::cout << option.getOptionNumber() << ". " << option.getText() << std::endl;
		}

		// Get user input
		while (selectedOption < 1 || selectedOption > static_cast<int>(options.size())) {
			std::cout << "Enter the number of your choice: ";
			std::cin >> selectedOption;
		}

		return selectedOption;
	}

	void handleChoice(int selectedOption, Dialogue& dialogue) {
		const std::vector<DialogueOption>& options = dialogue.getOptions();
		if (selectedOption >= 1 && selectedOption <= static_cast<int>(options.size())) {
			// Handle player's choice here based on the selected option
			// You can implement the game logic and progression accordingly
			// For simplicity, we'll just display the selected option's text
			std::cout << "You chose option " << options[selectedOption - 1].getOptionNumber() << ": " << options[selectedOption - 1].getText() << std::endl;
		}
	}
};

int main() {
	Game game;
	game.start();

	return 0;
}
// FIXME: read file not working since it passing in no options from file.
// TODO: slow typing for individual characters.
// TODO: make look better by centering text & add colors.
// TODO: put text in table and on top put the path name in accordance to the .1 file.
// TODO: take away <1> and other numbers(more for debugging)
// TODO: make game w/ UI using C# .NET framework & AI generated art for different path options
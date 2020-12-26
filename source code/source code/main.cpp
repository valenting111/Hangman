#include <iostream>
#include <string>
#include <fstream>
#include <time.h>


void Clear()
{
#if defined _WIN32
	system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
	system("clear");
#elif defined (__APPLE__)
	system("clear");
#endif
}




std::fstream& GotoLine(std::fstream& file, unsigned int num) {
	file.seekg(std::ios::beg);
	for (int i = 0; i < num - 1; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return file;
}

class Hangman
{
private:
	unsigned int n_tries_left = 6;
	bool has_won = false;
	std::string secret_word;
	std::string display;
	std::string current_guess;
	std::string entered_valid_chars = "";
	std::string entered_invalid_chars = "";

public:
	std::string& get_Secret_Word();
	void set_Secret_Word(std::string word);
	std::string& get_Display();
	void update_Display();
	unsigned int& get_n_tries_left();
	void set_n_tries_left(unsigned int n);
	bool& get_victory_status();
	void set_victory_status(bool status);
	void add_successful_letter(std::string& letter);
	void add_wrong_letter(std::string& letter);
	std::string get_correct_letters_guessed();
	std::string get_incorrect_letters_guessed();
	void update_current_guess();
	void check_victory();

};


std::string& Hangman::get_Secret_Word()
{
	return this->secret_word;
}

void Hangman::set_Secret_Word(std::string word)
{
	this->secret_word = word;
	return;
}

std::string& Hangman::get_Display()
{
	return this->display;
}

void Hangman::update_Display()
{
	std::string basic_gui = "** HANGMAN ** \n * * * * * \n . . . . . \n  . . . . . \n * * * * *";
	this->display = basic_gui.insert(40, this->current_guess);
}

unsigned int& Hangman::get_n_tries_left()
{
	return this->n_tries_left;
}

void Hangman::set_n_tries_left(unsigned int n)
{
	this->n_tries_left = n;
}

bool& Hangman::get_victory_status()
{
	return this->has_won;
}

void Hangman::set_victory_status(bool status)
{
	this->has_won = status;
}

void Hangman::add_successful_letter(std::string& letter)
{
	this->entered_valid_chars.append(letter);
}

void Hangman::add_wrong_letter(std::string& letter)
{
	this->entered_invalid_chars.append(letter);
}

std::string Hangman::get_correct_letters_guessed()
{
	return this->entered_valid_chars;
}

std::string Hangman::get_incorrect_letters_guessed()
{
	return this->entered_invalid_chars;
}

void Hangman::update_current_guess()
{
	std::string temp = "";
	std::string::iterator it;
	for (it = this->secret_word.begin(); it != this->secret_word.end(); it++)
	{
		if (this->entered_valid_chars.find(*it) != std::string::npos)
		{
			temp.push_back(*it);
		}
		else
		{
			temp.push_back('_');
		}
	}
	temp.append("\n");
	this->current_guess = temp;
}

void Hangman::check_victory()
{
	if (this->current_guess.find('_') == std::string::npos)
	{
		this->set_victory_status(true);
	}
}


std::string & input_letter_guess(std::string& input, Hangman& hangm)
{
	try 
	{
		std::getline(std::cin, input);
		if (input.size() > 1)
		{
			throw 1;
		}
		else if (hangm.get_correct_letters_guessed().find(input) != std::string::npos || hangm.get_incorrect_letters_guessed().find(input) != std::string::npos)
		{
			throw 2;
		}
		else
		{
			return input;
		}
	}
	catch (int e)
	{
		if (e == 1)
		{
			std::cout << "You have entered more than one character, please try again" << std::endl;
		}
		if (e == 2)
		{
			std::cout << "You have already entered character : " << input << ", please enter a new character" << std::endl;
		}
		input_letter_guess(input, hangm);
	}
	catch (...)
	{
		std::cout << "Something went wrong, please enter a single valid character" << std::endl;
		input_letter_guess(input, hangm);
	}
}



int main()
{

	std::string rand_word;
	unsigned int total_n_words = 370103;

	try
	{
		std::fstream english_words("./words_alpha.txt", std::ios::in);
		srand(time(nullptr));
		unsigned int rand_num = rand() % total_n_words;
		//std::cout << rand_num << std::endl;

		GotoLine(english_words, rand_num);

		
		english_words >> rand_word;
		//std::cout << rand_word << std::endl;
		english_words.close();
	}
	catch(...)
	{
		std::cout << "Something went wrong when generating a random word" << std::endl;
	}
	

	Hangman my_hangman;
	my_hangman.set_Secret_Word(rand_word);
	my_hangman.update_current_guess();
	my_hangman.update_Display();
	std::cout << std::endl << std::endl << my_hangman.get_Display() << std::endl;

	std::string letter_guess;
	std::cout << "Enter a single character" << std::endl;

	while (!my_hangman.get_victory_status() && my_hangman.get_n_tries_left() != 0)
	{
		input_letter_guess(letter_guess, my_hangman);
		

		//If the letter guessed is inside the secret word
		if (my_hangman.get_Secret_Word().find(letter_guess) != std::string::npos)
		{
			my_hangman.add_successful_letter(letter_guess);
			my_hangman.update_current_guess();
			my_hangman.update_Display();
			my_hangman.check_victory();
		}

		//If the letter guessed isn't inside the secret word
		else
		{

			my_hangman.add_wrong_letter(letter_guess);
			my_hangman.set_n_tries_left(my_hangman.get_n_tries_left() - 1);

		}
		
		Clear();

		std::cout << "letter entered: " << letter_guess << std::endl;
		std::cout << std::endl << my_hangman.get_Display() << std::endl;
		
		std::cout << "Number of tries left: " << my_hangman.get_n_tries_left() << std::endl;
	}

	if (my_hangman.get_victory_status())
	{
		std::cout << "Congrats, you won!" << std::endl;
	}
	else 
	{
		std::cout << "You lost, the word to guess was " << my_hangman.get_Secret_Word() << ", maybe next time!" << std::endl;
	}



	return 0;
}
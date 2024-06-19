import <print>;
import <cstdint>;
import <array>;
import <algorithm>;
import <random>;
import <string_view>;
import <ranges>;
import <iostream>;
import <exception>;
import <cstring>;

std::random_device rdev;
std::mt19937 reng(rdev());
std::array<uint8_t, 16> board;

void init()
{
	std::iota(board.begin(), board.end(), 0);
	std::shuffle(board.begin(), board.end(), reng);
}

bool check()
{
	for (int i = 0; i < board.size()-1; i++) {
		if (board[i] != i+1)
			return false;
	}
	return true;
}

void draw()
{
	const std::string_view horizontal_separator {"+----"};
	auto draw_separator = [=]() {
		for (int index : std::views::iota(0, 4)) {
			std::print("{}", horizontal_separator);
		}
		std::println("+");
	};

	for (int index : std::views::iota(0, (int)board.size()))
	{
		if (index == 0) {
			draw_separator();
		}

		if (board[index] == 0)
			std::print("|    ");
		else
			std::print("| {:>2} ", board[index]);

		if ((index + 1) % 4 == 0) {
			std::println("|");
			draw_separator();
		}
	}
}

bool input()
{
	const auto it_zero = std::find(board.begin(), board.end(), 0);
	const int zero_index = it_zero - board.begin();

	int index[4] {-1, -1, -1, -1}; // left, right, top, bottom
	if (zero_index % 4 != 0)
		index[0] = zero_index - 1;
	if ((zero_index + 1) % 4 != 0)
		index[1] = zero_index + 1;
	if (zero_index - 4 < 16)
		index[2] = zero_index - 4;
	if (zero_index + 4 >= 0)
		index[3] = zero_index + 4;

	int moves[4] {};
	
	std::print("Move? (");
	bool last_comma = true;
	for (int z = 0; z < 4; z++) {
		const auto i = index[z];
		if (i != -1) {
			const auto n = *std::next(board.begin(), i);
			if (n == 0) continue;

			if (!last_comma) {
				std::print(", ");
				last_comma = true;
			}
			std::print("{}", *std::next(board.begin(), i));
			last_comma = false;
		}
	}
	std::print("): ");

	unsigned move;
	std::cin >> move;
    if (move == 0)
		return true;

	int chosen_index = -1;
	for (int i = 0; i < 4; i++) {
		if (move == board[index[i]]) {
			chosen_index = i;
			break;
		}
	}

	if (chosen_index == -1 || !std::cin)
		std::println("Not a valid move");
	else
		std::swap(board[zero_index], board[index[chosen_index]]);

	std::println("");
	return false;
}

void run()
{
	bool quit = false;
	while (!quit)
	{
		if (check()) {
			std::println("You won!");
			return;
		}

		draw();
		quit = input();
	}
}

int main(int argc, char** argv)
{
	try
	{
		if (argc > 16) {
			memset(&board[0], 0xff, board.size() * 1);
			for (int i = 0; i < 16; i++) {
				const auto elem = std::stoi(argv[i+1]);
				if (elem < 0 || elem > 15)
					throw std::out_of_range(std::format("Out of range board element for index {}", i));

				decltype(board)::pointer search = nullptr;
				std::for_each(board.begin(), board.begin() + i, [&](auto& e) {
					if (e == elem) search = &e;
				});
				if (search != nullptr)
					throw std::runtime_error(std::format("{} at index {} has already been utilized", elem, search - &board[0]));

				board[i] = elem;
			}
		} else if (argc != 1) {
			throw std::runtime_error(std::format("Supply the entire board. Remaining {}", 16 - (argc-1)));
		}
		else {
			init();
		}
		run();
	}
	catch (std::exception& e)
	{
		std::println(stderr, "std::exception: {}", e.what());
	}
}

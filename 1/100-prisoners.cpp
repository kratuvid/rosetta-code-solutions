// BROKEN

import <algorithm>;
import <array>;
import <iomanip>;
import <iostream>;
import <numeric>;
import <print>;
import <random>;
import <ranges>;
using namespace std;

random_device rdev;
mt19937 reng(rdev());
uniform_int_distribution<int> dist(0, 99);

using state_t = std::array<int, 100>;

auto generate()
{
	state_t state;
	std::iota(state.begin(), state.end(), 0);
	for (auto _ : std::views::iota(0, 100)) {
		const int i = dist(reng), k = dist(reng);
		if (i != k) std::swap(state[i], state[k]);
	};
	return state;
}

bool play(state_t const& state, bool is_random)
{
	for (int prisoner : std::views::iota(0, 100)) {
		bool found = false;
		int opened = 0;
		int drawer = is_random ? dist(reng) : prisoner;
		std::array<bool, 100> drawers_state {};

		while (opened < 50)
		{
			if (drawers_state[drawer]) {
				if (is_random) { drawer = dist(reng); continue; }
				else break;
			}
			
			drawers_state[drawer] = true;

			if (state[drawer] == prisoner) {
				found = true;
				break;
			}

			drawer = is_random ? dist(reng) : state[drawer];
			opened++;
		}

		if (!found)
			return false;
	}

	return true;
}

int main()
{
	auto state = generate();

	const unsigned trials = 100'0000u * 1000u;

	cout.precision(4);
	unsigned success[2] {};
	for (unsigned trial : std::views::iota(0u, trials)) {
		if (play(state, true)) success[0]++;
		if (play(state, false)) success[1]++;

		std::print("\r{:.4f}%, {} {} ", trial * 100.0 / double(trials), success[0], success[1]);
		cout.flush();
	}
	println("\rProbability (random): {}", success[0] / double(trials));
	println("\rProbability (optimal): {}", success[1] / double(trials));
}

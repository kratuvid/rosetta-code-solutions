#include <print>
#include <random>
#include <iostream>
#include <iomanip>
using namespace std;

random_device rdev;
mt19937 reng(rdev());
uniform_int_distribution<int> dist(0, 99);

void generate(int state[100])
{
	for (int i=0; i < 100; i++)
		state[i] = i;
	for (int i=0; i < 50; i++)
	{
		const int j = dist(reng), k = dist(reng);
		if (j != k) std::swap(state[j], state[k]);
	}
}

bool random_do(int state[100])
{
	bool drawers[100] {};

	for (int prisoner=0; prisoner < 100; prisoner++)
	{
		bool found = false;
		int countdown = 50;

		while (countdown != 0)
		{
			int chosen_drawer = dist(reng);
			if (drawers[chosen_drawer]) continue;
			else drawers[chosen_drawer] = true;

			if (state[chosen_drawer] == prisoner)
			{
				found = true;
				break;
			}

			countdown--;
		}

		if (!found)
			return false;
	}

	return true;
}

bool optimal_do(int state[100])
{
	bool drawers[100] {};

	for (int prisoner=0; prisoner < 100; prisoner++)
	{
		bool found = false;
		int countdown = 50;

		int chosen_drawer = prisoner;
		while (countdown != 0)
		{
			if (drawers[chosen_drawer]) continue;
			else drawers[chosen_drawer] = true;

			if (state[chosen_drawer] == prisoner)
			{
				found = true;
				break;
			}

			chosen_drawer = state[chosen_drawer];
			countdown--;
		}

		if (!found)
			return false;
	}

	return true;
}

int main()
{
	int state[100];
	generate(state);

	int successes = 0;
	const unsigned trials = unsigned(10'000);
	for (unsigned i=0; i < trials; i++)
	{
		bool win = random_do(state);
		if (win) successes++;

		cout << setprecision(4) << "\r" << i / double(trials) << "%   ";
		cout.flush();
	}
	println("\rSuccess rate (random): {}", (successes * 100.0) / double(trials));

	successes = 0;
	if (0)
	for (unsigned i=0; i < trials; i++)
	{
		bool win = optimal_do(state);
		if (win) successes++;

		cout << setprecision(4) << "\r" << i / double(trials) << "%   ";
		cout.flush();
	}
	println("\rSuccess rate (optimal): {}", (successes * 100.0) / double(trials));
}

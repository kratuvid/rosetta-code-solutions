import <array>;
import <print>;
using namespace std;

consteval auto perform()
{
    std::array<bool, 100> doors {};

	for (int i=1; i <= 100; i++)
	{
		for (int j = i; j <= 100; j += i)
		{
			doors[j-1] = !doors[j-1];
		}
	}
	
    return doors;
}

int main()
{
    auto doors = perform();

	for (int i=0; i < 100; i++)
		if (doors[i])
			println("{}", i+1);

    bool last = doors[0];
    int last_index = 0;
	if (0)
    for (int i=1; i < 100; i++)
    {
        if (doors[i] != last || i == 99)
        {
            println("{}-{}: {}, ", last_index, i, last);
            
            last = doors[i];
            last_index = i;
        }
    }
}

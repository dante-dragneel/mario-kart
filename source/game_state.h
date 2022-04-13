#define MAX_OBSTACLES 20
#define MAX_ENEMIES 5
#define MAX_VALUEPACKS 5

#define M 40
#define N 20
#define RESX 1280
#define RESY 720
#define CELL_SIZE 32

#define EXTRA_TIME 0
#define POWER_STAR 1
#define EXTRA_LIFE 2

#define GRASS 0
#define ROAD 1
#define OBSTACLE 2
#define FINISH 3

struct Cell
{
	int type;
};

struct Object
{
	int x;
	int y;
};

struct ValuePack
{
	int x;
	int y;
	int type;
};

struct GameState
{
	struct Object kart;
	struct Object enemies[MAX_ENEMIES];
	struct ValuePack valuepacks[MAX_VALUEPACKS];

	struct Cell map[M][N];

	int lives;
	int speed;
	int time_left;
	int score;
	int level;

	int won;
	int lost;
};

int objectWithinFrame(struct Object ob)
{
	if ((ob.x < RESX - CELL_SIZE) && (ob.y < 20 * CELL_SIZE))
	{
		if ((ob.x > 0) && (ob.y > 0))
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

int equal(struct Object ob1, struct Object ob2)
{
	if ((ob1.x == ob2.x) && (ob1.y == ob2.y))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int val_equal(struct Object ob1, struct ValuePack ob2)
{
	if ((ob1.x == ob2.x * CELL_SIZE) && (ob1.y == ob2.y * CELL_SIZE))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

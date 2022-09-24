#include <iostream>
#include <vector>
using namespace std;

class Point
{
  public:
    int x;
    int y;
    int z;
    bool vis;
};

class Vector3
{
  public:
    int x;
    int y;
    int z;
};

class Move
{
  public:
    int direction;
    bool valid;
};

class Path
{
  public:
    int x;
    int y;
    int z;
    int validMoves;
};

const int dimensions = 6;
const int maxPoints = dimensions * dimensions * dimensions;

Vector3 player;

vector<Point> pointsList;
vector<Path> path;

vector<Path> badWayList;
bool reverseOn = false;
bool foundPath = false;

int reverseTries = 0;
int tries = 0;
int floatCheck = 0;

long moveCount = 0;

Vector3 getVector(Path path)
{
  Vector3 vector;
  vector.x = path.x;
  vector.y = path.y;
  vector.z = path.z;
  return vector;
}

Vector3 getVector(Point point)
{
  Vector3 vector;
  vector.x = point.x;
  vector.y = point.y;
  vector.z = point.z;
  return vector;
}

void prepare()
{
  for (int x = 0; x < dimensions; x++)
  {
    for (int y = 0; y < dimensions; y++)
    {
      for (int z = 0; z < dimensions; z++)
      {
        Point point;
        point.vis = false;
        point.x = x;
        point.y = y;
        point.z = z;

        pointsList.push_back(point);
      }
    }
  }
}

bool checkVectors(Vector3 pos1, Vector3 pos2)
{
  bool noFail = true;

  if (pos1.x != pos2.x) {
    noFail = false;
  }

  if (pos1.y != pos2.y) {
    noFail = false;
  }

  if (pos1.z != pos2.z) {
    noFail = false;
  }

  return noFail;
}

void updatePoints()
{
  for (int i = 0; i < maxPoints; i++)
  {
    if (checkVectors(player, getVector(pointsList[i])))
    {
      pointsList[i].vis = true;
    }
  }
}

Path createPath(int validMoves)
{
  Path localPath;

  localPath.x = player.x;
  localPath.y = player.y;
  localPath.z = player.z;
  localPath.validMoves = validMoves;

  return localPath;
}

void placeRandom()
{
  int x = rand() % dimensions;
  int y = rand() % dimensions;
  int z = rand() % dimensions;

  Vector3 vector;
  vector.x = x;
  vector.y = y;
  vector.z = z;

  player = vector;

  updatePoints();
  path.push_back(createPath(3));
}

bool wrongMove(Vector3 pos)
{
  for (int i = 0; i < maxPoints; i++)
  {
    if (checkVectors(pos, getVector(pointsList[i])) && pointsList[i].vis)
    {
      return true;
    }
  }

  return false;
}

bool outOfDimensions(Vector3 pos)
{
  if (pos.x < 0 || pos.x >= dimensions)
  {
    return true;
  }

  if (pos.y < 0 || pos.y >= dimensions)
  {
    return true;
  }

  if (pos.z < 0 || pos.z >= dimensions)
  {
    return true;
  }

  return false;
}

Vector3 getNewPosition(int direction)
{
  Vector3 vector;
  vector.x = player.x;
  vector.y = player.y;
  vector.z = player.z;

  switch (direction)
  {
    case 1:
      vector.y = vector.y + 1;
      return vector;

    case 2:
      vector.z = vector.z + 1;
      return vector;

    case 3:
      vector.x = vector.x - 1;
      return vector;

    case 4:
      vector.y = vector.y - 1;
      return vector;

    case 5:
      vector.z = vector.z - 1;
      return vector;

    default:
      vector.x = vector.x + 1;
      return vector;
  }
}

vector<int> filterMoves(vector<Move> possible)
{
  vector<int> moves;

  for (int i = 0; i < 6; i++)
  {
    if (possible[i].valid)
    {
      moves.push_back(possible[i].direction);
    }
  }

  return moves;
}

bool routeBeenTried(Vector3 pos)
{
  for (int a = 0; a < badWayList.size(); a++)
  {
    if (checkVectors(pos, getVector(badWayList[a])))
    {
      vector<Path>::iterator it = badWayList.begin();
      it += a;
      badWayList.erase(it);
      return true;
    }
  }

  return false;
}

vector<int> getPossibleMoves()
{
  vector<Move> possible;

  for (int i = 0; i < 6; i++)
  {
    Move move;
    move.direction = i;
    move.valid = true;
    possible.push_back(move);
  }

  for (int i = 0; i < 6; i++) {
    Vector3 pos = getNewPosition(possible[i].direction);

    if (outOfDimensions(pos))
    {
      possible[i].valid = false;
    }

    if (wrongMove(pos) && possible[i].valid)
    {
      possible[i].valid = false;
    }
    
    if (routeBeenTried(pos) && possible[i].valid)
    {
      possible[i].valid = false;
    }
  }

  return filterMoves(possible);
}

void move(int direction, int validMoves)
{
  moveCount++;
  player = getNewPosition(direction);

  updatePoints();
  path.push_back(createPath(validMoves));
}

void updateBadWayList(Path p)
{
  bool match = false;

  for (int a = 0; a < badWayList.size(); a++)
  {
    if (checkVectors(getVector(p), getVector(badWayList[a])))
    {
      match = true;
    }
  }

  if (!match)
  {
    badWayList.push_back(p);
  }
}

void cutPath(int times)
{
  int pathSize = path.size();

  for (int i = pathSize - 1; i >= pathSize - times; i--)
  {
    for (int a = 0; a < maxPoints; a++)
    {
      if (checkVectors(getVector(path[i]), getVector(pointsList[a])))
      {
        pointsList[a].vis = false;
      }
    }

    vector<Path>::iterator vi = path.end() - 1;
    path.erase(vi);
  }

  Path p = path.back();

  Vector3 vector;
  vector.x = p.x;
  vector.y = p.y;
  vector.z = p.z;

  player = vector;
}

void revert()
{
  reverseTries++;

  for (int i = path.size() - 1; i >= 0; i--)
  {
    if (path[i].validMoves > 1)
    {
      updateBadWayList(path[i+1]);

      cutPath(i);
      return;
    }
  }
}

void reset()
{
  tries++;

  if (tries % 100000 == 0)
  {
    cout << tries;
    cout << " restarts\n";
  }

  reverseTries = 0;

  pointsList.clear();
  path.clear();

  prepare();
  placeRandom();
}

void generatePath()
{
  vector<int> moves = getPossibleMoves();

  int movesSize = moves.size();
  int pathSize = path.size();

  if (movesSize == 0)
  {
    if (pathSize == maxPoints)
    {
      foundPath = true;
      return;
    }

    if ((float)pathSize / (float)maxPoints > 0.75)
    {
      floatCheck++;

      if (floatCheck % 100000 == 0)
      {
        cout << floatCheck;
        cout << " float check pass\n";
      }

      if (reverseTries < 10000000)
      {
        revert();
      }

      else
      {
        // cout << "Reverse limit reached!\n";
        reset();
      }
    }

    else
    {
      if (tries < 500000000)
      {
        reset();
      }

      else
      {
        // cout << "Limit reached!\n";
        foundPath = true;
      }
    }

    return;
  }

  int direction = moves[rand() % movesSize];
  move(direction, movesSize);
}

int main()
{
  srand(time(0));

  prepare();
  placeRandom();

  while (!foundPath)
  {
    generatePath();
  }

  for (int i = 0; i < path.size(); i++)
  {
    cout << "Vector3(";
    cout << path[i].x;
    cout << ", ";
    cout << path[i].y;
    cout << ", ";
    cout << path[i].z;

    cout << "),\n";
  }

  cout << path.size();
  cout << "/";
  cout << maxPoints;
  cout << "\n";

  cout << tries;
  cout << " tries\n";

  cout << floatCheck;
  cout << " float check pass\n";

  cout << moveCount;
  cout << " move count\n";

  return 0;
}

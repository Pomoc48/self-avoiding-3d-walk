#include <iostream>
#include <vector>

using namespace std;

typedef struct {
  int x;
  int y;
  int z;
} Vector3;

typedef struct {
  Vector3 pos;
  bool vis;
} Point;

typedef struct {
  int direction;
  bool valid;
} Move;

typedef struct {
  Vector3 pos;
  int validMoves;
} Path;

const int dimensions = 4;
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

bool checkVectors(Vector3 pos1, Vector3 pos2) {
  if (pos1.x != pos2.x) return false;
  if (pos1.y != pos2.y) return false;
  if (pos1.z != pos2.z) return false;
  return true;
}

void updatePoints() {
  for (int i = 0; i < maxPoints; i++) {
    bool match = checkVectors(player, pointsList[i].pos);
    if (match) pointsList[i].vis = true;
  }
}

Path createPath(int validMoves) {
  Path localPath;

  localPath.pos = player;
  localPath.validMoves = validMoves;

  return localPath;
}

void placeRandom() {
  player.x = rand() % dimensions;
  player.y = rand() % dimensions;
  player.z = rand() % dimensions;

  updatePoints();
  path.push_back(createPath(3));
}

void prepare() {
  for (int x = 0; x < dimensions; x++) {
    for (int y = 0; y < dimensions; y++) {
      for (int z = 0; z < dimensions; z++) {
        Point point;
        point.vis = false;
        point.pos.x = x;
        point.pos.y = y;
        point.pos.z = z;

        pointsList.push_back(point);
      }
    }
  }
  placeRandom();
}

bool wrongMove(Vector3 pos) {
  for (int i = 0; i < maxPoints; i++) {
    bool match = checkVectors(pos, pointsList[i].pos);
    if (match && pointsList[i].vis) return true;
  }

  return false;
}

bool outOfDimensions(Vector3 pos)
{
  if (pos.x < 0 || pos.x >= dimensions) return true;
  if (pos.y < 0 || pos.y >= dimensions) return true;
  if (pos.z < 0 || pos.z >= dimensions) return true;

  return false;
}

Vector3 getNewPosition(int direction) {
  Vector3 vector = player;

  switch (direction) {
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

vector<int> filterMoves(vector<Move> possible) {
  vector<int> moves;

  for (int i = 0; i < 6; i++) {
    if (possible[i].valid) moves.push_back(possible[i].direction);
  }

  return moves;
}

bool routeBeenTried(Vector3 pos) {
  for (int a = 0; a < badWayList.size(); a++) {
    if (checkVectors(pos, badWayList[a].pos)) {
      vector<Path>::iterator it = badWayList.begin();
      it += a;
      badWayList.erase(it);
      return true;
    }
  }

  return false;
}

vector<int> getPossibleMoves() {
  vector<Move> possible;

  for (int i = 0; i < 6; i++) {
    Move move;
    move.direction = i;
    move.valid = true;
    possible.push_back(move);
  }

  for (int i = 0; i < 6; i++) {
    Vector3 pos = getNewPosition(possible[i].direction);

    if (outOfDimensions(pos)) {
      possible[i].valid = false;
    }

    if (wrongMove(pos) && possible[i].valid) {
      possible[i].valid = false;
    }
    
    if (routeBeenTried(pos) && possible[i].valid) {
      possible[i].valid = false;
    }
  }

  return filterMoves(possible);
}

void move(int direction, int validMoves) {
  moveCount++;
  player = getNewPosition(direction);

  updatePoints();
  path.push_back(createPath(validMoves));
}

void updateBadWayList(Path p) {
  bool match = false;

  for (int a = 0; a < badWayList.size(); a++) {
    bool vectorM = checkVectors(p.pos, badWayList[a].pos);
    if (vectorM) match = true;
  }

  if (!match) badWayList.push_back(p);
}

void cutPath(int times) {
  int pathSize = path.size();

  for (int i = pathSize - 1; i >= pathSize - times; i--) {
    for (int a = 0; a < maxPoints; a++) {
      if (checkVectors(path[i].pos, pointsList[a].pos)) {
        pointsList[a].vis = false;
      }
    }

    vector<Path>::iterator vi = path.end() - 1;
    path.erase(vi);
  }

  player = path.back().pos;
}

void revert() {
  reverseTries++;

  for (int i = path.size() - 1; i >= 0; i--) {
    if (path[i].validMoves > 1) {
      updateBadWayList(path[i+1]);
      cutPath(i);
      return;
    }
  }
}

void reset() {
  tries++;
  reverseTries = 0;

  pointsList.clear();
  path.clear();

  prepare();
}

void generatePath() {
  vector<int> moves = getPossibleMoves();

  int movesSize = moves.size();
  int pathSize = path.size();

  if (movesSize == 0) {
    if (pathSize == maxPoints) {
      foundPath = true;
      return;
    }

    if ((float)pathSize / (float)maxPoints > 0.75) {
      floatCheck++;
      if (reverseTries < 10000000) revert();
      else reset();
    }

    else {
      if (tries < 500000000) reset();
      else foundPath = true;
    }

    return;
  }

  int direction = moves[rand() % movesSize];
  move(direction, movesSize);
}

void showResults() {
  for (int i = 0; i < path.size(); i++) {
    cout << "Vector3(";
    cout << path[i].pos.x;
    cout << ", ";
    cout << path[i].pos.y;
    cout << ", ";
    cout << path[i].pos.z;

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
}

int main() {
  srand(time(0));

  prepare();

  while (!foundPath) generatePath();

  showResults();
  return 0;
}

import 'dart:math';

class Vector3 {
  int x;
  int y;
  int z;

  Vector3(this.x, this.y, this.z);
}

class Move {
  int direction;
  bool valid;

  Move(this.direction, this.valid);
}

class Data {
  int boxSize;
  List<List<List<bool>>> pointsList;
  Vector3 lastPosition;
  bool pathFound;
  List<Vector3> finalPath;

  Data({
    required this.boxSize,
    required this.pointsList,
    required this.lastPosition,
    required this.pathFound,
    required this.finalPath,
  });
}

void main(List<String> args) {
  Data data = _initializeData(args);
  if (data.boxSize == -1) return;
  // int cycles = 0;

  while (!data.pathFound) {
    data = _findThePath(data);
    // cycles++;
    // if (cycles % 50000000 == 0) {
    //   print(cycles);
    //   exit(0);
    // }
  }

  for (Vector3 vector in data.finalPath) {
    print("Vector3(${vector.x}, ${vector.y}, ${vector.z}),");
  }

  // print(cycles);
}

Data _initializeData(List<String> args, {int? boxSizePass}) {
  int boxSize = boxSizePass is int ? boxSizePass : _getBoxSize(args);
  List<List<List<bool>>> pointsList = _preparePointsList(boxSize);
  Vector3 lastPosition = _placeRandom(boxSize, pointsList);

  return Data(
    boxSize: boxSize,
    pointsList: pointsList,
    lastPosition: lastPosition,
    finalPath: [lastPosition],
    pathFound: false,
  );
}

int _getBoxSize(List<String> args) {
  if (args.isEmpty) {
    print("Box size not defined!");
    return -1;
  } else {
    try {
      int boxSize = int.parse(args[0]);
      return boxSize;
    } catch (e) {
      print("Box size not a number!");
      return -1;
    }
  }
}

List<List<List<bool>>> _preparePointsList(int boxSize) {
  List<List<List<bool>>> pointsList = [];
  for (int z = 0; z < boxSize; z++) {
    List<List<bool>> boolListList = [];
    for (int y = 0; y < boxSize; y++) {
      List<bool> boolList = [];
      for (int x = 0; x < boxSize; x++) {
        boolList.add(false);
      }
      boolListList.add(boolList);
    }
    pointsList.add(boolListList);
  }
  return pointsList;
}

Vector3 _placeRandom(int boxSize, List<List<List<bool>>> pointsList) {
  int x = _getRandom(boxSize);
  int y = _getRandom(boxSize);
  int z = _getRandom(boxSize);

  pointsList[x][y][z] = true;
  return Vector3(x, y, z);
}

Data _findThePath(Data data) {
  List<int> moveList = _getPossibleMoves(data);

  int pathSize = data.finalPath.length;
  int maxPathSize = pow(data.boxSize, 3).floor();

  if (moveList.isEmpty) {
    if (pathSize == maxPathSize) {
      data.pathFound = true;
      return data;
    }

    // TODO implement this later
    // if (pathSize / maxPathSize > 0.8) {

    // }

    return _initializeData([], boxSizePass: data.boxSize);
  }

  return _move(moveList[_getRandom(moveList.length)], data);
}

List<int> _getPossibleMoves(Data data) {
  List<Move> movesList = [];
  for (int d = 0; d < 6; d++) movesList.add(Move(d, true));

  for (Move move in movesList) {
    Vector3 nextPos = _getNextPosition(move.direction, data.lastPosition);

    if (_outOfDimensions(nextPos, data.boxSize)) {
      move.valid = false;
      continue;
    }

    if (_wrongMove(nextPos, data.pointsList)) {
      move.valid = false;
      continue;
    }
  }

  return _filterMoves(movesList);
}

Vector3 _getNextPosition(int direction, Vector3 pos) {
  Vector3 vector = Vector3(pos.x, pos.y, pos.z);
  switch (direction) {
    case 0: vector.x++;
      return vector;

    case 1: vector.y++;
      return vector;

    case 2: vector.z++;
      return vector;

    case 3: vector.x--;
      return vector;

    case 4: vector.y--;
      return vector;

    default: vector.z--;
      return vector;
  }
}

bool _outOfDimensions(Vector3 pos, int boxSize) {
  if (pos.x < 0 || pos.x >= boxSize) return true;
  if (pos.y < 0 || pos.y >= boxSize) return true;
  if (pos.z < 0 || pos.z >= boxSize) return true;

  return false;
}

bool _wrongMove(Vector3 pos, List<List<List<bool>>> pointsList) {
  if (pointsList[pos.x][pos.y][pos.z]) return true;
  return false;
}

List<int> _filterMoves(List<Move> list) {
  List<int> dirList = [];

  for (Move move in list) {
    if (move.valid) dirList.add(move.direction);
  }

  return dirList;
}

Data _move(int direction, Data data) {
  data.lastPosition = _getNextPosition(direction, data.lastPosition);
  data.finalPath.add(data.lastPosition);

  Vector3 pos = data.lastPosition;
  data.pointsList[pos.x][pos.y][pos.z] = true;

  return data;
}

int _getRandom(int max) {
  return Random().nextInt(max);
}

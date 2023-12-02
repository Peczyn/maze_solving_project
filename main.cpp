#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
using namespace std;


class MazeGenerator{
private:
    int mWidth;
    int mHeight;
    int mVisited;
    int mSize;
    vector<vector<int>> mMaze;
    stack<pair<int, int>> mStack;
    enum{
        North = 0x01,
        East = 0x02,
        South = 0x04,
        West = 0x08,
        CellVisited = 0x10,
    }; //WSEN


public:
    vector<vector<char>> matrix;


private:
    virtual bool CreateMaze(int width, int height);
    virtual bool UpdateMaze();

public:
    virtual bool GenerateFullMaze(int width, int height);
} maze;





class MazeSolver : public MazeGenerator
{
private:
    int startingPositionX;
    int startingPositionY;
    int destinationX;
    int destinationY;

    stack<char> moves;


    vector<vector<float>> Distances;
    vector<vector<char>> Solution;


private:
    virtual bool CountDistance();
    virtual bool FindSolutionMoves();
    virtual bool GenerateSolutionMatrix();


public:
    virtual bool GenerateSolutionForMaze();
};


template<typename T>
void PrintMatrix(const vector<vector<T>>& matrix);

















int main() {


    maze.GenerateFullMaze(10,20);


    MazeSolver maze1;
    maze1.matrix = maze.matrix;
    maze1.GenerateSolutionForMaze();




//    vector<vector<float>> odleglosci = MazeCounter(maze);
//    stack<char> moves = test(maze,odleglosci);
//    vector<vector<char>> way = MazeSolver(moves,maze);

//    PrintMatrix(matrix);
//    PrintMatrix(way);
    return 0;
}









bool MazeGenerator::CreateMaze(int width, int height) {
    mWidth = width;
    mHeight = height;
    mSize = mWidth*mHeight;

    mMaze =  vector<vector<int>>(mHeight,vector<int>(mWidth,0));

    int x = rand()%mWidth;
    int y = rand()%mHeight;

    mStack.push(make_pair(x,y));
    mMaze[y][x]= CellVisited;
    mVisited=1;

    return true;
}

bool MazeGenerator::UpdateMaze() {
    auto offsetX = [&](int x)
    {
        return (mStack.top().first + x);
    };
    auto offsetY = [&](int y)
    {
        return (mStack.top().second + y);
    };


    if(mVisited < mWidth*mHeight)
    {
        vector<int> neighbours;

        if(mStack.top().second > 0 && (mMaze[offsetY(-1)][offsetX(0)] & CellVisited) ==0)
        {
            neighbours.push_back(0);
        }
        if(mStack.top().first < mWidth-1  && (mMaze[offsetY(0)][offsetX(1)] & CellVisited) ==0)
        {
            neighbours.push_back(1);
        }
        if(mStack.top().second < mHeight-1  && (mMaze[offsetY(1)][offsetX(0)] & CellVisited) ==0)
        {
            neighbours.push_back(2);
        }
        if(mStack.top().first > 0 && (mMaze[offsetY(0)][offsetX(-1)] & CellVisited) ==0)
        {
            neighbours.push_back(3);
        }


        if(!neighbours.empty())
        {
            int nextCellDir = neighbours[rand() % neighbours.size()];

            switch(nextCellDir)
            {
                case 0:
                    mMaze[offsetY(-1)][offsetX(0)] |= CellVisited | South;
                    mMaze[offsetY(0)][offsetX(0)]  |= North;
                    mStack.push(make_pair((mStack.top().first+0),(mStack.top().second-1)));
                    break;
                case 1:
                    mMaze[offsetY(0)][offsetX(1)] |= CellVisited | West;
                    mMaze[offsetY(0)][offsetX(0)]  |= East;
                    mStack.push(make_pair((mStack.top().first+1),(mStack.top().second+0)));
                    break;
                case 2:
                    mMaze[offsetY(1)][offsetX(0)] |= CellVisited | North;
                    mMaze[offsetY(0)][offsetX(0)]  |= South;
                    mStack.push(make_pair((mStack.top().first+0),(mStack.top().second+1)));
                    break;
                case 3:
                    mMaze[offsetY(0)][offsetX(-1)] |= CellVisited | East;
                    mMaze[offsetY(0)][offsetX(0)]  |= West;
                    mStack.push(make_pair((mStack.top().first-1),(mStack.top().second+0)));
                    break;
            }
            mVisited++;
        }
        else
        {
            mStack.pop();
        }
    }
    return true;
}

bool MazeGenerator::GenerateFullMaze(int width, int height) {
    srand((unsigned) time(nullptr));
    CreateMaze(width,height);
    while(mVisited!=mSize)
    {
        UpdateMaze();
    }



    for(int i=0; i<mHeight*3; i++)
    {
        vector<char> temp;
        for(int j=0; j<mWidth*3; j++)
        {
            temp.push_back('@');
        }
        matrix.push_back(temp);
    }

    for(int row =0; row<mHeight; row++)
    {
        for(int collumn=0; collumn<mWidth; collumn++) {
            matrix[row*3+1][collumn*3+1]=' ';
            if ((mMaze[row][collumn] & 0x01) == 1) {
                matrix[row*3][collumn*3 + 1] = ' ';
            }
            if ((mMaze[row][collumn] & 0x02) == 2) {
                matrix[row*3 + 1][collumn*3 + 2] = ' ';
            }
            if ((mMaze[row][collumn] & 0x04) == 4) {
                matrix[row*3 + 2][collumn*3 + 1] = ' ';
            }
            if ((mMaze[row][collumn] & 0x08) == 8) {
                matrix[row*3 + 1][collumn*3] = ' ';
            }
        }
    }
    PrintMatrix(matrix);
    return true;
}



template<typename T>
void PrintMatrix(const vector<vector<T>>& matrix)
{
    for(auto &a : matrix)
    {
        for(auto &b: a) cout << b << ' ';
        cout << endl;
    }
    cout << endl;
}


bool MazeSolver::CountDistance() {


    destinationX= matrix[0].size()-2;
    destinationY= matrix.size()-2;

    for(int row=0; row<matrix.size(); row++)
    {
        vector<float> temp;

        for(int collumn=0; collumn<matrix[0].size(); collumn++)
        {
            if(matrix[row][collumn]!='@')
            {
                temp.push_back(sqrt(pow(row-destinationX,2)+pow(collumn-destinationY,2)));
            }
            else
            {
                temp.push_back(-1);
            }

        }
        Distances.push_back(temp);
    }
}


bool MazeSolver::FindSolutionMoves() {
    moves.push('X');

    startingPositionX = 1;
    startingPositionY = 1;

    int counter = 1;
    while(startingPositionX!=destinationX || startingPositionY!=destinationY){

        Distances[startingPositionY][startingPositionX]=-2;

        float directions[4]=  {-1,-1,-1,-1};

        if(startingPositionY!=0) directions[0] =  Distances[startingPositionY-1][startingPositionX];
        if(startingPositionX!= Distances[0].size()-1) directions[1] =  Distances[startingPositionY][startingPositionX+1];
        if(startingPositionY!= Distances.size()-1) directions[2] =  Distances[startingPositionY+1][startingPositionX];
        if(startingPositionX!=0) directions[3] =  Distances[startingPositionY][startingPositionX-1];

        float min= INT32_MAX;

        counter = 0;
        for(int i=0; i<4; i++)
        {
            if(directions[i]>=0) counter++;
            if(directions[i]<min && directions[i]>=0) min=directions[i];
        }

        if(counter==0)
        {
            if(moves.top()=='X') moves.pop();
            if(moves.empty()) return false;

            while(moves.top()!='X')
            {
                if      (moves.top()=='N')   startingPositionY++;
                else if (moves.top()=='E')   startingPositionX--;
                else if (moves.top()=='S')   startingPositionY--;
                else if (moves.top()=='W')   startingPositionX++;
                moves.pop();
            }
            continue;
        }


        if(counter>1) moves.push('X');

        if(directions[0]==min)
        {
            moves.push('N');
            startingPositionY--;
        }
        else if(directions[1]==min)
        {
            moves.push('E');
            startingPositionX++;
        }
        else if(directions[2]==min)
        {
            moves.push('S');
            startingPositionY++;
        }
        else if(directions[3]==min)
        {
            moves.push('W');
            startingPositionX--;
        }
    }
}


bool MazeSolver::GenerateSolutionMatrix() {
    Solution = vector<vector<char>>(Distances.size(),vector<char>(Distances[0].size(),'.'));
    int a = destinationX;
    int b = destinationY;

    while(!moves.empty())
    {
        Solution[b][a] = 'X';
        if(moves.top()=='N') b++;
        else if(moves.top()=='E') a--;
        else if(moves.top()=='S')b--;
        else if(moves.top()=='W')a++;
        moves.pop();
    }
    Solution[b][a]= 'X';
}

bool MazeSolver::GenerateSolutionForMaze() {

    CountDistance();
    FindSolutionMoves();
    GenerateSolutionMatrix();
    PrintMatrix(Solution);
    return true;
}


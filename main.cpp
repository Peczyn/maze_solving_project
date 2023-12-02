#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
using namespace std;


class Maze{
public:
    int mWidth;
    int mHeight;

    int mVisited;
    int mSize;
    vector<vector<int>> mMaze;

    int startingPositionX = 1;
    int startingPositionY = 1;
    int destinationX = 13;
    int destinationY = 13;

    vector<vector<char>> matrix;

private:
    enum{
        North = 0x01,
        East = 0x02,
        South = 0x04,
        West = 0x08,
        CellVisited = 0x10,
    }; //WSEN


    stack<pair<int, int>> mStack;
    int mPathWidth;

public:
    virtual bool CreateMaze(int width, int height);

    virtual bool Update()
    {
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
};



template<typename T>
void PrintMatrix(const vector<vector<T>>& matrix);

vector<vector<float>> MazeCounter(Maze maze);

vector<vector<char>> MazeSolver(stack<char> moves, Maze maze);

stack<char> test(Maze maze, vector<vector<float>> x);

void StworzLabirynt(Maze &M);

int main() {

    Maze maze;
    StworzLabirynt(maze);
    maze.destinationX= maze.matrix[0].size()-2;
    maze.destinationY= maze.matrix.size()-2;
    vector<vector<float>> odleglosci = MazeCounter(maze);
    stack<char> moves = test(maze,odleglosci);
    vector<vector<char>> way = MazeSolver(moves,maze);

    PrintMatrix(maze.matrix);
//    PrintMatrix(odleglosci);
    PrintMatrix(way);
    return 0;
}


bool Maze::CreateMaze(int width, int height) {
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

vector<vector<float>> MazeCounter(Maze maze)
{
    vector<vector<float>> CountedMatrix;

    for(int row=0; row<maze.matrix.size(); row++)
    {
        vector<float> temp;
        for(int collumn=0; collumn<maze.matrix[0].size(); collumn++)
        {
            if(maze.matrix[row][collumn]!='@')
            {
                temp.push_back(sqrt(pow(row-maze.destinationX,2)+pow(collumn-maze.destinationY,2)));
            }
            else
            {
                temp.push_back(-1);
            }

        }
        CountedMatrix.push_back(temp);
    }
    return CountedMatrix;
}

vector<vector<char>> MazeSolver(stack<char> moves,Maze maze)
{
    vector<vector<char>> way(maze.matrix.size(),vector<char>(maze.matrix[0].size(),'.'));
    int a = maze.destinationX;
    int b = maze.destinationY;

    while(!moves.empty())
    {
        way[b][a] = 'X';
        if(moves.top()=='N') b++;
        else if(moves.top()=='E') a--;
        else if(moves.top()=='S')b--;
        else if(moves.top()=='W')a++;
        moves.pop();
    }
    way[b][a]= 'X';
    return way;
}

stack<char> test(Maze maze, vector<vector<float>> x)
{
    stack<char> moves;
    moves.push('X');
    int counter = 1;
    while(maze.startingPositionX!=maze.destinationX || maze.startingPositionY!=maze.destinationY){
        x[maze.startingPositionY][maze.startingPositionX]=-2;
        float directions[4]=  {-1,-1,-1,-1};

        if(maze.startingPositionY!=0) directions[0] = x[maze.startingPositionY-1][maze.startingPositionX];
        if(maze.startingPositionX!=x.size()-1) directions[1] = x[maze.startingPositionY][maze.startingPositionX+1];
        if(maze.startingPositionY!=x.size()-1) directions[2] = x[maze.startingPositionY+1][maze.startingPositionX];
        if(maze.startingPositionX!=0) directions[3] = x[maze.startingPositionY][maze.startingPositionX-1];

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
            if(moves.empty()) return moves;
            while(moves.top()!='X')
            {
                if(moves.top()=='N') maze.startingPositionY++;
                else if(moves.top()=='E')maze.startingPositionX--;
                else if(moves.top()=='S')maze.startingPositionY--;
                else if(moves.top()=='W')maze.startingPositionX++;
                moves.pop();
            }
            continue;
        }


        if(counter>1) moves.push('X');

        if(directions[0]==min)
        {
            moves.push('N');
            maze.startingPositionY--;
        }
        else if(directions[1]==min)
        {
            moves.push('E');
            maze.startingPositionX++;
        }
        else if(directions[2]==min)
        {
            moves.push('S');
            maze.startingPositionY++;
        }
        else if(directions[3]==min)
        {
            moves.push('W');
            maze.startingPositionX--;
        }
    }
    return moves;
}

void StworzLabirynt(Maze &M)
{
    srand((unsigned) time(NULL));
    M.CreateMaze(15,110);
    while(M.mVisited!=M.mSize)
    {
        M.Update();
    }



    for(int i=0; i<M.mHeight*3; i++)
    {
        vector<char> temp;
        for(int j=0; j<M.mWidth*3; j++)
        {
            temp.push_back('@');
        }
        M.matrix.push_back(temp);
    }

    for(int row =0; row<M.mHeight; row++)
    {
        for(int collumn=0; collumn<M.mWidth; collumn++) {
            M.matrix[row*3+1][collumn*3+1]=' ';
            if ((M.mMaze[row][collumn] & 0x01) == 1) {
                M.matrix[row*3][collumn*3 + 1] = ' ';
            }
            if ((M.mMaze[row][collumn] & 0x02) == 2) {
                M.matrix[row*3 + 1][collumn*3 + 2] = ' ';
            }
            if ((M.mMaze[row][collumn] & 0x04) == 4) {
                M.matrix[row*3 + 2][collumn*3 + 1] = ' ';
            }
            if ((M.mMaze[row][collumn] & 0x08) == 8) {
                M.matrix[row*3 + 1][collumn*3] = ' ';
            }
        }
    }
}
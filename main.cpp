#include <iostream>
#include <vector>
#include <cmath>
#include <stack>
using namespace std;

class Maze{
public:
    vector<vector<char>> matrix {
            {'1','1','1','1','1','1','0','1','1','0'},
            {'1','1','1','1','0','1','0','0','0','0'},
            {'1','1','1','1','0','0','1','1','1','1'},
            {'1','1','1','1','0','0','1','0','0','1'},
            {'1','1','1','1','0','0','1','0','0','1'},
            {'1','1','1','1','0','0','1','0','0','1'},
            {'1','0','0','1','1','0','1','0','0','1'},
            {'1','0','0','1','1','1','1','1','0','1'},
            {'1','0','1','1','1','0','1','0','0','1'},
            {'1','1','1','1','1','0','1','0','0','1'},
    };
    int startingPositionX = 1;
    int startingPositionY = 1;
    int destinationX = 6;
    int destinationY = 6;
};

vector<vector<float>> MazeCounter(Maze maze);

template<typename T>
void WypiszTablice(const vector<vector<T>>& matrix);

vector<vector<char>> RozwiazanieLabiryntu(stack<char> moves, Maze maze);

stack<char> test(Maze maze, vector<vector<float>> x);


int main() {
    Maze maze;
    vector<vector<float>> odleglosci = MazeCounter(maze);
    WypiszTablice(odleglosci);

    stack<char> moves = test(maze,odleglosci);
    vector<vector<char>> way = RozwiazanieLabiryntu(moves,maze);

    WypiszTablice(way);
    return 0;
}



vector<vector<float>> MazeCounter(Maze maze)
{
    vector<vector<float>> result;
    for(int i=0; i<maze.matrix.size(); i++)
    {
        vector<float> temp;
        for(int j=0; j<maze.matrix.size(); j++)
        {
            if(maze.matrix[i][j]=='1')
            {
                temp.push_back(sqrt(pow(i-maze.destinationX,2)+pow(j-maze.destinationY,2)));
            }
            else
            {
                temp.push_back(-1);
            }

        }
        result.push_back(temp);
    }
    return result;
}

template<typename T>
void WypiszTablice(const vector<vector<T>>& matrix)
{
    for(auto &a : matrix)
    {
        for(auto &b: a) cout << b << '\t';
        cout << endl;
    }

}

vector<vector<char>> RozwiazanieLabiryntu(stack<char> moves,Maze maze)
{
    vector<vector<char>> way(10,vector<char>(10,'.'));
    int a = maze.destinationX;
    int b = maze.destinationY;
    cout << endl << endl;
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
        for(int i=0; i<3; i++)
        {
            if(directions[i]>=0) counter++;
            if(directions[i]<min && directions[i]>=0) min=directions[i];
        }

        if(counter==0)
        {
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
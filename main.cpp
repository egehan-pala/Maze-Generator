#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <vector>
#include <random>
#include <chrono>

using namespace std;
// Define a structure for a node in a linked list
template <class type>
struct node
{
    type value;
    node<type>* next;

    // Constructor with default arguments
    node(type value = type(), node<type>* next = nullptr) : value(value), next(next) {}
};

// Define a stack data structure using linked list
template <class type>
class stack{
    private:
        node<type>* top;

    public:
        stack() {this->top = nullptr;}// Constructor to initialize top of stack
    
        stack(stack<type> &copy){
            //Copy constructor to create a deep copy of the stack
            node<type>* ptr = top->next;
            node<type>* copy_top = new node<type>(top->value, nullptr);
            node<type>* copy_ptr = copy_top;
            while(ptr != nullptr){
                copy_ptr->next = new node<type>(ptr->value, nullptr);
                ptr = ptr->next;
                copy_ptr = copy_ptr->next;
            }
        }
        ~stack(){
            // Destructor to deallocate memory for stack
            while(!isEmpty()){
                pop();
            }
        }
        bool isEmpty() const{// Check if the stack is empty
            return top == nullptr;
        }
    
        type& topValue(){
            // Return the top value of the stack
            if(!isEmpty())
                return top->value;
            else
                throw out_of_range("Stack is empty");
        }

        void push(const type &value){// Push a value onto the stack
            node<type>* ptr = new node<type>(value, nullptr);
            ptr->next = top;
            top = ptr;
        }
        void pop(){ // Pop the top element from the stack
            if(!isEmpty())
            {
                node<type>* ptr = top->next;
                delete top;
                top = ptr;
            }
        }
};

// Define a structure for a cell in the maze
struct cell {
    int x;
    int y;
    int upWall;
    int downWall;
    int rightWall;
    int leftWall;
    bool visited;
    bool left;
    bool right;
    bool up;
    bool down;
    
    // Constructor with default arguments
    cell(){}
    cell(int X, int Y) : x(X), y(Y), upWall((int)1), downWall((int)1), rightWall((int)1), leftWall((int)1), visited(false), left(false), right(false), up(false), down(false) {}
};

// Random number generator
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count()); 
// chrono command for shuffle -taken from stackOverFlow
// https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c

// Function to check neighboring cells
void neigbourChecker(vector<int> &neigbourTails, stack<cell> &mazeStack, vector<vector<cell> > &maze,  cell &currentCell)
{
    // done checks for overflow and visted posibility
    if (currentCell.y > 0 && !maze[currentCell.x][currentCell.y-1].visited){
        neigbourTails.push_back(0); // left
    }
    if (currentCell.y < maze[0].size() - 1 && !maze[currentCell.x][currentCell.y+1].visited) {
        neigbourTails.push_back(1); // Right
    }
    if(currentCell.x <  maze.size() - 1&& !maze[currentCell.x+1][currentCell.y].visited) {
        neigbourTails.push_back(2); // up
    }
    if(currentCell.x > 0 && !maze[currentCell.x-1][currentCell.y].visited) {
        neigbourTails.push_back(3); // down
    }
}


// Function to create a maze
void createMaze(vector<vector<cell> > &maze)
{
    stack<cell> mazeStack;
    mazeStack.push(cell(0,0)); 
    maze[0][0].visited = true;

    while(!mazeStack.isEmpty())
    {
        cell currentCell = mazeStack.topValue();
        vector<int> neigbourTails;// Vector to store possible neighboring directions
        neigbourChecker(neigbourTails, mazeStack, maze, currentCell);// Check neighboring cells
        
        
        // Check if there are unvisited neighboring cells
        if (!neigbourTails.empty()) {
            
            shuffle(neigbourTails.begin(), neigbourTails.end(), rng); //shuffling the vector so that the possible directions can be random
            int num = neigbourTails[0];// Select a random neighboring cell
            
            // Check the neighboring cells in random order
            if (num == 0 ) {
                // Move left
                if (currentCell.y > 0 && !maze[currentCell.x][currentCell.y-1].visited)
                {
                    // Update walls
                    maze[currentCell.x][currentCell.y].leftWall = 0;
                    maze[currentCell.x][currentCell.y].left = true;
                    maze[currentCell.x][currentCell.y-1].rightWall = 0;
                    maze[currentCell.x][currentCell.y-1].right = true;
                    maze[currentCell.x][currentCell.y-1].visited = true; // make visited true
                    
                    currentCell = maze[(currentCell.x)][currentCell.y-1]; // update the current cell
                    currentCell.left = true;
                    mazeStack.push(maze[currentCell.x][currentCell.y]);
                 
                }
            }
            else if (num == 1) {
                // Move right
                if (currentCell.y < maze[0].size() - 1 && !maze[currentCell.x][currentCell.y+1].visited)
                {
                    // Update walls
                    maze[currentCell.x][currentCell.y].rightWall = 0;
                    maze[currentCell.x][currentCell.y].right = true;
                    maze[currentCell.x][currentCell.y+1].leftWall = 0;
                    maze[currentCell.x][currentCell.y+1].left = true;
                    maze[currentCell.x][currentCell.y+1].visited = true; // make visited true
                    
                    currentCell = maze[(currentCell.x)][currentCell.y+1];// update the current cell
                    currentCell.right = true;
                    mazeStack.push(maze[currentCell.x][currentCell.y]);
               
                }
                
            }
            else if (num == 2) {
                // Move up
                if(currentCell.x <  maze.size() - 1&& !maze[currentCell.x+1][currentCell.y].visited){
                    // Update walls
                    maze[currentCell.x][currentCell.y].upWall = 0;
                    maze[currentCell.x][currentCell.y].up = true;
                    maze[currentCell.x+1][currentCell.y].downWall = 0;
                    maze[currentCell.x+1][currentCell.y].down = true;
                    maze[currentCell.x+1][currentCell.y].visited = true;// make visited true
                    
                    currentCell = maze[(currentCell.x+1)][currentCell.y];// update the current cell
                    currentCell.up = true;
                    mazeStack.push(maze[currentCell.x][currentCell.y]);
                }
            }
            else if (num == 3) {
                // Move down
                if(currentCell.x > 0 && !maze[currentCell.x-1][currentCell.y].visited){
                    // Update walls
                    maze[currentCell.x][currentCell.y].downWall = 0;
                    maze[currentCell.x][currentCell.y].down = true;
                    maze[currentCell.x-1][currentCell.y].upWall = 0;
                    maze[currentCell.x-1][currentCell.y].up = true;
                    maze[currentCell.x-1][currentCell.y].visited = true;// make visited true
                    
                    currentCell = maze[(currentCell.x-1)][currentCell.y];// update the current cell
                    currentCell.down = true;
                    mazeStack.push(maze[currentCell.x][currentCell.y]);
                }
            }
        }
        else{
            // If there are no unvisited neighboring cells, backtrack
            mazeStack.pop();
        }
    }

}

// Function to write maze to file
void writeMazeToFile(const vector<vector<cell> >& maze, int mazeID) {
    ofstream outFile("maze_" + to_string(mazeID) + ".txt");
    if (outFile.is_open()) {// Write maze dimensions to file
        outFile << maze.size() << " " << maze[0].size() << endl;
        for (int i = 0; i < maze.size(); ++i) {
            for (int j = 0; j < maze[0].size(); ++j) {
                outFile << "x=" << j << " y=" << i << " l=" << maze[i][j].leftWall << " r=" << maze[i][j].rightWall
                    << " u=" << maze[i][j].upWall << " d=" << maze[i][j].downWall << endl;
            }
        }
        outFile.close();
    }  
    else {
        cerr << "Unable to open file for writing" << endl;// Print error message if unable to open file
    }
}

// Path Finder Function
void pathFinder(vector<vector<cell> > &maze, int &mazeID){
    // Reset visited flags to make the maze searchable again
    for (int i = 0; i< maze.size(); i++){
        for (int j=0; j< maze[i].size(); j++){
            maze[i][j].visited =false;
        }
    }
    int x_in, y_in;
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): "; cin >> x_in >> y_in;
    int x_out, y_out;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): "; cin >> x_out >> y_out;

    stack<cell> mStack;
    mStack.push(cell(x_in,y_in));// Push starting cell to stack
    static cell currentCell = mStack.topValue();
    vector<cell> path;//to store data of visited cells while finding path
    path.push_back(cell(x_in,y_in));
    
    // Iterate until current cell reaches the exit point
    while (!(currentCell.x == x_out && currentCell.y == y_out)) {
        currentCell = mStack.topValue();// Get top cell from stack
        maze[currentCell.x][currentCell.y].visited = true;// Mark current cell as visited
            // Check the neighboring cells
        
            if (currentCell.x > 0 && !maze[currentCell.x-1][currentCell.y].visited && maze[currentCell.x][currentCell.y].down) {
                //move down
                maze[(currentCell.x-1)][currentCell.y].visited = true;// Mark neighboring cell as visited
                currentCell = maze[(currentCell.x-1)][currentCell.y];// Move to neighboring cell
                mStack.push(maze[currentCell.x][currentCell.y]);// Push neighboring cell to stack
                path.push_back(currentCell);// Add neighboring cell to path
            }
            else if (currentCell.x <  maze.size() - 1&& !maze[currentCell.x+1][currentCell.y].visited && maze[currentCell.x][currentCell.y].up) {
                //move up
                maze[(currentCell.x+1)][currentCell.y].visited = true;// Mark neighboring cell as visited
                currentCell = maze[(currentCell.x+1)][currentCell.y];// Move to neighboring cell
                mStack.push(maze[currentCell.x][currentCell.y]);// Push neighboring cell to stack
                path.push_back(currentCell);// Add neighboring cell to path  it is similar for the rest 2
                
            }
            else if (currentCell.y > 0 && !maze[currentCell.x][currentCell.y-1].visited && maze[currentCell.x][currentCell.y].left) {
                //move left
                maze[currentCell.x][(currentCell.y-1)].visited = true;
                currentCell = maze[currentCell.x][(currentCell.y-1)];
                mStack.push(maze[currentCell.x][currentCell.y]);
                path.push_back(currentCell);
            }
            else if (currentCell.y < maze[0].size() - 1 && !maze[currentCell.x][currentCell.y+1].visited && maze[currentCell.x][currentCell.y].right) {
                //move right
                maze[currentCell.x][(currentCell.y+1)].visited = true;
                currentCell = maze[currentCell.x][(currentCell.y+1)];
                mStack.push(maze[currentCell.x][currentCell.y]);
                path.push_back(currentCell);
            }
        
        
        else{
            // If there's no unvisited neighboring cell, backtrack
            mStack.pop();
            path.pop_back();
        }
    }
        // Write path coordinates to a file
        string filename = "maze_" + to_string(mazeID) + "path" +
            to_string(x_in) + "" + to_string(y_in) + "" +
            to_string(x_out) + "_" + to_string(y_out) + ".txt";
        ofstream outFile(filename);
        if (!outFile.is_open()) {
            cout << "Unable to open file: " << filename << endl;
            return;
        }
         for (int i = 0; i < path.size(); ++i) {
            outFile << path[i].x << " " << path[i].y << endl;
        }
        outFile.close();
}


int main(){
    
    int mazeNums; cout << "Enter the number of mazes: "; cin >> mazeNums;
    int mazeRows, mazeCols; cout << "Enter the number of rows and columns (M and N): "; cin >> mazeRows >> mazeCols;

    vector<vector<vector<cell> > > allMazes; //vector to store all of the mazes
    // Generate mazes and store them in allMazes vector
    for (int k=1; k<=mazeNums; k++)
    {
        //creating the open maze schema
        vector<vector<cell> > maze(mazeRows,vector<cell>(mazeCols)); //maze with all walls closed
        for (int j = 0; j< mazeRows; j++)
        {
            for (int i = 0; i < mazeCols; i++)
            {
                maze[j][i] = cell(j, i);
            }
        }
        // Generate the maze
        createMaze(maze);
        // Write maze to file
        writeMazeToFile(maze, k);
        allMazes.push_back(maze);
    }
    cout << "All mazes are generated." << endl;
    int mazeID;
    cout << "Enter a maze ID between 1 to "<< allMazes.size() <<" inclusive to find a path: "; cin >> mazeID;
    pathFinder(allMazes[mazeID-1], mazeID);
    writeMazeToFile(allMazes[mazeID-1], mazeID);

    return 0;
}

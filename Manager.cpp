#include "Manager.h"
#include <cstring>
using namespace std;

int a = 0;  //Variables to determine if the Rabinkarp command started

Manager::~Manager()
{
    /*Every time the program starts, erase the contents of the file*/
    if (fout.is_open())
        fout.close();

    if (ferr.is_open())
        ferr.close();
}

void Manager::Run(const char* filepath)
{
    fout.open(RESULT_LOG_PATH);
    ferr.open(ERROR_LOG_PATH);

    ifstream fin;
    fin.open(filepath); //open command.txt
    if (!fin)   //if command file is not exist, print error
    {
        fout << "=====SYSTEM=====" << endl;
        fout << "CommandFileNotExist" << endl;
        fout << "=================" << endl << endl;
        return; //End of the program
    }

    char cmd[32];
    string RabincarpFile;   //store Rabincarp's data file name

    while (!fin.eof())
    {
        fin.getline(cmd, 32);
        char* tmp = strtok(cmd, " ");
	if(tmp==NULL) break;	//if command is finish, end program
        if (!strcmp(cmd, "LOAD"))
        {
            tmp = strtok(NULL, " ");
            PrintError(Load(tmp));  //print error code (Include success case)

        }
        else if (!strcmp(cmd, "FLOYD"))
        {
            int size = m_graph.Size();
            enum Result error = FindShortestPathFloyd();
            if (error != 0) //if not success, print error code
                PrintError(error); 
           
        }
        else if (!strcmp(cmd, "DIJKSTRA"))
        {
	    tmp=strtok(NULL," ");
	    if(tmp)	//if input over command, print error code
	    {
		    fout<<"====InvalidVertexKey===="<<endl;
		    fout<<"Error code : "<<InvalidVertexKey<<endl;
		    fout<<"==================="<<endl<<endl;
	    }
	    else
	    {
      	   	   int size = m_graph.Size();
      	    	  enum Result error = FindShortestPathDijkstraUsingSet(0, size - 1);  //startvertex, endvertex
        	    if (error != 0) //if not success, print error code
          	      PrintError(error);
	    }
        }
        else if (!strcmp(cmd, "BFS"))
        {
	    tmp=strtok(NULL," ");
	    if(tmp)	//if input over command, print error code
	    {
		    fout<<"====InvalidVertexKey===="<<endl;
		    fout<<"Error code : "<<InvalidVertexKey<<endl;
		    fout<<"=================="<<endl<<endl;
	    }
	    else
	    {
         	   int size = m_graph.Size();
                   enum Result error = FindPathBfs(0, size - 1);   //startvertex, endvertex
         	   if (error != 0) //if not success, print error code
              		  PrintError(error);
	    }
        }
        else if (!strcmp(cmd, "BELLMANFORD"))
        {
	    tmp=strtok(NULL," ");
	    if(tmp)	//if input over command, print error code
	    {
		    fout<<"====InvalidVertexKey===="<<endl;
		    fout<<"Error code : "<<InvalidVertexKey<<endl;
		    fout<<"===================="<<endl<<endl;
	    }
	    else
	    {
          	  int size = m_graph.Size();
        	  enum Result error = FindShortestPathBellmanFord(0, size - 1);   //startvertex, endvertex
        	    if (error != 0)  //if not success, print error code
            		    PrintError(error);
	    }
        }
        else if (!strcmp(cmd, "LOADREPORT"))
        {
            tmp = strtok(NULL, " ");
            RabincarpFile = tmp;    //store file name
            PrintError(LOADREPORT(tmp));  //print error code (Include success case)
        }
        else if (!strcmp(cmd, "RABINKARP"))
        {
            a = 0;  //Rabinkarp command start
            tmp = strtok(NULL, "\n"); //Cut strings including spaces
       
            if (RabincarpFile.empty()) //if do not exist reportdata file, print error code
                PrintError(FaildtoUpdatePath);
            else //reportdata file is exist, Run rabinkarp
            {
                bool isEmpty;
                ifstream read;

                isEmpty = read.peek() == EOF;
                read.open(RabincarpFile);

                if (read.good())	//read successful of data.txt
                {
                    char data[200];
                    enum Result result; //Variables to determine whether a string has been found
                    int check = 0;  //if check is 0, Not found the string
                    bool finish = false;    //if true, finish rabinkarp
                    while (read.getline(data, sizeof(data)))
                    {
                        if (data == NULL)	//if File is empty, print error code
                            PrintError(LoadFileNotExist);
                        else
                        {
                            result = RabinKarpCompare(tmp, data);
                            if (result == Success)  //if success, found the string
                                check++;

                            if (result == InvalidOptionNumber)  //if string is more than 10 characters, print error code
                            {
                                PrintError(result);
                                finish = true;  //finish Rabinkarp
                                break;
                            }
                        }
                    }
                    if (check == 0 && !finish)  //Not found the string
                    {
                        fout << "======RABINKARP======" << endl;
                        fout << "NO DUPLICATE TITLE EXISTS" << endl;
                    }
                    if (!finish)
                        fout << "==================" << endl << endl;
                }
            }
        }
        else if(!strcmp(cmd,"PRINT"))
        {
		enum Result result=Print();
		if(result!=Success)
			PrintError(result);
        }
        else //if a command that doesn't exist
        {
            /*In case of annotation*/
            int check = 2;
            for (int i = 0; i < 2; i++)
            {
                if (cmd[i] == '/')
                    check-=1;
            }
	    if (check != 0) //this case is not annotation
            {
                /*do not exist command, print error message*/
                fout << "======ASTAR======" << endl;
                fout << "NonDefinedCommand" << endl;
                fout << "=================" << endl << endl;
            }
        }
    }
}
void Manager::PrintError(Result result) //print error code
{
    fout << "================" << std::endl;
    fout << "Error code: " << result << std::endl;
    fout << "================" << std::endl << endl;
}



/// <summary>
/// make a graph
/// </summary>
///
/// <param name="filepath">
/// the filepath to read to make the graph
/// </param>
///
/// <returns>
/// Result::Success if load is successful.
/// Result::LoadFileNotExist if file is not exist.
/// </returns>
Result Manager::Load(const char* filepath)  //load mapdata file
{
    bool isEmpty;
    ifstream read;

    isEmpty = read.peek() == EOF;
    read.open(filepath);    //file open
   
    if (read.good())	//read successful of mapdata.txt
    {
        char data[200];
        int vertex = 0; //startVertex
        while (read.getline(data, sizeof(data)))
        {
            if (data == NULL)	//if File is empty, print error code
                return LoadFileNotExist;
            else
            {
                int num = 0;    //v_key
                m_graph.AddLine();  //insert from vertex
                char* tmp = strtok(data, "/"); // '/'Divide strings based on the criteria
                int weight;
        
                tmp = strtok(NULL, " ");    //cut one weight
                while (tmp) 
                {
                    weight = atoi(tmp); //change weight to 'int' type
                    m_graph.AddNode(vertex, num, weight);
                    num += 1;
                    tmp = strtok(NULL, " ");
                }
                vertex += 1;    //next vertex
            }
        }
    }
    else //If the file does not exist, print error code
        return LoadFileNotExist;

    /*print success*/
    fout << "=====LOAD======" << endl;
    fout << "Success" << endl;
    fout << "===============" << endl << endl;
    return Success;
}

Result Manager::LOADREPORT(const char* filepath)    //load reportdata file
{
    ifstream fin;
    fin.open(filepath); //file open
    if (!fin)   //If the file does not exist, print error code
        return FaildtoUpdatePath;
    
    /*print success*/
    fout << "=====LOADREPORT======" << endl;
    fout << "Success" << endl;
    fout << "====================" << endl << endl;
    return Success;

}
/// <summary>
/// print out the graph as matrix form
/// </summary>
///
/// <returns>
/// Result::Success if the printing is successful
/// Result::GraphNotExist if there is no graph
/// </returns>
Result Manager::Print()
{
    if(m_graph.Empty()) //if graph is empty, print error
        return GraphNotExist;

    /*if graph is not empty, print graph*/
    m_graph.Print(fout);
    return Success;
}
/// <summary>
/// find the path from startVertexKey to endVertexKey with DFS 
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindPathBfs(int startVertexKey, int endVertexKey)
{
    int size = m_graph.Size();
    if (m_graph.Empty())    //if graph is not exist, print error code
        return GraphNotExist;

    vector<int> path = m_graph.FindPathBfs(startVertexKey, endVertexKey);
    if (m_graph.IsNegativeEdge()) //If have a negative weight, invalid algorithm
        fout << "=====InvalidAlgorithm=====" << endl;
    else
        fout << "==========BFS==========" << endl;  //do not have a negative weight
    fout << "shortest path : ";
    /*print shortest path*/
    for (int i = 0; i < (signed)(path.size() - 1); i++)
        fout << path[i] << " ";

    fout << endl;
    fout << "path length : ";
    /*print path length*/
    fout << path.back() << endl;

    fout << "Course : ";
	
    ifstream read;
    read.open("mapdata.txt");   //open mapdata file
    if (read.good())
    {
	int size=m_graph.Size();
        char data[200];
        int index=0;
	string*company=new string[size];	//store company's name
        while (read.getline(data, sizeof(data)))
        {
            if (data == NULL)	//if File is empty, print error code
                return LoadFileNotExist;
            else
            {
                char* tmp = strtok(data, "/");  // '/'Divide strings based on the criteria
                company[index]=tmp;	//store company's name
		index++;
	    }
	}
	/*print course*/
	for(int i=0;i<path.size()-1;i++)
		for(int j=0;j<size;j++)
			if(path[i]==j)	//if same to shortest path vetex, print account
			{
				fout<<company[j]<<" ";
				break;
			}
	delete[] company;
    }
    else
	    return LoadFileNotExist;
    fout<<endl;
    fout << "===================" << endl << endl;

    if (m_graph.IsNegativeEdge())   //If have a negative weight, print the error code
        return InvalidAlgorithm;

    return Success;
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Dijkstra using std::set
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::InvalidAlgorithm if an exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathDijkstraUsingSet(int startVertexKey, int endVertexKey)
{
    if (m_graph.Empty())    //if graph is not exist, print error code
        return GraphNotExist;

    vector<int> path = m_graph.FindShortestPathDijkstraUsingSet(startVertexKey,endVertexKey);
    
    if (m_graph.IsNegativeEdge())   //If have a negative weight, invalid algorithm
        fout << "======InvalidAlgorithm======" << endl;
    else  //do not have a negative weight
        fout << "========DIJKSTRA========" << endl;
    fout << "shortest path : ";
    /*print shortest path*/
    for (int i = 0; i < (signed)(path.size() - 1); i++)
        fout << path[i] << " ";

    fout << endl;
    fout << "path length : ";
    /*print path length*/
    fout << path.back() << endl;

    fout << "Course : ";

    ifstream read;
    read.open("mapdata.txt");   //open mapdata file
    if (read.good())
    {
        char data[200];
        int index=0;
	int size=m_graph.Size();
	string*company=new string[size];	//store company's name
        while (read.getline(data, sizeof(data)))
        {
            if (data == NULL)	//if File is empty, print error code
                return LoadFileNotExist;
            else
            {
                char* tmp = strtok(data, "/");  // '/'Divide strings based on the criteria
                company[index]=tmp;	//store company's name
		index++;
	    }
	}

	/*print course*/
        for (int i = 0; i < (signed)(path.size() - 1); i++)
                    for(int j=0;j<size;j++)
			    if(path[i]==j)	//if same to shortest path vertex, print account
			    {
				    fout<<company[j]<<" ";
				    break;
			    }
	delete[] company;
    }
    else
	    return LoadFileNotExist;
    fout<<endl;
    fout << "===================" << endl << endl;
    
    if (m_graph.IsNegativeEdge())   //If have a negative weight, print the error code
        return InvalidAlgorithm;

    return Success;
}
/// <summary>
/// find the shortest path from startVertexKey to endVertexKey with Bellman-Ford
/// </summary>
///
/// <param name="startVertexKey">
/// the start vertex key
/// </param>
/// <param name="endVertexKey">
/// the end vertex key
/// </param>
///
/// <returns>
/// Result::InvalidVertexKey or Result::GraphNotExist or Result::NegativeCycleDetected if exception has occurred.
/// Result::Success otherwise.
/// </returns>
Result Manager::FindShortestPathBellmanFord(int startVertexKey, int endVertexKey)
{
    if (m_graph.Empty())    //if graph is empty, print error code
        return GraphNotExist;

    vector<int>path = m_graph.FindShortestPathBellmanFord(startVertexKey,endVertexKey);
    if (path.empty())   //if occurs negative cycle, print error code
        return NegativeCycleDetected;

    fout << "========BELLMANFORD========" << endl;
    fout << "shortest path : ";
    /*print shortes path*/
    for (int i = 0; i < (signed)(path.size() - 1); i++)
        fout << path[i] << " ";

    fout << endl;
    fout << "path length : ";
    /*print path length*/
    fout << path.back() << endl;

    fout << "Course : ";

    ifstream read;
    read.open("mapdata.txt");   //open mapdata file
    if (read.good())
    {
        char data[200];
        int index = 0;
	int size=m_graph.Size();
	string*company=new string[size];	//sotre company's name

        while (read.getline(data, sizeof(data)))
        {
            if (data == NULL)	//if File is empty, print error code
                return LoadFileNotExist;
            else
            {
                char* tmp = strtok(data, "/");  // '/'Divide strings based on the criteria
                company[index]=tmp;	//store company's name
		index++;
	    }
	}

	/*print course*/
	for (int i = 0; i < (signed)(path.size() - 1); i++)
		for(int j=0;j<size;j++)
			if(path[i]==j)	//if same to shortest path vetex, print account
			{
				fout<<company[j]<<" ";
				break;
			}
	delete[] company;
    }
    else
	    return LoadFileNotExist;
    
    fout<<endl;
    fout << "===================" << endl << endl;

    return Success;
}

Result Manager::FindShortestPathFloyd()
{
    if (m_graph.Empty())    //if graph is empty, print error code
        return GraphNotExist;

    int size = m_graph.Size();
    vector < vector <int> >v = m_graph.FindShortestPathFloyd();

    if (v.empty())  //if occurs negative cycle, print error code
        return NegativeCycleDetected;

    /*print success Floyd result*/
    fout << "======FLOYD======" << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            fout << v[i][j] << " ";
        }
        fout << endl;
    }
    fout << "=================" << endl << endl;

    return Success;
}

Result Manager::RabinKarpCompare(const char* CompareString,const char* ComparedString)
{
    enum Result result = StringDoesNotMatch;
    
    /*Change to string*/
    string compared(ComparedString);
    string compare(CompareString);
 
    int comparedSize = compared.size(); //compared string size
    int compareSize = compare.size();   //compare string size

    if (compareSize > 10)   //if compare string size over 10 letters, print error code
        return InvalidOptionNumber;

    int comparedHash = 0, compareHash = 0, power = 1;   //Initialization hash value
    int i=0;	//check compared string's start index
    
    /*Unify and compare in capital letters*/
    for (int i = 0; i < comparedSize; i++)
    {
        if (compared[i] >= 97 && compared[i] <= 122)   //if it's lower case
            compared[i] -= 32;    //Change upper case
    }
    for (int i = 0; i < compareSize; i++)
    {
        if (compare[i] >= 97 && compare[i] <= 122) //if it's lower case
            compare[i] -= 32;   //Change to upper case
    }

    /*Calculate compare string's Hash*/
    for(i=compareSize-1;i>=0;i--)
    {
	    compareHash=compareHash+compare[i]*power;
	    power*=2;
    }

    /*Calculate compared string's Hash*/
    for(i=0;i<=comparedSize-compareSize;i++)
    {
	    comparedHash=0;	//initialization
	    power=1;
	    for(int j=compareSize-1;j>=0;j--)
	    {
		    comparedHash=comparedHash+compared[j+i]*power;
		    power*=2;
	    }
    

	    if(comparedHash==compareHash)	//equal Hash value 
	    {
		    bool find=true;
		    for(int j=0;j<compareSize;j++)
		    {
			    if(compare[j]!=compared[i])
			    {	//if even one character is different,
				    find=false;	//String mismatch
				    break;	//finish Rabinkarp algorithm
			    }
			    i++;	//compare next character
		    }
		    if(find)	//string match
		    {
			    if(a==0)	//if the RABINKARP command started, print
			    {
				    fout<<"=====RABINKARP====="<<endl;
				    fout<<"DUPLICATE TITLE EXISTS"<<endl;
			    }
			    result=Success;
			    fout<<ComparedString<<endl;
			    a++;
			    break;	//finish Rabinkarp algorithm
		    }
  	  }
    }
    return result;
}

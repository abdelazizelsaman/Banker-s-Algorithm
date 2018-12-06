#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int P,R;
int total_resources[10];
int maxm[10][10];
int alloc[10][10];
int need[10][10];
int total_allocation[10] = {0};
int available[10];
bool finish[10] = {false};
int work[10];
int counter = 0;

void randMax (void){
    int instance; // no of free instances remaining for each resource
    for(int i=0 ; i<10 ; i++){
        instance = total_resources[i];
        for(int j=0 ; j<10 ; j++){
                maxm[i][j] = 10;
                while(maxm[i][j] > instance){
                    maxm[i][j] = rand()%10;
                }
                instance -= maxm[i][j];
        }
    }
}

void calcNeed()
{
    for (int i = 0 ; i < P ; i++) //calculate need
    {
        for (int j = 0 ; j < R ; j++)
        {
             need[i][j] = maxm[i][j] - alloc[i][j];
        }
    }
}

void printNeed()
{
    cout << "Need matrix : " << endl;
    for (int i = 0 ; i < P ; i++) //print need
    {
        for (int j = 0 ; j < R ; j++)
        {
             cout << need[i][j] << " ";
        }
        cout << endl;
    }
}

void calcAvailable()
{
    for (int i = 0 ; i < P ; i++) //calculating total allocated resources so we can calculate available
    {
        for (int j = 0 ; j < R ; j++)
        {
            total_allocation[j] += alloc[i][j];
        }
    }

    for (int i = 0 ; i < R ; i++) //calculate available
    {
        available[i] = total_resources[i] - total_allocation[i];
    }
}

void printAvailable()
{
    cout << "Available array : ";
    for (int i = 0 ; i < R ; i++) //print available
    {
        cout << available[i] << " ";
    }
    cout << endl;
    cout << endl;
}

void calcWork()
{
    for (int i = 0 ; i < R ; i++) //work = available
    {
        work[i] = available[i];
    }
}

void printWork()
{
    cout << "Work array : ";
    for (int i = 0 ; i < R ; i++) //print work
    {
        cout << work[i] << " ";
    }
    cout << endl;
    cout << endl;
}

void printMaxm()
{
    cout << "Maximum allocation matrix : " << endl;
    for (int i = 0 ; i < P ; i++)
    {
        for (int j = 0 ; j < R ; j++)
        {
            cout << maxm[i][j] << " ";
        }
        cout << endl;
    }
}

void printAlloc()
{
    cout << "Allocation matrix : " << endl;
    for (int i = 0 ; i < P ; i++)
    {
        for (int j = 0 ; j < R ; j++)
        {
            cout << alloc[i][j] << " ";
        }
        cout << endl;
    }
}

bool isSafe() //safety algorithm
{
    counter = P;
    bool safe;
    calcWork(); //recalculate work from available after resource request has been made and available array has been changed
    for (int i = 0 ; i < P ; i++) //return finish to false so we can request resources
    {                             //and recheck system safety after resource request has been made
        finish[i] = false;
    }
    while (counter != 0) //safety algorithm
    {
        safe = false;
        for (int i = 0 ; i < P ; i++)
        {
            bool flag = true;
            for (int j = 0 ; j < R ; j++) //loop on the whole row to check if any element need[i][j] is larger than work[j] (comparing need rows to work array)
            {
                if (finish[i] == false && need[i][j] > work[j])
                {
                    flag = false;
                }
            }
            for (int j = 0 ; j < R ; j++)
            {
                if (finish[i] == false && flag == true)
                {
                    cout << "P" << i << " is in safe sequence" << endl;
                    for (int k = 0 ; k < R ; k++)
                    {
                        work[k] += alloc[i][k];
                    }
                    finish[i] = true;
                    safe = true;
                    counter--;
                }

                break; //We already checked the whole row before so we don't have to check it again
            }
        }
        if (!safe)
        {
            cout << "System is not in safe state" << endl;
            cout << endl;
            break;
        }
    } //while (counter != 0)

    return safe;
}

void requestResource() // Resource Request Algorithm
{
    int requests[10];
    bool requestState[3] = {false};
	for (int i = 0 ; i < P ; i++)
    {
        cout << "P" << i << " is making a request" << endl;
        for (int j = 0 ; j < R ; j++) //each ith loop we enter the a request array for the ith process
        {
            cin >> requests[j];
            //requests[j] = ((rand()%maxm[i][j]) + 1); //random in factor of maxm[i][j]
        }

        bool flag = true; //flag to check on whole row of need[i][j] if flag is true then whole row of need[i] is indeed larger than request
        for (int j = 0 ; j < R ; j++)
        {
            if (requests[j] > need[i][j])
            {
                flag = false;
            }
        }

        for (int j = 0 ; j < R ; j++)
        {
            if(flag == true)
            {
                if(requests[j] <= available[i])
                {
                    available[j] -= requests[j];
                    alloc[i][j] += requests[j];
                    need[i][j] -= requests[j];

                    requestState[0] = true;
                    requestState[1] = false;
                    requestState[2] = false;
                }
                else
                {
                    requestState[0] = false;
                    requestState[1] = true;
                    requestState[2] = false;
                }
            }

            else //flag = false;
            {
                requestState[0] = false;
                requestState[1] = false;
                requestState[2] = true;
            }
        }

        if (requestState[0])
        {
            if (isSafe())
            {
                cout << "System is safe" << endl;
                cout << "Request for P" << i << " has been accepted" << endl;
            }
            else
            {
                for (int j = 0 ; j < R ; j++)
                {
                    available[j] += requests[j];
                    alloc[i][j] -= requests[j];
                    need[i][j] += requests[j];
                }
                cout << "System is unsafe" << endl;
                cout << "Request for P" << i << " has been denied because system will not be safe" << endl;
            }
        }
        else if(requestState[1])
        {
            cout << "P" << i << "should wait, current resources are not available" << endl;
        }
        else
        {
            cout << "Error! Allocation limit reached, can't allocate more than maximum!" << endl;
        }
    }
}

void releaseResource() //release function
{
    int release[10] = {0};
    for (int i = 0 ; i < P ; i++)
    {
        cout << "P" << i << " is releasing resources" << endl;
        for (int j = 0 ; j < R ; j++)
        {
            cin >> release[j];
            //release[j] = ((rand()%alloc[i][j]) + 1); //random in factor of alloc[i][j]
        }
        int x = 0;
        while(x != 8000000) //delay to release resources after that period of time
        {
            x++;
        }
        for (int j = 0 ; j < R ; j++)
        {
            available[j] += release[j];
            alloc[i][j] -= release[j];
            need[i][j] += release[j];
        }

        if (isSafe())
        {
            cout << "Released resources of Process " << i << " are : " << endl;
            for (int j = 0 ; j < R ; j++)
            {
                cout << release[j] << " ";
            }
            cout << endl;
            cout << "System is safe, Resources have been released" << endl;
        }
        else
        {
            for (int j = 0 ; j < R ; j++)
            {
                available[j] -= release[j];
                alloc[i][j] += release[j];
                need[i][j] -= release[j];
            }
            cout << "Resources P" << i << " requested to release are : ";
            for (int j = 0 ; j < R ; j++)
            {
                cout << release[j] << " ";
            }
            cout << endl;
            cout << "System is unsafe, Resources cannot be released" << endl;
        }
    }
}

int main()
{
    cout << "Enter number of processes" << endl;
    cin >> P;
    cout << "Enter number of resources" << endl;
    cin >> R;
    cout << "Enter number of instances of each resource" << endl;
    for (int i = 0 ; i < R ; i++)
    {
        cin >> total_resources[i];
    }
    randMax(); // generate random max allocation need for each process based on total instances provided by user

//    cout << "Enter maximum matrix (maximum resources that could be allocated to each process)";
//    for (int i = 0 ; i < P ; i++)
//    {
//        for (int j = 0 ; j < R ; j++)
//        {
//            cin >> maxm[i][j];
//        }
//    }

    cout << "Printing the randomly generated Maximum value of instances for each process"<<endl;
    for(int i=0; i<10 ; i++){
        for( int j=0 ; j<10 ; j++){
            cout << maxm[i][j] << " " ;
        }
        cout <<endl;
    }
    cout << "Enter allocation matrix (resources that could be allocated to each process)" << endl;
    for (int i = 0 ; i < P ; i++)
    {
        for (int j = 0 ; j < R ; j++)
        {
            cin >> alloc[i][j];
        }
    }

    calcNeed(); //calculate need
    printNeed(); //print need

    calcAvailable(); //calculate available
    printAvailable(); //print available

    calcWork(); //work = available;
    printWork(); //print work

    if (isSafe()) //does the safety algorithm and checks safety
    {
        cout << "System is safe" << endl;
        cout << endl;
    }

    while(1)
    {
        int x;
        requestResource();
        releaseResource();
        cout << "Do you want to request more resources?    1.Yes  0.No" << endl;
        cin >> x;
        if (x == 0)
        {
            break;
        }
    }
} //main

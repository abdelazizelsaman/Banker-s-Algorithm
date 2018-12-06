#include <iostream>
using namespace std;

int main()
{
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

    cout << "Enter number of processes" << endl;
    cin >> P;
    cout << "Enter number of resources" << endl;
    cin >> R;
    cout << "Enter number of instances of each resource" << endl;
    for (int i = 0 ; i < R ; i++)
    {
        cin >> total_resources[i];
    }

    cout << "Enter maximum matrix (maximum resources that could be allocated to each process)";
    for (int i = 0 ; i < P ; i++)
    {
        for (int j = 0 ; j < R ; j++)
        {
            cin >> maxm[i][j];
        }
    }

    cout << "Enter allocation matrix (resources that could be allocated to each process)" << endl;
    for (int i = 0 ; i < P ; i++)
    {
        for (int j = 0 ; j < R ; j++)
        {
            cin >> alloc[i][j];
        }
    }

    for (int i = 0 ; i < P ; i++) //calculate need
    {
        for (int j = 0 ; j < R ; j++)
        {
             need[i][j] = maxm[i][j] - alloc[i][j];
        }
    }

    cout << "Need matrix : " << endl;

    for (int i = 0 ; i < P ; i++) //print need
    {
        for (int j = 0 ; j < R ; j++)
        {
             cout << need[i][j] << " ";
        }
        cout << endl;
    }

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

    cout << "Available array : " << endl;
    for (int i = 0 ; i < R ; i++) //print available
    {
        cout << available[i] << " ";
    }
    cout << endl;

    for (int i = 0 ; i < R ; i++) //work = available
    {
        work[i] = available[i];
    }

    cout << "Work array : " << endl;
    for (int i = 0 ; i < R ; i++) //print work
    {
        cout << work[i] << " ";
    }
    cout << endl;

    counter = P;
    bool safe;
    //LOOP:
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
                    cout << "Process P" << i << " is in safe sequence" << endl;
                    for (int k = 0 ; k < R ; k++)
                    {
                        work[k] += alloc[i][k];
                    }
                    for (int k = 0 ; k < R ; k++)
                    {
                        cout << work[k] << " ";
                    }
                    cout << endl;
                    finish[i] = true;
                    safe = true;
                    counter--;
                }
                else
                {
                    break;
                }
            }
        }
        if (!safe)
        {
            cout << "System is not in safe state" << endl;
            break;
        }
    } //safety algorithm
    if (safe)
    {
        cout << "System is safe" << endl;
    }//safety algorithm

    /*for (int i = 0 ; i < P ; i++) //if not all processes are in safe sequence we go back to the loop
    {
        if (finish[i] == false)
            goto LOOP;
    }*/


    // Resource Request Algorithm
	for (int i = 0 ; i < P ; i++)
    {
        int requests[10];
        for (int k = 0 ; k < R ; k++) //each ith loop we enter the ith request of the ith process
        {
            cin >> requests[k];
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

                    cout << "Request " << i << " accepted" << endl;
                }
                else
                {
                    cout << "Process " << i << "should wait, current resources are not available" << endl;
                }
            }
            else
                {
                    cout << "Error" << endl;
                    cout << "Allocations limit reached" << endl;
                }

        }
    }
} //main


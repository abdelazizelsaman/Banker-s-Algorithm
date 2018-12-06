while (counter != 0)
{
    safe = 0;
    for (i = 0; i < processes; i++)
    {
        if (running[i])
        {
            exec = 1;
            for (j = 0; j < resources; j++)
            {
                if (maximum_claim[i][j] - current[i][j] > available[j])
                {
                    exec = 0;
                    break;
                }
            }
            if (exec)
            {
                printf("\nProcess%d is executing\n", i + 1);
                running[i] = 0;
                counter--;
                safe = 1;

                for (j = 0; j < resources; j++)
                {
                    available[j] += current[i][j];
                }

                break;
            }
        }
    }
    if (!safe)
    {
        printf("\nThe processes are in unsafe state.\n");
        break;
    }
    else
    {
        printf("\nThe process is in safe state");
        printf("\nAvailable vector:");

        for (i = 0; i < resources; i++)
        {
            printf("\t%d", available[i]);
        }

        printf("\n");
    }
}

# Unix-system-monitoring-tool

Unix system monitoring tool that reports various information such as RAM usage, CPU usage, logged in users, and more. Here are the command line arguments it accepts:

--system
        to indicate that only the system usage should be generated.


--user

        to indicate that only the users usage should be generated.



--sequential

        to indicate that the information will be output sequentially without needing to "refresh" the screen (useful if you would like to redirect the output into a file).

 

--samples=N

        if used the value N will indicate how many times the statistics are going to be collected and results will be average and reported based on the N number of repetitions.
If not value is indicated the default value will be 10.


--tdelay=T

        to indicate how frequently to sample in seconds.
If not value is indicated the default value will be 1 sec.

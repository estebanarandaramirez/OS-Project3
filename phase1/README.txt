OS - PROJECT 3 
Esteban Aranda (earandaramirez) & Nicholas Hollan (nchollan)
------------------------------------------------------------
Phase 1:
Simulation of costume shop with pirate and ninja customers who cannot come in to the store at the same time. 
To compile the project type "make" when in the appropriate directory. To run the program, the user has to input "./shop #costumingteams #pirates #ninjas costumingTimePirates costumingTimeNinjas arrivalTimePirates arrivalTimeNinjas". For example:
./shop 3 20 12 15 5 10 10
To clean the project type "make clean".

For testing, once the program is built open "test_inputs.txt" and copy one line at a time into the command line to run the program with those specific inputs.

The known defects that this part of the project has is printing the stats. Specifically, the time each costuming team was busy is not accurate as is the average length of the queue. Other than that, we believe that the program works properly. As a side note, testing was done with time values in ms instead of seconds so we do not know if some things may differ when running it in seconds.

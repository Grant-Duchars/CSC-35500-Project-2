// Author: Grant Duchars
#include "AirportAnimator.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/sem.h>

// Runway, updating passengers, updating tours
const int NUM_SEMAPHORES = 3;
const int PLANE_CAPACITY = 12;
int passengersAtAirport;
int toursTotal;
int toursCompleted;

void *doAirplane(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "\033[1mproject2: \033[31merror:\033[0m usage is ./project2 <TOTAL PASSENGERS> <TOTAL TOURS>\n");
        return 1;
    }
    passengersAtAirport = atoi(argv[1]);
    toursTotal = atoi(argv[2]);

    pthread_t tid0; // tid1, tid2, tid3, tid4, tid5, tid6, tid7;
    pthread_create(&tid0, NULL, &doAirplane, (void *)1);
    pthread_join(tid0, NULL);
    return 0;
}

void *doAirplane(void *arg)
{
    int planeNum = (int)arg;
    while (toursCompleted < toursTotal)
    {
        // Board Passengers
        int passengersOnBoard = 0;
        while (passengersOnBoard != PLANE_CAPACITY)
        {
            // TODO: protect this
            --passengersAtAirport;
            AirportAnimator::updatePassengers(planeNum, ++passengersOnBoard);
            sleep(rand() % 3);
        }

        // Taxi Out
        AirportAnimator::taxiOut(planeNum);

        // Takeoff
        // TODO: protect this
        AirportAnimator::takeoff(planeNum);

        // Tour
        sleep(15 + rand() % 31);

        // Land
        AirportAnimator::land(planeNum);

        // Taxi In
        AirportAnimator::taxiIn(planeNum);

        // Deboard Passengers
        while (passengersOnBoard != 0)
        {
            // TODO: Protect this
            AirportAnimator::updatePassengers(planeNum, --passengersOnBoard);
            ++passengersAtAirport;
            sleep(1);
        }

        // Update tours
        // TODO: protect this
        AirportAnimator::updateTours(++toursCompleted);
    }
    pthread_exit(NULL);
}
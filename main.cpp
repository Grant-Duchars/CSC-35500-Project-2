// Author: Grant Duchars
#include "AirportAnimator.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

const int PLANE_CAPACITY = 12;
int passengersAtAirport;
int toursTotal;
int toursCompleted;
// Runway Sem, Passenger Sem, Tours Sem
sem_t sem_ids[3];

void *doAirplane(void *arg);

int main(int argc, char *argv[])
{
    // Check if arguments were passed in
    if (argc != 3)
    {
        fprintf(stderr, "\033[1mproject2: \033[31merror:\033[0m usage is ./project2 <TOTAL PASSENGERS> <TOTAL TOURS>\n");
        return 1;
    }
    // Set number of total passengers and tours to complete
    passengersAtAirport = atoi(argv[1]);
    toursTotal = atoi(argv[2]);

    // Initialize the three semaphores
    for (int i = 0; i < 3; ++i)
    {
        sem_init(&sem_ids[i], 0, 1);
    }

    // "Let there be airport"
    AirportAnimator::init();

    // Create the eight threads to run the eight airplanes
    pthread_t thread_ids[8];
    for (int i = 0; i < 8; ++i)
    {
        int *thread_num = new int(i);
        pthread_create(&thread_ids[i], NULL, doAirplane, (void *)thread_num);
    }

    // Wait on the eight threads for them to finish their tours
    for (int i = 0; i < 8; ++i)
    {
        pthread_join(thread_ids[i], NULL);
    }

    // Finish up
    AirportAnimator::end();
    printf("Airport successfully animated\n");
    return 0;
}

void *doAirplane(void *arg)
{
    // Which plane this thread is controlling
    int planeNum = (int)arg;
    while (toursCompleted < toursTotal)
    {
        // Board Passengers
        int passengersOnBoard = 0;
        while (passengersOnBoard != PLANE_CAPACITY)
        {
            // TODO: protect this
            sem_wait(&sem_ids[1]);
            --passengersAtAirport;
            ++passengersOnBoard;
            AirportAnimator::updatePassengers(planeNum, passengersOnBoard);
            sem_post(&sem_ids[1]);
            sleep(rand() % 3);
        }

        // Taxi Out
        AirportAnimator::taxiOut(planeNum);

        // Takeoff
        sem_wait(&sem_ids[0]);
        AirportAnimator::takeoff(planeNum);
        sem_post(&sem_ids[0]);

        // Tour
        sleep(15 + rand() % 31);

        // Land
        sem_wait(&sem_ids[0]);
        AirportAnimator::land(planeNum);
        sem_post(&sem_ids[0]);

        // Taxi In
        AirportAnimator::taxiIn(planeNum);

        // Deboard Passengers
        while (passengersOnBoard != 0)
        {
            sem_wait(&sem_ids[1]);
            --passengersOnBoard;
            AirportAnimator::updatePassengers(planeNum, passengersOnBoard);
            ++passengersAtAirport;
            sem_post(&sem_ids[1]);
            sleep(1);
        }

        // Update tours
        sem_wait(&sem_ids[2]);
        AirportAnimator::updateTours(++toursCompleted);
        sem_post(&sem_ids[2]);
    }
    pthread_exit(NULL);
}
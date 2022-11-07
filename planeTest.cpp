#include "AirportAnimator.hpp"

#include <unistd.h>

#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  unsigned int planeNum;
  if (!(argc == 2 && (planeNum = atoi(argv[1])) < 8))
  {
    cerr << "Plane number not properly specified.";
    srand(time(NULL));
    planeNum = rand() % 8;
    cerr << " Set to:" << planeNum << endl;
    sleep(3);
  }

  cerr << "Plane:" << planeNum << endl;
  cerr.flush();
  sleep(3);

  AirportAnimator::init();

  for (int passenger = 1; passenger <= 12; passenger++)
  {
    AirportAnimator::updatePassengers(planeNum, passenger);
    sleep(1);
  }

  AirportAnimator::updateStatus(planeNum, "TAXI");
  AirportAnimator::taxiOut(planeNum);

  AirportAnimator::updateStatus(planeNum, "TKOFF");
  AirportAnimator::takeoff(planeNum);

  AirportAnimator::updateStatus(planeNum, "TOUR");
  sleep(10);

  AirportAnimator::updateStatus(planeNum, "LNDRQ");
  sleep(2);

  AirportAnimator::updateStatus(planeNum, "LAND");
  AirportAnimator::land(planeNum);

  AirportAnimator::updateStatus(planeNum, "TAXI");
  AirportAnimator::taxiIn(planeNum);

  AirportAnimator::updateStatus(planeNum, "DEPLN");
  for (int passenger = 11; passenger >= 0; passenger--)
  {
    AirportAnimator::updatePassengers(planeNum, passenger);
    sleep(1);
  }

  AirportAnimator::updateStatus(planeNum, "DEPLN");

  AirportAnimator::updateTours(1);
  sleep(2);

  AirportAnimator::end();
}

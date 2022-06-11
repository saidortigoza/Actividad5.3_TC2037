//Integrantes:
/*
Miguel Weiping Tang Feng A01610836
Said Ortigoza Trujilla A01707430
*/

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <pthread.h>


const int MAXBabRope = 3;
const int MAXBaboons = 10;

pthread_mutex_t mutexRope;
pthread_cond_t condRopeRight;
pthread_cond_t condRopeLeft;

int numBabRope = 0;

int leftBaboonsCrossed = 0;
int rightBaboonsCrossed = 0;

int leftBabRope = 0;
int rightBabRope = 0;

using namespace std;

void* leftBabCross(void* arg) {
	pthread_mutex_lock(&mutexRope);
	while (leftBabRope == MAXBabRope) {
		printf("----Left Baboon Waiting----\n");
		pthread_cond_wait(&condRopeRight, &mutexRope);
	}	
	leftBabRope++;
	leftBaboonsCrossed++;
	printf("Left baboon crossing... | ");
	printf("Num Left Baboons in Rope: %d | ", leftBabRope);
	printf("Total Left Baboons Crossed: %d\n", leftBaboonsCrossed);

	if (leftBabRope == MAXBabRope || leftBaboonsCrossed == MAXBaboons) {
		rightBabRope = 0;
		pthread_mutex_unlock(&mutexRope);
		pthread_cond_broadcast(&condRopeLeft);
	}
	else {
		pthread_mutex_unlock(&mutexRope);
		pthread_cond_broadcast(&condRopeLeft);
	}
	pthread_exit(NULL);
}

void* rightBabCross(void* arg) {
	pthread_mutex_lock(&mutexRope);
	while (leftBabRope < MAXBabRope) {
		if (leftBaboonsCrossed < MAXBaboons) {
			printf("----Right Baboon Waiting----\n");
			pthread_cond_wait(&condRopeLeft, &mutexRope);
		} 
		else {
			break;
		}
	}
	rightBabRope++;
	rightBaboonsCrossed++;
	printf("Right baboon crossing... | ");
	printf("Num Right Baboons in Rope: %d | ", rightBabRope);
	printf("Total Right Baboons Crossed: %d\n", rightBaboonsCrossed);
	if (rightBabRope == MAXBabRope) {
		leftBabRope = 0;
		pthread_mutex_unlock(&mutexRope);
		pthread_cond_broadcast(&condRopeRight);
	}
	else {
		pthread_mutex_unlock(&mutexRope);
		pthread_cond_broadcast(&condRopeRight);
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
	int i;
	pthread_t baboons[MAXBaboons];
	pthread_mutex_init(&mutexRope, NULL);
	pthread_cond_init(&condRopeRight, NULL);
	pthread_cond_init(&condRopeLeft, NULL);

	for (i = 0; i < MAXBaboons; i++) {
		if (pthread_create(&baboons[i], NULL, leftBabCross, NULL) != 0) {
			perror("Failed to create thread Baboons Left");
		}
	}
	for (i = 0; i < MAXBaboons; i++) {
		if (pthread_create(&baboons[i], NULL, rightBabCross, NULL) != 0) {
			perror("Failed to create thread Baboons right");
		}
	}
	for (i = 0; i < MAXBaboons; i++) {
		if (pthread_join(baboons[i], NULL) != 0) {
			perror("Failed to join threads");
		}
	}

	printf("Total of Left Baboons that crossed the rope: %d\n", leftBaboonsCrossed);
	printf("Total of Right Baboons that crossed the rope: %d\n", rightBaboonsCrossed);

	pthread_mutex_destroy(&mutexRope);
	pthread_cond_destroy(&condRopeRight);
	pthread_cond_destroy(&condRopeLeft);

	return 0;
}
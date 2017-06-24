/*   Azeez Saba
    112407605
    asaba1
*/

#include <stdio.h>
#include <math.h>

static int weights_correct(int array[], int size);
static double compute_numeric(int scores[], int weights[], int days_late[], int penalty_points, 
	int drops_allowed, int size);
static double compute_mean(int scores[], int weights[], int days_late[], int penalty_points, int number_of_assignments);
static double compute_deviation(double calculated_mean, int scores[], int weights[], int days_late[], int penalty_points,
 int number_of_assignments);

int main() {

	int penalty_points, drops_allowed, number_of_assignments;
	char stats;
	int assignment_number, current_score, current_weight, current_days_late;
	int i;

	/*create associative arrays*/
	int test_scores[50] = {0}; /*array changes based on whats dropped*/
	int scores[50]; /*stays the same*/
	int weights[50];
	int days_late[50];


	scanf("%d %d %c", &penalty_points, &drops_allowed, &stats);
	scanf("%d", &number_of_assignments);

	
	/*assigns assignment values to appropriate indeces*/
	
	for (i = 0; i < number_of_assignments; i++) {
		scanf("%d, %d, %d, %d", &assignment_number, &current_score,
			&current_weight, &current_days_late);
		/*takes into account the reduced score if there were late assignments*/
		test_scores[assignment_number - 1] = current_score;
		scores[assignment_number - 1] = current_score;
		weights[assignment_number - 1] = current_weight;
		days_late[assignment_number - 1] = current_days_late;
	}

	/*check if the weights add up to 100
	  terminate program if they don't*/
	if (!weights_correct(weights, number_of_assignments)) {
		return 1;
	}

	/*prints Numeric Score*/
	printf("Numeric Score: %5.4f\n", compute_numeric(test_scores, weights, days_late, penalty_points,
		drops_allowed, number_of_assignments));

	/*prints Points Penalty Per Day Late*/
	printf("Points Penalty Per Day Late: %d\n", penalty_points);

	/*prints Number of Assignments Dropped*/
	printf("Number of Assignments Dropped: %d\n", drops_allowed);
 
	printf("Values Provided:\n");

	/*prints Assignment, Score, Weight, Days Late*/
	printf("Assignment, Score, Weight, Days Late\n");
	for (i = 0; i < number_of_assignments; i++) {
		printf("%d, %d, %d, %d", i + 1, scores[i], weights[i], days_late[i]);
		if (i < number_of_assignments) {
			printf("\n");
		}
	}

	/*prints stats if the user wished for it*/
	if (stats == 'Y' || stats == 'y') {
		double calculated_mean = compute_mean(scores, weights, days_late, penalty_points, number_of_assignments);
		printf("Mean: %5.4f", calculated_mean);
		printf(", Standard Deviation: %5.4f", compute_deviation(calculated_mean, scores, weights, days_late,
			penalty_points, number_of_assignments));
		printf("\n");
	}

	return 1;

}

/*computes numeric value*/
static double compute_numeric(int scores[], int weights[], int days_late[], int penalty_points, 
	int drops_allowed, int size) {
	double numeric_score = 0;
	double total_weight;
	double check_score;

	/*begins dropping scores*/
	/*sets dropped scores to 0*/
	int index_of_small_score; /*keeps track of where to drop the lowest score*/
	double smallest = scores[0] * weights[0]/100; /*current lowest score x weight value*/

	/*stays in the loop until it fills the number of dropped scores*/
	int i;
	for (i = 0; i < drops_allowed; i++) {
		int j;
		for (j = 0; j < size; j++) {
			if (scores[j] != 0) {
				if (scores[j] * weights[j]/100 < smallest) {
					smallest = scores[j] * weights[j]/100;
					index_of_small_score = j;
				}
			}
		}
		scores[index_of_small_score] = 0;
		smallest = 10000; /*some arbitrary large number*/
	}

	/*calculate total adjusted weight of all non-dropped scores*/
	
	for (i = 0; i < size; i++) {
		if (scores[i] != 0) {
			total_weight += (double) weights[i]/100;
		}
	}

	/*calculate numeric score from following formula:
	  [current score - (days late x penalty)] x weight/overall weight*/
	
	for(i = 0; i < size - drops_allowed; i++) {
		if (scores[i] != 0) {
			check_score = (double) (scores[i] - (days_late[i] * penalty_points)) * (double) weights[i]/ (double) total_weight;
			if (check_score > 0) {
				numeric_score += check_score;
			} 
		}
	}

	return numeric_score / 100;
}

/*checks to see if the weights add up to 100*/
static int weights_correct(int array[], int size) {
	int i;
	int counter = 0;
	for (i = 0; i < size; i++) {
		counter += array[i];
	}

	if (counter != 100) {
		return 0;
	}

	return 1;
}

/*computes mean*/
static double compute_mean(int scores[], int weights[], int days_late[], int penalty_points, int number_of_assignments) {
	double mean = 0;
	int col;
	for (col = 0; col < number_of_assignments; col++) {
		mean += scores[col] - (penalty_points * days_late[col]);
	}
	mean /= (double) number_of_assignments;

	return mean;
}

/*computes deviation*/
static double compute_deviation(double calculated_mean, int scores[], int weights[], int days_late[], int penalty_points,
 int number_of_assignments) {
 	double deviation = 0;
 	int current_score = 0;
	int col;
	for (col = 0; col < number_of_assignments; col++) {
		current_score = scores[col] - (penalty_points * days_late[col]);
		deviation += ((current_score - calculated_mean) * (current_score - calculated_mean));
	}
	deviation /= (double) number_of_assignments;

	return sqrt(deviation);
}



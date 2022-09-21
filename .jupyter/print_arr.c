
#include <stdio.h>

void print_arr(const char *label, const int arr[], const int len) {
	printf("%-10s ", label);
	for (int i = 0; i < len-1; i++) {
		printf("%d, ", arr[i]);
	}
	printf("%d\n", arr[len-1]);
}
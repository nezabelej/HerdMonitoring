#include "contiki.h"
#include "dev/i2cmaster.h"  // Include IC driver
#include "dev/tmp102.h"     // Include sensor driver
#include "dev/cc2420/cc2420.h"
#include "net/rime/rime.h"
#include "net/rime/mesh.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>

#define TMP102_READ_INTERVAL (CLOCK_SECOND)  // Poll the sensor every second


PROCESS (herd_monitor_gateway, "Herd monitor - gateway");
AUTOSTART_PROCESSES (&herd_monitor_gateway);

static int int_cmp(int a[], int b[]) 
{ 
	int x = a[1];
    int y = b[1];
    if (x < y) return 1;
    else if (x > y) return -1;
    return 0;
} 

static void bsort(int arr[][2]) {
	int n = 5;
	int i,j;
	for (i = 0; i < (n-1); ++i)
	{
		for (j = 0; j < (n-i-1); ++j)
		{
			if (int_cmp(arr[j], arr[j+1]) > 0) {
				int swap1 = arr[j][0];
				int swap2 = arr[j][1];
				arr[j][0] = arr[j+1][0];
				arr[j][1] = arr[j+1][1];
				arr[j+1][0] = swap1;
				arr[j+1][1] = swap2;
			}
		}
	}
	
 
}

PROCESS_THREAD (herd_monitor_gateway, ev, data)
{
	PROCESS_BEGIN();
	{
		int n = 5;
		int i,j;

		int RSSIarray[5][5] = {{1,1,1,1,1},{-4,-2, 0, 1, 0}, {1,1,1,-9,-2}, {-3,-33,-44,-55,-2}, {1,-5,-6,-7,1}};
		int power[n][2];

		//Creating power array. It displays number of neighbours of each node. 
		// power[2][1] --> number of neighbours of 3. node
		// power[2][0] --> 2
		for (i = 0; i < n; i++) {
			power[i][0] = i;
			power[i][1] = 0;
			for (j = 0; j < n; j++) {
				if (RSSIarray[i][j] <= 0) {
					power[i][1]++;
				}
			}
		}

		//Sorting power by number of neighbours. First element is the one with the most neighbours.
		//After sorting: power[0][0] --> node with most neighbour (id = power[0][0] + 1)
		//               power[0][1] --> number of neighbours
		//power[0] --> possible cluster heads
		bsort(power);
		
		for (i = 0; i < n; i++) {
			printf("%d %d\n",power[i][0],power[i][1]);
		}
	}

  PROCESS_END ();
}		


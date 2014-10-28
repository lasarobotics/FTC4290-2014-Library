/**********************************************************

IR Operation
ir.h

Mathematics and drivers required for operation
and functionality of the IR sensor.

**********************************************************/

/**** INCLUDES ****/
#include "../../drivers/hitechnic-irseeker-v2.h" //IR seeker drivers

/**** MOVING AVERAGE ****/
const int samples = 10; // moving average sample size
int indexavg = 0;       // current index
int IR3avg[samples];    // sample list one
int IR4avg[samples];    // sample list two

/**
 * Moving Average
 * @param Last data point of IR 3
 * @param Last data point of IR 4
 * @param Returns current average of IR 3
 * @param Returns current average of IR 4
 */
void moveavg(int last3, int last4, float& avg3, float& avg4)
{
  if (indexavg == samples){
    indexavg = 0;
  }
  IR3avg[indexavg] = last3;
  IR4avg[indexavg] = last4;
  avg3 = 0;
  avg4 = 0;
  for (int i=0; i<samples; i++)
  {
    avg3 += (float)IR3avg[i];
    avg4 += (float)IR4avg[i];
  }
  avg3 = avg3/samples;
  avg4 = avg4/samples;
  indexavg++;
  return;
}

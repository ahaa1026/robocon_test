#include "encodor.h"
extern Encodor encodor;



void encorder_task()
{
    EncodorGetCounter(&encodor);
    EncodorGetPosition(&encodor);
    EncodorGetSpeed(&encodor,1);

}

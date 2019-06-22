#include <math.h>

namespace phygine {
    /**
    * Define a real type to allow the engine to be rapidly compiled in different precisions if we
    * event need to later.
    */
    typedef float real;

    /** Defines the precision of the square root operator. */
#define real_sqrt sqrtf

    /** Defines the precision of the power operator. */
#define real_pow powf
}
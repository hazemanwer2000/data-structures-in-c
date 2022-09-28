
/**
 *  @brief      : The data-type of choice stored in the structures.
 *                  (Note: Should be 'void *' for generality, and for many functions, that depend on other structures.)
**/
#define DATA_TYPE void *

/**
 *  @brief      : The default value of the data-type of choice in the structures.
 *                  This value is returned by indexable, item-returning functions, when the index is out-of-bounds.
 *                  (Note: Should be '0' for generality.)
**/
#define DEFAULT_VALUE 0

/**
 *  @brief      : The data-type of choice for the length of a data structure to be stored in.
 *                  (Note: It defines the maximum length of a data structure.)
 *                  (Note: When negative, it permits negative or reverse indexing.)
**/
#define LENGTH_DT signed long int

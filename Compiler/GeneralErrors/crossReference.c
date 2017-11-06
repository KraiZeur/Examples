// Keywords : -> 
// Cross reference
// Circular dependencies

#ifdef __NO_COMPIL
typedef struct base {
  abstract_t data;
  int id;
} base_t;

typedef struct abstract {
  int value;
} abstract_t;
#endif /* __NO_COMPIL */

// The example above will not compil due to forward declaration one
// way to bypass this problem is to declare the typedef of the abstract struct
// and use data as a pointer (due to unknwon struct size)

#ifdef __COMPIL
typedef struct abstract abstract_t; // forward declaration ( trust me this is a struct )

typedef struct base {
  abstract_t* data;
  int id;
} base_t;

typedef struct abstract {
  int value;
} abstract_t;
#endif /* __COMPIL */

// !!! Warning you have to use a pointer to the structure !!!

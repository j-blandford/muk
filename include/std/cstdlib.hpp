#pragma once
#include <stddef.h>
#include <stdint.h>

// Structs for stdlib
typedef struct {
    short quot, rem;
} div_t;

typedef struct {
    long quot, rem;
} ldiv_t;

namespace std {
  using size_t = size_t;
  using div_t = div_t;
  using ldiv_t = ldiv_t;
}

namespace std {
	
}
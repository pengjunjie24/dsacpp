

#include <Print/Print.hpp>

void UniPrint::p(int e) { printf(" %04d", e); }
void UniPrint::p(float e) { printf(" %4.3f", e); }
void UniPrint::p(double e) { printf(" %4.3f", e); }
void UniPrint::p(char e) { printf(" %c", (31 < e) && (e < 128) ? e : '$'); }
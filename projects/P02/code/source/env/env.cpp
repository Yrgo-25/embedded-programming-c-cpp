/**
 * @note Definitions required to satisfy the linker when using C++ interfaces and static local 
 *       variables in embedded projects. 
 */
void operator delete(void*, unsigned int) noexcept {}
extern "C" void __cxa_pure_virtual() {}
extern "C" int __cxa_guard_acquire (volatile void *g) { return !*(char *)g; }
extern "C" void __cxa_guard_release (volatile void *g) { *(char *)g = 1; }
extern "C" void __cxa_guard_abort (volatile void *) {}
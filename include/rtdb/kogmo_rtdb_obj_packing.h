// im SFB muessen wir insb. fuer die DSpace AutoBox und Simulink unterscheiden..

// dies ist nur die Definition fuer GNUC:
#if !defined(PACKED_struct)
 #define PACKED_struct struct __attribute__((packed))
#endif

#include "nx_api.h"
#if defined(NX_TAHI_ENABLE) && defined(FEATURE_NX_IPV6)  
#include "netx_tahi.h"


#if 0
/* Frame (78 bytes) */
static char pkt1[78] = {
    0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, /* 33.3DV.. */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, /* ...3DV.. */
    0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56              /* "..3DV */
};

/* Frame (78 bytes) */
static char pkt2[78] = {
    0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, /* 33.3DV.. */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, /* ...3DV.. */
    0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56              /* "..3DV */
};

/* Frame (78 bytes) */
static char pkt3[78] = {
    0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, /* 33.3DV.. */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, /* ...3DV.. */
    0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56              /* "..3DV */
};

/* Frame (70 bytes) */
static char pkt4[70] = {
    0x33, 0x33, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x85, 0x00, /* ........ */
    0xad, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};

/* Frame (70 bytes) */
static char pkt5[70] = {
    0x33, 0x33, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x85, 0x00, /* ........ */
    0xad, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};

/* Frame (70 bytes) */
static char pkt6[70] = {
    0x33, 0x33, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x85, 0x00, /* ........ */
    0xad, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};
#endif

/* Frame (118 bytes) */
static char pkt7[118] = {
    0x33, 0x33, 0x00, 0x01, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x40, 0x11, 0xff, 0xfe, 0x80, /* ...@.... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x22, /* ......." */
    0x02, 0x23, 0x00, 0x40, 0x6d, 0x29, 0x01, 0x3e, /* .#.@m).> */
    0xf0, 0x12, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x01, /* ........ */
    0x00, 0x01, 0xac, 0x7d, 0x87, 0x3a, 0x00, 0x11, /* ...}.:.. */
    0x22, 0x33, 0x44, 0x56, 0x00, 0x03, 0x00, 0x0c, /* "3DV.... */
    0xc0, 0xde, 0xdb, 0xad, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, /* ........ */
    0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x17, /* ........ */
    0x00, 0x1f, 0x00, 0x29, 0x00, 0x18              /* ...).. */
    };

#if 0
/* Frame (118 bytes) */
static char pkt8[118] = {
    0x33, 0x33, 0x00, 0x01, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x40, 0x11, 0xff, 0xfe, 0x80, /* ...@.... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x22, /* ......." */
    0x02, 0x23, 0x00, 0x40, 0x6c, 0xc5, 0x01, 0x3e, /* .#.@l..> */
    0xf0, 0x12, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x01, /* ........ */
    0x00, 0x01, 0xac, 0x7d, 0x87, 0x3a, 0x00, 0x11, /* ...}.:.. */
    0x22, 0x33, 0x44, 0x56, 0x00, 0x03, 0x00, 0x0c, /* "3DV.... */
    0xc0, 0xde, 0xdb, 0xad, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, /* ........ */
    0x00, 0x64, 0x00, 0x06, 0x00, 0x08, 0x00, 0x17, /* .d...... */
    0x00, 0x1f, 0x00, 0x29, 0x00, 0x18              /* ...).. */
    };

/* Frame (118 bytes) */
static char pkt9[118] = {
    0x33, 0x33, 0x00, 0x01, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x40, 0x11, 0xff, 0xfe, 0x80, /* ...@.... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x22, /* ......." */
    0x02, 0x23, 0x00, 0x40, 0x6b, 0xfd, 0x01, 0x3e, /* .#.@k..> */
    0xf0, 0x12, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x01, /* ........ */
    0x00, 0x01, 0xac, 0x7d, 0x87, 0x3a, 0x00, 0x11, /* ...}.:.. */
    0x22, 0x33, 0x44, 0x56, 0x00, 0x03, 0x00, 0x0c, /* "3DV.... */
    0xc0, 0xde, 0xdb, 0xad, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, /* ........ */
    0x01, 0x2c, 0x00, 0x06, 0x00, 0x08, 0x00, 0x17, /* .,...... */
    0x00, 0x1f, 0x00, 0x29, 0x00, 0x18              /* ...).. */
    };

/* Frame (118 bytes) */
static char pkt10[118] = {
    0x33, 0x33, 0x00, 0x01, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x40, 0x11, 0xff, 0xfe, 0x80, /* ...@.... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x02, 0x22, /* ......." */
    0x02, 0x23, 0x00, 0x40, 0x6a, 0x6d, 0x01, 0x3e, /* .#.@jm.> */
    0xf0, 0x12, 0x00, 0x01, 0x00, 0x0e, 0x00, 0x01, /* ........ */
    0x00, 0x01, 0xac, 0x7d, 0x87, 0x3a, 0x00, 0x11, /* ...}.:.. */
    0x22, 0x33, 0x44, 0x56, 0x00, 0x03, 0x00, 0x0c, /* "3DV.... */
    0xc0, 0xde, 0xdb, 0xad, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x02, /* ........ */
    0x02, 0xbc, 0x00, 0x06, 0x00, 0x08, 0x00, 0x17, /* ........ */
    0x00, 0x1f, 0x00, 0x29, 0x00, 0x18              /* ...).. */
    };

/* Frame (78 bytes) */
static char pkt11[78] = {
    0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, /* 33.3DV.. */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, /* ...3DV.. */
    0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56              /* "..3DV */
};

/* Frame (78 bytes) */
static char pkt12[78] = {
    0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, /* 33.3DV.. */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, /* ...3DV.. */
    0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56              /* "..3DV */
};

/* Frame (78 bytes) */
static char pkt13[78] = {
    0x33, 0x33, 0xff, 0x33, 0x44, 0x56, 0x00, 0x11, /* 33.3DV.. */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0x00, 0x00, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x02, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x01, 0xff, 0x33, 0x44, 0x56, 0x87, 0x00, /* ...3DV.. */
    0xd0, 0x03, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56              /* "..3DV */
};

/* Frame (70 bytes) */
static char pkt14[70] = {
    0x33, 0x33, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x85, 0x00, /* ........ */
    0xad, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};

/* Frame (70 bytes) */
static char pkt15[70] = {
    0x33, 0x33, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x85, 0x00, /* ........ */
    0xad, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};

/* Frame (70 bytes) */
static char pkt16[70] = {
    0x33, 0x33, 0x00, 0x00, 0x00, 0x02, 0x00, 0x11, /* 33...... */
    0x22, 0x33, 0x44, 0x56, 0x86, 0xdd, 0x60, 0x00, /* "3DV..`. */
    0x00, 0x00, 0x00, 0x10, 0x3a, 0xff, 0xfe, 0x80, /* ....:... */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x11, /* ........ */
    0x22, 0xff, 0xfe, 0x33, 0x44, 0x56, 0xff, 0x02, /* "..3DV.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x85, 0x00, /* ........ */
    0xad, 0xf9, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, /* ........ */
    0x00, 0x11, 0x22, 0x33, 0x44, 0x56              /* .."3DV */
};
#endif



TAHI_TEST_SEQ tahi_dhcpv6_04_003[] = {
    {TITLE, "dhcpv6 04-003", 13, 0},

    {CHECK,  &pkt7[0], sizeof(pkt7), 30},

    {CLEANUP, NX_NULL, 0, 0},
    {DUMP, NX_NULL, 0, 0}
};

int  tahi_dhcpv6_04_003_size = sizeof(tahi_dhcpv6_04_003) / sizeof(TAHI_TEST_SEQ);
#endif

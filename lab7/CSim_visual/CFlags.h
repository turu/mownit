#ifndef CFLAGS_H
#define CFLAGS_H

namespace CSim
{

    /* flags */
    const int CE_WRITE = 0x000001; //data will be saved to files (not used in ReRun mode)
    const int CE_NOT_WRITE = 0x000010; //data wont be saved to files (not used in ReRun mode)
    const int CE_ADJUSTABLE = 0x000100; //CSystem will adjust individual steps after every iteration (improves accuracy, decreases performance)
    const int CE_NOT_ADJUSTABLE = 0x001000;

    extern int CFlags;

}
#endif

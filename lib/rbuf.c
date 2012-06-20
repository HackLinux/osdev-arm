/*
 * =====================================================================================
 *
 *       Filename:  ringbuff.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/12/12 15:41:23
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  sahithi: sahithi.cse@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <funcs.h>

#define MAX_RBUF_LEN         256

static char ringbuf[MAX_RBUF_LEN];
static int r_st  = 0;
static int r_end = 1;

int console_write(char *, int len);
int write_ring_buf(char *rbuf, char *ubuf, unsigned int ulen);
static void copy_data(char *dbuf, unsigned int d_st, unsigned int d_end, char *ubuf, unsigned int ulen);

void print_rbuf(char *dbuf)
{   
    unsigned int end1 = 0, end2 = 0;
    
    if (r_st < r_end) {
        console_write(dbuf + r_st, r_end - r_st);
    }
    else { 
        end1 = MAX_RBUF_LEN  - r_st;
        end2 = r_end;
        console_write(dbuf + r_st, end1);
        console_write(dbuf, end2);
    }
}



int write_rbuf(char *ubuf, unsigned int ulen)
{
    char *rbuf = ringbuf;
    unsigned int r_nend;
    if (ulen > (MAX_RBUF_LEN - 1)) {
        ubuf = ubuf + (ulen  - (MAX_RBUF_LEN - 1));
        ulen = MAX_RBUF_LEN - 1;

    }

    /*
     *   ulen <= (MAX_RBUF_LEN - 1)
     */

    r_nend = (r_end + ulen) % MAX_RBUF_LEN;

    /* (r_st <= r_next_end < r_end)  || (r_next_end < r_end < r_st ) || (r_end < r_st <= r_next_end) */
    if ((r_st <= r_nend && r_end > r_nend) || ((r_st > r_end) && (r_nend >= r_st || r_nend < r_end)))
        r_st = (r_nend + 1)%MAX_RBUF_LEN;


    copy_data(rbuf, r_end, r_nend, ubuf, ulen);
    r_end = r_nend;
    return ulen;
}

static void copy_data(char *dbuf, unsigned int d_st, unsigned int d_end, char *ubuf, unsigned int ulen)
{
    unsigned int end1 = 0, end2 = 0;

    if (d_st < d_end) {
        memcpy(dbuf + d_st, ubuf, ulen);
    }
    else { 
        end2 = d_end;
        end1 = ulen - end2;
        memcpy(dbuf + d_st, ubuf, end1);
        memcpy(dbuf, ubuf + end1, end2);
    }
}

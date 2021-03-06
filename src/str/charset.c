/*
 * charset.c
 * Author: wangwei.
 * Char encoding detective and transcoding.
 */

#include <iconv.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "../../include/util/assert.h"
#include "../../include/str/charset.h"
#include "../../include/util/log.h"

int sg_charset_conv(const char *from_charset, const char *to_charset,
                    const char *from_str, size_t from_len,
                    char *out_str, size_t out_max_len)
{
    size_t rc;
    iconv_t ict;

    assert(from_charset);
    assert(strlen(from_charset) > 0);
    assert(to_charset);
    assert(strlen(to_charset) > 0);
    assert(from_str);
    assert(strlen(from_str) > 0);
    assert(from_len > 0);
    assert(out_str);
    assert(out_max_len > 0);

    ict = iconv_open(to_charset, from_charset);
    if (!ict) {
        sg_log_err("iconv from %s to %s open failure.", from_charset, to_charset);
        return -1;
    }

    memset(out_str, 0, out_max_len);
    errno = 0;
    rc = iconv(ict, (char **)&from_str, &from_len, &out_str, &out_max_len);
    if (rc == -1) {
        sg_log_err("iconv failure, %s.", strerror(errno));
        //perror("iconv");
        return -1;
    }

    iconv_close(ict);
    return 0;
}
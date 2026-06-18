/*!
 * \file post.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-18
 * \brief Module dedicated to running a power-on self-test (POST)
 */

#include "post-api.h"

enum PostReturnCode post_api_run(void) {
    return POST_RC_OK;
}

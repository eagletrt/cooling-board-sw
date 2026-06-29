/*!
 * \file post.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-18
 * \brief Module dedicated to running a power-on self-test (POST)
 */

#include "post-api.h"
#include "temperatures-api.h"

enum PostReturnCode post_api_run(void) {
    temperatures_api_init();

    return POST_RC_OK;
}

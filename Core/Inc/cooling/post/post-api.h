/*!
 * \file post.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-18
 * \brief Module dedicated to running a power-on self-test (POST)
 */

#ifndef POST_API_H
#define POST_API_H

#include "post.h"

/*!
 * \brief Run the power-on self-test
 *
 * \retval POST_RC_OK if POST has been able to complete succesfully
 * \retval POST_RC_ERROR if POST has been unsuccessful
 */
enum PostReturnCode post_api_run(void);

/*!
 * \brief Initializes all the modules
 *
 * \retval POST_RC_OK if all the modules have been initialized succesfully
 * \retval POST_RC_ERROR if any module was not able to initialize succesfully
 */
enum PostReturnCode post_api_modules_init(void);

#endif // POST_API_H

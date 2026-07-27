/*!
 * \file post.h
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-06-18
 * \brief Module dedicated to running a power-on self-test (POST)
 */

#include "post-api.h"
#include "temperatures-api.h"
#include "can-communication-api.h"

enum PostReturnCode post_api_run(struct PostInitData *post_init_data) {
    if (post_init_data == nullptr ||
        post_init_data->can_network_configurations[CAN_COMMUNICATION_NETWORK_PRIMARY].on_receive == nullptr ||
        post_init_data->can_network_configurations[CAN_COMMUNICATION_NETWORK_PRIMARY].send == nullptr) {
        return POST_RC_ERROR;
    }

    enum PostReturnCode post_return_code = POST_RC_OK;

    if (temperatures_api_init() != TEMPERATURES_RC_OK) {
        return POST_RC_UNINITIALIZED_MODULE;
    }

    if (can_communication_api_init(post_init_data->can_network_configurations) != CAN_COMMUNICATION_RC_OK) {
        post_return_code = POST_RC_UNINITIALIZED_MODULE;
    }

    return post_return_code;
}

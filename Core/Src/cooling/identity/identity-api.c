/*!
 * \file identity-api.c
 * \author Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-08-07
 * \brief Implementation of the APIs of the module dedicated to providing information about the cooling board.
 */

#include "identity-api.h"

#include <string.h>
#include <time.h>
#include "eagletrt.h"
#include "can-communication-api.h"
#include "can-primary-api.h"

/*!
 * \brief Internal module handler
 * \details Hidden from external linkage to enforce API-only access
 */
EAGLETRT_STATIC struct IdentityHandler identity_handler;

enum IdentityReturnCode identity_api_init(void) {
    memset(&identity_handler, 0, sizeof(identity_handler));

    struct tm timeinfo;
    strptime(__DATE__ " " __TIME__, "%b %d %Y %H:%M:%S", &timeinfo);
    identity_handler.firmware_build_time = mktime(&timeinfo);

    return IDENTITY_RC_OK;
}

EAGLETRT_STATIC enum IdentityReturnCode prv_identity_build_status_frame(uint8_t fsm_state, struct CanCommunicationFrame *frame) {
    frame->id = CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGFSM;
    union CanPrimaryMessages message = { .coolingfsm = { .status = fsm_state } };

    int16_t frame_length = can_primary_api_serialize_from_id(frame->id, &message, frame->data);

    if (frame_length >= 0) {
        frame->length = (uint8_t)frame_length;
    } else {
        return IDENTITY_RC_ERROR;
    }

    return IDENTITY_RC_OK;
}

EAGLETRT_STATIC enum IdentityReturnCode prv_identity_build_version_frame(struct CanCommunicationFrame *frame) {
    frame->id = CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGVERSION;
    union CanPrimaryMessages message = { .coolingversion = { .major = 0, .minor = 1, .patch = 0 } };

    int16_t frame_length = can_primary_api_serialize_from_id(frame->id, &message, frame->data);

    if (frame_length >= 0) {
        frame->length = (uint8_t)frame_length;
    } else {
        return IDENTITY_RC_ERROR;
    }

    return IDENTITY_RC_OK;
}

EAGLETRT_STATIC enum IdentityReturnCode prv_identity_build_version_info_frame(struct CanCommunicationFrame *frame) {
    frame->id = CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGVERSIONINFO;
    union CanPrimaryMessages message = { .coolingversioninfo = { .buildtime = identity_handler.firmware_build_time, .commithash = 0x6767, .dirty = 0 } };

    int16_t frame_length = can_primary_api_serialize_from_id(frame->id, &message, frame->data);

    if (frame_length >= 0) {
        frame->length = (uint8_t)frame_length;
    } else {
        return IDENTITY_RC_ERROR;
    }

    return IDENTITY_RC_OK;
}

EAGLETRT_STATIC enum IdentityReturnCode prv_identity_build_libcan_version_frame(struct CanCommunicationFrame *frame) {
    frame->id = CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGLIBCANVERSION;
    union CanPrimaryMessages message = { .coolinglibcanversion = { .major = can_version_major, .minor = can_version_minor, .patch = can_version_patch } };

    int16_t frame_length = can_primary_api_serialize_from_id(frame->id, &message, frame->data);

    if (frame_length >= 0) {
        frame->length = (uint8_t)frame_length;
    } else {
        return IDENTITY_RC_ERROR;
    }

    return IDENTITY_RC_OK;
}

EAGLETRT_STATIC enum IdentityReturnCode prv_identity_build_libcan_version_info_frame(struct CanCommunicationFrame *frame) {
    frame->id = CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGLIBCANVERSIONINFO;
    union CanPrimaryMessages message = { .coolinglibcanversioninfo = { .generationtime = can_generation_time, .commithash = 0x6767, .dirty = 0 } };

    int16_t frame_length = can_primary_api_serialize_from_id(frame->id, &message, frame->data);

    if (frame_length >= 0) {
        frame->length = (uint8_t)frame_length;
    } else {
        return IDENTITY_RC_ERROR;
    }

    return IDENTITY_RC_OK;
}

enum IdentityReturnCode identity_api_send_status(uint8_t fsm_state, uint32_t tick) {
    if (tick - identity_handler.last_send_tick >= can_primary_cycle_time_coolingfsm) {
        struct CanCommunicationFrame status_frame = { 0 };

        // TODO: Check for errors
        prv_identity_build_status_frame(fsm_state, &status_frame);

        can_communication_api_add_to_tx_buffer(CAN_COMMUNICATION_NETWORK_PRIMARY, &status_frame);
    }

    return IDENTITY_RC_OK;
}

enum IdentityReturnCode identity_api_send_information(uint32_t tick) {
    constexpr uint32_t identity_information_cycle_time = can_primary_cycle_time_coolingversion;

    if (tick - identity_handler.last_send_tick >= identity_information_cycle_time) {
        struct CanCommunicationFrame version_frame = { 0 };
        struct CanCommunicationFrame version_info_frame = { 0 };
        struct CanCommunicationFrame libcan_version_frame = { 0 };
        struct CanCommunicationFrame libcan_version_info_frame = { 0 };

        // TODO: Check for errors
        prv_identity_build_version_frame(&version_frame);
        prv_identity_build_version_info_frame(&version_info_frame);
        prv_identity_build_libcan_version_frame(&libcan_version_frame);
        prv_identity_build_libcan_version_info_frame(&libcan_version_info_frame);

        can_communication_api_add_to_tx_buffer(CAN_COMMUNICATION_NETWORK_PRIMARY, &version_frame);
        can_communication_api_add_to_tx_buffer(CAN_COMMUNICATION_NETWORK_PRIMARY, &version_info_frame);
        can_communication_api_add_to_tx_buffer(CAN_COMMUNICATION_NETWORK_PRIMARY, &libcan_version_frame);
        can_communication_api_add_to_tx_buffer(CAN_COMMUNICATION_NETWORK_PRIMARY, &libcan_version_info_frame);
    }

    return IDENTITY_RC_OK;
}

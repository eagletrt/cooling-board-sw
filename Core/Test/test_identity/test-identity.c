/*!
 * \file test_identity.c
 * \authors Alessandro Bridi
 * \authors Thomas Moretti [majorfrost173@gmail.com]
 * \date 2026-08-07
 * \brief Unit tests for the identity module.
 */

#include <unity.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "fff.h"
#include "identity-api.h"
#include "can-communication-api.h"
#include "can-primary.h"
#include "can-primary-api.h"
#include "eagletrt-api.h"

DEFINE_FFF_GLOBALS;

FAKE_VALUE_FUNC(enum CanCommunicationReturnCode, mock_can_send, const struct CanCommunicationFrame *);
FAKE_VALUE_FUNC(enum CanCommunicationReturnCode, mock_on_receive, const struct CanCommunicationFrame *);

#define TEST_MAX_CAPTURED_FRAMES (4U)

/*!
 * \brief Frames flushed onto the primary bus during a single test.
 */
EAGLETRT_STATIC struct CanCommunicationFrame test_captured_frames[TEST_MAX_CAPTURED_FRAMES];
EAGLETRT_STATIC uint32_t test_captured_count;

/*!
 * \brief Send callback: records every frame the module flushes to the primary bus.
 *
 * \param[in] frame The frame being transmitted.
 *
 * \retval CAN_COMMUNICATION_RC_OK always.
 */
EAGLETRT_STATIC enum CanCommunicationReturnCode prv_capture_send(const struct CanCommunicationFrame *frame) {
    if (frame != NULL && test_captured_count < TEST_MAX_CAPTURED_FRAMES) {
        test_captured_frames[test_captured_count] = *frame;
        test_captured_count++;
    }
    return CAN_COMMUNICATION_RC_OK;
}

/*!
 * \brief Drain the primary network's tx queue, invoking \ref prv_capture_send per frame.
 */
EAGLETRT_STATIC void prv_flush_primary(void) {
    EAGLETRT_API_UNUSED(can_communication_api_process_tx(CAN_COMMUNICATION_NETWORK_PRIMARY));
}

void setUp(void) {
    RESET_FAKE(mock_can_send);
    RESET_FAKE(mock_on_receive);
    FFF_RESET_HISTORY();

    mock_can_send_fake.custom_fake = prv_capture_send;
    mock_on_receive_fake.return_val = CAN_COMMUNICATION_RC_OK;

    memset(test_captured_frames, 0, sizeof(test_captured_frames));
    test_captured_count = 0U;

    struct CanCommunicationNetworkConfig configs[CAN_COMMUNICATION_NETWORK_COUNT];
    for (size_t i = 0; i < CAN_COMMUNICATION_NETWORK_COUNT; i++) {
        configs[i].send = mock_can_send;
        configs[i].on_receive = mock_on_receive;
        configs[i].cs_enter = NULL;
        configs[i].cs_exit = NULL;
    }
    can_communication_api_init(configs);

    identity_api_init();
}

void tearDown(void) {
}

/* --- Test Cases --- */

/*!
 * \defgroup identity_api_init Tests for identity_api_init
 * \{
 */

void test_identity_api_init_returns_ok(void) {
    TEST_ASSERT_EQUAL_MESSAGE(IDENTITY_RC_OK, identity_api_init(), "Identity initialization must report success.");
}
/*! \} */

/*!
 * \defgroup identity_api_send_state Tests for identity_api_send_state
 * \{
 */

void test_send_state_returns_ok(void) {
    enum IdentityReturnCode rc = identity_api_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_IDLE);

    TEST_ASSERT_EQUAL_MESSAGE(IDENTITY_RC_OK, rc, "Sending the state must report success.");
}

void test_send_state_queues_exactly_one_frame(void) {
    identity_api_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_FLASH);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1U, test_captured_count, "Sending the state must queue exactly one primary frame.");
}

void test_send_state_targets_coolingfsm_frame_id(void) {
    identity_api_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_IDLE);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGFSM, test_captured_frames[0].id, "The state must be reported on the COOLINGFSM frame.");
}

void test_send_state_encodes_status(void) {
    identity_api_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_INIT);
    prv_flush_primary();

    union CanPrimaryMessages msg = { 0 };
    EAGLETRT_API_UNUSED(can_primary_api_deserialize_from_id(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGFSM, test_captured_frames[0].data, &msg));

    TEST_ASSERT_EQUAL_UINT8_MESSAGE(CAN_PRIMARY_COOLINGFSM_STATUS_INIT, msg.coolingfsm.status, "The status must be encoded into the COOLINGFSM frame.");
}

/*! \} */

/*!
 * \defgroup identity_api_periodically_send_state Tests for identity_api_periodically_send_state
 * \{
 */

void test_periodically_send_state_returns_ok(void) {
    enum IdentityReturnCode rc = identity_api_periodically_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_IDLE, can_primary_cycle_time_coolingfsm);

    TEST_ASSERT_EQUAL_MESSAGE(IDENTITY_RC_OK, rc, "Periodically sending the state must report success.");
}

void test_periodically_send_state_does_not_send_before_cycle_elapsed(void) {
    identity_api_periodically_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_FLASH, can_primary_cycle_time_coolingfsm - 1);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0U, test_captured_count, "No state frame must be emitted before the cycle time has elapsed.");
}

void test_periodically_send_state_sends_once_cycle_elapsed(void) {
    identity_api_periodically_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_INIT, can_primary_cycle_time_coolingfsm);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1U, test_captured_count, "A single state frame must be emitted once the cycle time has elapsed.");
}

void test_periodically_send_state_does_not_resend_within_same_cycle(void) {
    // First call at the cycle boundary emits; a second call still inside the
    // same cycle window must not enqueue another frame.
    identity_api_periodically_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_ERROR, can_primary_cycle_time_coolingfsm);
    identity_api_periodically_send_state(CAN_PRIMARY_COOLINGFSM_STATUS_IDLE, can_primary_cycle_time_coolingfsm + 1);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1U, test_captured_count, "A second call within the same cycle must not emit another state frame.");
}
/*! \} */

/*!
 * \defgroup identity_api_periodically_send_version Tests for identity_api_periodically_send_version
 * \{
 */

void test_periodically_send_version_does_not_send_before_cycle_elapsed(void) {
    identity_api_periodically_send_version(can_primary_cycle_time_coolingversion - 1);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0U, test_captured_count, "No version frame must be emitted before the cycle time has elapsed.");
}

void test_periodically_send_version_emits_two_frames_when_cycle_elapsed(void) {
    identity_api_periodically_send_version(can_primary_cycle_time_coolingversion);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(2U, test_captured_count, "A version report must emit both the version and version-info frames.");
}

void test_periodically_send_version_first_frame_is_coolingversion(void) {
    identity_api_periodically_send_version(can_primary_cycle_time_coolingversion);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGVERSION, test_captured_frames[0].id, "The first version frame must be the COOLINGVERSION frame.");
}

void test_periodically_send_version_second_frame_is_coolingversioninfo(void) {
    identity_api_periodically_send_version(can_primary_cycle_time_coolingversion);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGVERSIONINFO, test_captured_frames[1].id, "The second version frame must be the COOLINGVERSIONINFO frame.");
}

void test_periodically_send_version_encodes_firmware_version(void) {
    identity_api_periodically_send_version(can_primary_cycle_time_coolingversion);
    prv_flush_primary();

    union CanPrimaryMessages msg = { 0 };
    EAGLETRT_API_UNUSED(can_primary_api_deserialize_from_id(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGVERSION, test_captured_frames[0].data, &msg));

    TEST_ASSERT_EQUAL_UINT16_MESSAGE(IDENTITY_VERSION_MAJOR, msg.coolingversion.major, "The firmware major version must be encoded.");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(IDENTITY_VERSION_MINOR, msg.coolingversion.minor, "The firmware minor version must be encoded.");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(IDENTITY_VERSION_PATCH, msg.coolingversion.patch, "The firmware patch version must be encoded.");
}
/*! \} */

/*!
 * \defgroup identity_api_periodically_send_libcan_version Tests for identity_api_periodically_send_libcan_version
 * \{
 */

void test_periodically_send_libcan_version_does_not_send_before_cycle_elapsed(void) {
    identity_api_periodically_send_libcan_version(can_primary_cycle_time_coolinglibcanversion - 1);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0U, test_captured_count, "No libcan version frame must be emitted before the cycle time has elapsed.");
}

void test_periodically_send_libcan_version_emits_two_frames_when_cycle_elapsed(void) {
    identity_api_periodically_send_libcan_version(can_primary_cycle_time_coolinglibcanversion);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(2U, test_captured_count, "A libcan version report must emit both the version and version-info frames.");
}

void test_periodically_send_libcan_version_first_frame_is_coolinglibcanversion(void) {
    identity_api_periodically_send_libcan_version(can_primary_cycle_time_coolinglibcanversion);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGLIBCANVERSION, test_captured_frames[0].id, "The first libcan frame must be the COOLINGLIBCANVERSION frame.");
}

void test_periodically_send_libcan_version_second_frame_is_coolinglibcanversioninfo(void) {
    identity_api_periodically_send_libcan_version(can_primary_cycle_time_coolinglibcanversion);
    prv_flush_primary();

    TEST_ASSERT_EQUAL_UINT32_MESSAGE(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGLIBCANVERSIONINFO, test_captured_frames[1].id, "The second libcan frame must be the COOLINGLIBCANVERSIONINFO frame.");
}

void test_periodically_send_libcan_version_encodes_libcan_version(void) {
    identity_api_periodically_send_libcan_version(can_primary_cycle_time_coolinglibcanversion);
    prv_flush_primary();

    union CanPrimaryMessages msg = { 0 };
    EAGLETRT_API_UNUSED(can_primary_api_deserialize_from_id(CAN_PRIMARY_MESSAGE_FRAME_ID_COOLINGLIBCANVERSION, test_captured_frames[0].data, &msg));

    TEST_ASSERT_EQUAL_UINT16_MESSAGE(can_version_major, msg.coolinglibcanversion.major, "The libcan major version must be encoded.");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(can_version_minor, msg.coolinglibcanversion.minor, "The libcan minor version must be encoded.");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(can_version_patch, msg.coolinglibcanversion.patch, "The libcan patch version must be encoded.");
}
/*! \} */

int main(void) {
    UNITY_BEGIN();

    /*!
     * \addtogroup identity_api_init
     * \{
     */
    RUN_TEST(test_identity_api_init_returns_ok);
    /*! \} */

    /*!
     * \addtogroup identity_api_send_state
     * \{
     */
    RUN_TEST(test_send_state_returns_ok);
    RUN_TEST(test_send_state_queues_exactly_one_frame);
    RUN_TEST(test_send_state_targets_coolingfsm_frame_id);
    RUN_TEST(test_send_state_encodes_status);
    /*! \} */

    /*!
     * \addtogroup identity_api_periodically_send_state
     * \{
     */
    RUN_TEST(test_periodically_send_state_returns_ok);
    RUN_TEST(test_periodically_send_state_does_not_send_before_cycle_elapsed);
    RUN_TEST(test_periodically_send_state_sends_once_cycle_elapsed);
    RUN_TEST(test_periodically_send_state_does_not_resend_within_same_cycle);
    /*! \} */

    /*!
     * \addtogroup identity_api_periodically_send_version
     * \{
     */
    RUN_TEST(test_periodically_send_version_does_not_send_before_cycle_elapsed);
    RUN_TEST(test_periodically_send_version_emits_two_frames_when_cycle_elapsed);
    RUN_TEST(test_periodically_send_version_first_frame_is_coolingversion);
    RUN_TEST(test_periodically_send_version_second_frame_is_coolingversioninfo);
    RUN_TEST(test_periodically_send_version_encodes_firmware_version);
    /*! \} */

    /*!
     * \addtogroup identity_api_periodically_send_libcan_version
     * \{
     */
    RUN_TEST(test_periodically_send_libcan_version_does_not_send_before_cycle_elapsed);
    RUN_TEST(test_periodically_send_libcan_version_emits_two_frames_when_cycle_elapsed);
    RUN_TEST(test_periodically_send_libcan_version_first_frame_is_coolinglibcanversion);
    RUN_TEST(test_periodically_send_libcan_version_second_frame_is_coolinglibcanversioninfo);
    RUN_TEST(test_periodically_send_libcan_version_encodes_libcan_version);
    /*! \} */

    return UNITY_END();
}

from application_client.boilerplate_transaction import Transaction
from application_client.boilerplate_command_sender import BoilerplateCommandSender, Errors
from application_client.boilerplate_response_unpacker import unpack_get_public_key_response, unpack_sign_tx_response
from ragger.backend import RaisePolicy
from ragger.navigator import NavInsID
from utils import ROOT_SCREENSHOT_PATH, check_signature_validity

# In this tests we check the behavior of the device when asked to sign a transaction


# Transaction signature refused test
# The test will ask for a transaction signature that will be refused on screen
def test_sign_tx_accept(firmware, backend, navigator, test_name):
    # Use the app interface instead of raw interface
    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/0'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, pub_key, _, _ = unpack_get_public_key_response(rapdu.data)

    if firmware.device.startswith("nano"):
        with client.sign_tx(path=path, transaction=bytes.fromhex("02008093006400008c0117010001b2a0187dfeb154a0852c822190317a96fdba4d7001000100e0ac003b00000000000000000000000000000000000000000000000000000000080000000000000000000117010001aca553215d606c663cac99e8503abe19f76289eb010001004026ff3a000000000000000000000000000000000000000000000000000000000000000000000000")):
            # Disable raising when trying to unpack an error APDU
            backend.raise_policy = RaisePolicy.RAISE_NOTHING
            navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
                                                      [NavInsID.BOTH_CLICK],
                                                      "Accept and send",
                                                      ROOT_SCREENSHOT_PATH,
                                                      test_name)

    else:
        for i in range(3):
            instructions = [NavInsID.USE_CASE_REVIEW_TAP] * i
            instructions += [NavInsID.USE_CASE_REVIEW_REJECT,
                             NavInsID.USE_CASE_CHOICE_CONFIRM,
                             NavInsID.USE_CASE_STATUS_WAIT]
            with client.sign_tx(path=path, transaction=transaction):
                backend.raise_policy = RaisePolicy.RAISE_NOTHING
                navigator.navigate_and_compare(ROOT_SCREENSHOT_PATH,
                                               test_name + f"/part{i}",
                                               instructions)


# Transaction signature refused test
# The test will ask for a transaction signature that will be refused on screen
def test_sign_tx_refused(firmware, backend, navigator, test_name):
    # Use the app interface instead of raw interface
    client = BoilerplateCommandSender(backend)
    path: str = "m/44'/0'/0'/0/0"

    rapdu = client.get_public_key(path=path)
    _, pub_key, _, _ = unpack_get_public_key_response(rapdu.data)

    if firmware.device.startswith("nano"):
        with client.sign_tx(path=path, transaction=bytes.fromhex("02008093006400008c0117010001b2a0187dfeb154a0852c822190317a96fdba4d7001000100e0ac003b00000000000000000000000000000000000000000000000000000000080000000000000000000117010001aca553215d606c663cac99e8503abe19f76289eb010001004026ff3a000000000000000000000000000000000000000000000000000000000000000000000000")):
            # Disable raising when trying to unpack an error APDU
            backend.raise_policy = RaisePolicy.RAISE_NOTHING
            navigator.navigate_until_text_and_compare(NavInsID.RIGHT_CLICK,
                                                      [NavInsID.BOTH_CLICK],
                                                      "Reject",
                                                      ROOT_SCREENSHOT_PATH,
                                                      test_name)

        assert client.get_async_response().status == Errors.SW_DENY
    else:
        for i in range(3):
            instructions = [NavInsID.USE_CASE_REVIEW_TAP] * i
            instructions += [NavInsID.USE_CASE_REVIEW_REJECT,
                             NavInsID.USE_CASE_CHOICE_CONFIRM,
                             NavInsID.USE_CASE_STATUS_WAIT]
            with client.sign_tx(path=path, transaction=transaction):
                backend.raise_policy = RaisePolicy.RAISE_NOTHING
                navigator.navigate_and_compare(ROOT_SCREENSHOT_PATH,
                                               test_name + f"/part{i}",
                                               instructions)
            assert client.get_async_response().status == Errors.SW_DENY

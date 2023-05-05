#pragma once

#include <stddef.h>   // size_t
#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool

/**
 * Enumeration with expected INS of APDU commands.
 */
typedef enum {
    GET_CONFIGURATION = 0x04,     /// This command returns specific application configuratio    
    GET_PUBLIC_KEY = 0x05,  /// GET PUBLIC ADDRESS
    SIGN_TX = 0x06,          /// SIGN TRANSACTION
    SIGN_NS = 0x07          /// SIGN NULS PERSONAL MESSAGE
} command_e;


/**
 * Structure with fields of APDU command.
 */
typedef struct {
    uint8_t cla;    /// Instruction class
    command_e ins;  /// Instruction code
    uint8_t p1;     /// Instruction parameter 1
    uint8_t p2;     /// Instruction parameter 2
    uint8_t lc;     /// Length of command data
    uint8_t *data;  /// Command data
} command_t;


/**
 * Parse APDU command from byte buffer.
 *
 * @param[out] cmd
 *   Structured APDU command (CLA, INS, P1, P2, Lc, Command data).
 * @param[in]  buf
 *   Byte buffer with raw APDU command.
 * @param[in]  buf_len
 *   Length of byte buffer.
 *
 * @return true if success, false otherwise.
 *
 */
bool apdu_parser(command_t *cmd, uint8_t *buf, size_t buf_len);

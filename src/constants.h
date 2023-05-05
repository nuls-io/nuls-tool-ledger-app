#pragma once

/**
 * Instruction class of the Boilerplate application.
 */
#define CLA 0xE0

/**
 * Length of APPNAME variable in the Makefile.
 */
#define APPNAME_LEN (sizeof(APPNAME) - 1)

/**
 * Maximum length of FLAGS(0x00) || MAJOR_VERSION || MINOR_VERSION || PATCH_VERSION.
 */
#define APPVERSION_LEN 4

/**
 * Maximum length of application name.
 */
#define MAX_APPNAME_LEN 64

/**
 * Maximum transaction length (bytes).
 */
#define MAX_TRANSACTION_LEN 300

/**
 * Maximum signature length (bytes).
 */
#define MAX_DER_SIG_LEN 72

/**
 * Exponent used to convert mBOL to BOL unit (N BOL = N * 10^3 mBOL).
 */
#define EXPONENT_SMALLEST_UNIT 3

#define MALLOC_BUFF_MAX_LEN 400



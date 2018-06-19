// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef _OE_CERT_H
#define _OE_CERT_H

#include <openenclave/bits/result.h>
#include <openenclave/bits/types.h>
#include "ec.h"
#include "rsa.h"

OE_EXTERNC_BEGIN

typedef struct _oe_cert
{
    /* Internal private implementation */
    uint64_t impl[4];
} oe_cert_t;

typedef struct _oe_cert_chain
{
    /* Internal private implementation */
    uint64_t impl[4];
} oe_cert_chain_t;

typedef struct _OE_CRL OE_CRL;

/* Error message type for oe_verify_cert_error_t() function */
typedef struct _oe_verify_cert_error
{
    /* Zero-terminated string error message */
    char buf[1024];
} oe_verify_cert_error_t;

/**
 * Read a certificate from PEM format
 *
 * This function reads a certificate from PEM data with the following PEM
 * headers.
 *
 *     -----BEGIN CERT-----
 *     ...
 *     -----END CERT-----
 *
 * The caller is responsible for releasing the certificate by passing it to
 * oe_cert_free().
 *
 * @param pemData zero-terminated PEM data
 * @param pemSize size of the PEM data (including the zero-terminator)
 * @param cert initialized certificate handle upon return
 *
 * @return OE_OK load was successful
 */
oe_result_t oe_cert_read_pem(
    const void* pemData,
    size_t pemSize,
    oe_cert_t* cert);

/**
 * Read a certificate chain from PEM format.
 *
 * This function reads a certificate chain from PEM data with the following PEM
 * headers.
 *
 *     -----BEGIN CERT-----
 *     ...
 *     -----END CERT-----
 *     -----BEGIN CERT-----
 *     ...
 *     -----END CERT-----
 *     -----BEGIN CERT-----
 *     ...
 *     -----END CERT-----
 *
 * Each certificate in the chain is verified with respect to its predecessor in
 * the chain. If any such verification fails, this function returns an error.
 * The caller is responsible for releasing the certificate chain by passing it
 * to oe_cert_chain_free().
 *
 * @param pemData zero-terminated PEM data
 * @param pemSize size of the PEM data (including the zero-terminator)
 * @param cert initialized certificate chain handle upon return
 *
 * @return OE_OK load was successful
 */
oe_result_t oe_cert_chain_read_pem(
    const void* pemData,
    size_t pemSize,
    oe_cert_chain_t* chain);

/**
 * Releases a certificate
 *
 * This function releases the given certificate.
 *
 * @param cert handle of the certificate being released
 *
 * @return OE_OK certificate was successfully released
 */
oe_result_t oe_cert_free(oe_cert_t* cert);

/**
 * Releases a certificate chain
 *
 * This function releases the given certificate chain.
 *
 * @param chain handle of certificate chain being released
 *
 * @return OE_OK certificate chain was successfully released
 */
oe_result_t oe_cert_chain_free(oe_cert_chain_t* chain);

/**
 * Verify the given certificate against a given certificate chain
 *
 * This function verifies the given certificate against the certificate
 * authority (CA), given by the certificate chain.
 *
 * @param cert verify this certificate
 * @param chain verify the certificate against this certificate chain
 * @param crl verify the certificate against this CRL
 * @param error Optional. Holds the error message if this function failed.
 *
 * @return OE_OK verify ok
 * @return OE_VERIFY_FAILED
 * @return OE_INVALID_PARAMETER
 * @return OE_FAILURE
 */
oe_result_t oe_cert_verify(
    oe_cert_t* cert,
    oe_cert_chain_t* chain,
    OE_CRL* crl, /* ATTN: placeholder for future capability */
    oe_verify_cert_error_t* error);

/**
 * Get the RSA public key from a certificate.
 *
 * This function gets the RSA public key from the given certificate. If the
 * the certificate does not contain an RSA public key, this function returns
 * OE_WRONG_TYPE.
 *
 * @param cert the certificate whose RSA public key is sought.
 * @param publicKey the handle of an RSA public key upon successful return.
 *     If successful, the caller is responsible for eventually releasing the
 *     key by passing it to **oe_rsa_public_key_free()**.
 *
 * @return OE_OK success
 * @return OE_INVALID_PARAMETER a parameter is invalid
 * @return OE_WRONG_TYPE the certificate does not contain an RSA public key
 * @return OE_FAILURE general failure
 */
oe_result_t oe_cert_get_rsa_public_key(
    const oe_cert_t* cert,
    oe_rsa_public_key_t* publicKey);

/**
 * Get the EC public key from a certificate.
 *
 * This function gets the EC public key from the given certificate. If the
 * the certificate does not contain an EC public key, this function returns
 * OE_WRONG_TYPE.
 *
 * @param cert the certificate whose EC public key is sought.
 * @param publicKey the handle of an EC public key upon successful return.
 *     If successful, the caller is responsible for eventually releasing the
 *     key by passing it to **oe_ec_public_key_free()**.
 *
 * @return OE_OK success
 * @return OE_INVALID_PARAMETER a parameter is invalid
 * @return OE_WRONG_TYPE the certificate does not contain an EC public key
 * @return OE_FAILURE general failure
 */
oe_result_t oe_cert_get_ec_public_key(
    const oe_cert_t* cert,
    oe_ec_public_key_t* publicKey);

/**
 * Get the length of a certificate chain.
 *
 * This function gets the length of the certificate chain. This length
 * is the total number of certificates contained in the chain.
 *
 * @param chain the chain whose length is to be determined
 * @param length the certificate chain length on success or zero on failure
 *
 * @return OE_OK success
 * @return OE_INVALID_PARAMETER a parameter is invalid
 * @return OE_FAILURE general failure
 */
oe_result_t oe_cert_chain_get_length(
    const oe_cert_chain_t* chain,
    size_t* length);

/**
 * Fetch the certificate with the given index from a certificate chain.
 *
 * This function fetches the certificate with the given index from a
 * certificate chain. The certificate with the highest index is the leaf
 * certificate. Use oe_cert_chain_get_length() to obtain the number of
 * certificates in the chain.
 *
 * @param chain the chain whose certificate is fetched.
 * @param index the index of the certificate to be fetched.
 * @param cert the handle of a certificate upon successful return.
 *     If successful, the caller is responsible for eventually releasing the
 *     certificate by passing it to **oe_cert_free()**.
 *
 * @return OE_OK success
 * @return OE_INVALID_PARAMETER a parameter is invalid
 * @return OE_OUT_OF_BOUNDS the certificate index is out of bounds
 * @return OE_FAILURE general failure
 */
oe_result_t oe_cert_chain_get_cert(
    const oe_cert_chain_t* chain,
    size_t index,
    oe_cert_t* cert);

/**
 * Fetch the root certificate from a certificate chain.
 *
 * This function fetches the root certificate from a certificate chain. The
 * root certificate is found by walking from the leaf certificate upwards
 * until a self-signed certificate is found. A self-signed certificate is one
 * in which the issuer-name and the subject-name are the same.
 *
 * @param chain the chain whose root certificate is fetched.
 * @param cert the handle of a certificate upon successful return.
 *     If successful, the caller is responsible for eventually releasing the
 *     certificate by passing it to **oe_cert_free()**.
 *
 * @return OE_OK success
 * @return OE_INVALID_PARAMETER a parameter is invalid
 * @return OE_NOT_FOUND no self-signed certificate was found
 * @return OE_FAILURE general failure
 */
oe_result_t oe_cert_chain_get_root_cert(
    const oe_cert_chain_t* chain,
    oe_cert_t* cert);

/**
 * Fetch the leaf certificate from a certificate chain.
 *
 * This function fetches the leaf certificate from a certificate chain.
 *
 * @param chain the chain whose leaf certificate is fetched.
 * @param cert the handle of a certificate upon successful return.
 *     If successful, the caller is responsible for eventually releasing the
 *     certificate by passing it to **oe_cert_free()**.
 *
 * @return OE_OK success
 * @return OE_INVALID_PARAMETER a parameter is invalid
 * @return OE_FAILURE general failure
 */
oe_result_t oe_cert_chain_get_leaf_cert(
    const oe_cert_chain_t* chain,
    oe_cert_t* cert);

OE_EXTERNC_END

#endif /* _OE_CERT_H */
/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2020-2025 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeSpan
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
#include <mbedtls/hkdf.h>
#include <mbedtls/platform_util.h>

#include "HKDF.h"

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// Wrapper function to call mbedtls_hkdf, below, with
// HAP-specific parameters and assumptions

int HKDF::create(uint8_t *outputKey, uint8_t *inputKey, int inputLen, const char *salt, const char *info){
  
  return(mbedtls_hkdf( mbedtls_md_info_from_type(MBEDTLS_MD_SHA512),
                (uint8_t *) salt, (size_t) strlen(salt),
                inputKey, (size_t) inputLen,
                (uint8_t *) info, (size_t) strlen(info),
                outputKey, 32 ));
  
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
// CODE FOR HKDF IS MISSING FROM THE MBEDTLS LIBRARY INCLUDED WITH THE
// ARDUINO-ESP32 LIBRARY.  THE CODE BELOW IS SOURCE DIRECTLY FROM THE MBEDTLS
// GITHUB.  SEE THE MBEDTLS GITHUB SITE FOR LICENSING TERMS:
//
//  https://github.com/ARMmbed/mbedtls/blob/development/LICENSE 
//
//

int mbedtls_hkdf( const mbedtls_md_info_t *md, const unsigned char *salt,
                  size_t salt_len, const unsigned char *ikm, size_t ikm_len,
                  const unsigned char *info, size_t info_len,
                  unsigned char *okm, size_t okm_len )
{
    int ret;
    unsigned char prk[MBEDTLS_MD_MAX_SIZE];

    ret = mbedtls_hkdf_extract( md, salt, salt_len, ikm, ikm_len, prk );

    if( ret == 0 )
    {
        ret = mbedtls_hkdf_expand( md, prk, mbedtls_md_get_size( md ),
                                   info, info_len, okm, okm_len );
    }

    mbedtls_platform_zeroize( prk, sizeof( prk ) );

    return( ret );
}

int mbedtls_hkdf_extract( const mbedtls_md_info_t *md,
                          const unsigned char *salt, size_t salt_len,
                          const unsigned char *ikm, size_t ikm_len,
                          unsigned char *prk )
{
    unsigned char null_salt[MBEDTLS_MD_MAX_SIZE] = { '\0' };

    if( salt == NULL )
    {
        size_t hash_len;

        if( salt_len != 0 )
        {
            return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
        }

        hash_len = mbedtls_md_get_size( md );

        if( hash_len == 0 )
        {
            return MBEDTLS_ERR_HKDF_BAD_INPUT_DATA;
        }

        salt = null_salt;
        salt_len = hash_len;
    }

    return( mbedtls_md_hmac( md, salt, salt_len, ikm, ikm_len, prk ) );
}

int mbedtls_hkdf_expand( const mbedtls_md_info_t *md, const unsigned char *prk,
                         size_t prk_len, const unsigned char *info,
                         size_t info_len, unsigned char *okm, size_t okm_len )
{
    size_t hash_len;
    size_t where = 0;
    size_t n;
    size_t t_len = 0;
    size_t i;
    int ret = 0;
    mbedtls_md_context_t ctx;
    unsigned char t[MBEDTLS_MD_MAX_SIZE];

    if( okm == NULL )
    {
        return( MBEDTLS_ERR_HKDF_BAD_INPUT_DATA );
    }

    hash_len = mbedtls_md_get_size( md );

    if( prk_len < hash_len || hash_len == 0 )
    {
        return( MBEDTLS_ERR_HKDF_BAD_INPUT_DATA );
    }

    if( info == NULL )
    {
        info = (const unsigned char *) "";
        info_len = 0;
    }

    n = okm_len / hash_len;

    if( okm_len % hash_len != 0 )
    {
        n++;
    }

    /*
     * Per RFC 5869 Section 2.3, okm_len must not exceed
     * 255 times the hash length
     */
    if( n > 255 )
    {
        return( MBEDTLS_ERR_HKDF_BAD_INPUT_DATA );
    }

    mbedtls_md_init( &ctx );

    if( ( ret = mbedtls_md_setup( &ctx, md, 1 ) ) != 0 )
    {
        goto exit;
    }

    memset( t, 0, hash_len );

    /*
     * Compute T = T(1) | T(2) | T(3) | ... | T(N)
     * Where T(N) is defined in RFC 5869 Section 2.3
     */
    for( i = 1; i <= n; i++ )
    {
        size_t num_to_copy;
        unsigned char c = i & 0xff;

        ret = mbedtls_md_hmac_starts( &ctx, prk, prk_len );
        if( ret != 0 )
        {
            goto exit;
        }

        ret = mbedtls_md_hmac_update( &ctx, t, t_len );
        if( ret != 0 )
        {
            goto exit;
        }

        ret = mbedtls_md_hmac_update( &ctx, info, info_len );
        if( ret != 0 )
        {
            goto exit;
        }

        /* The constant concatenated to the end of each T(n) is a single octet.
         * */
        ret = mbedtls_md_hmac_update( &ctx, &c, 1 );
        if( ret != 0 )
        {
            goto exit;
        }

        ret = mbedtls_md_hmac_finish( &ctx, t );
        if( ret != 0 )
        {
            goto exit;
        }

        num_to_copy = i != n ? hash_len : okm_len - where;
        memcpy( okm + where, t, num_to_copy );
        where += hash_len;
        t_len = hash_len;
    }

  exit:
    mbedtls_md_free( &ctx );
    mbedtls_platform_zeroize( t, sizeof( t ) );

    return( ret );
}

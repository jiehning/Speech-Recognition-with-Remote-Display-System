# Install script for directory: /root/pico/pico-sdk/lib/mbedtls/include

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/root/pico/ssd1306_tcp_test/build/_deps")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mbedtls" TYPE FILE MESSAGE_NEVER PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/aes.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/aesni.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/arc4.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/aria.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/asn1.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/asn1write.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/base64.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/bignum.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/blowfish.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/bn_mul.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/camellia.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ccm.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/certs.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/chacha20.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/chachapoly.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/check_config.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/cipher.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/cipher_internal.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/cmac.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/compat-1.3.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/config.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/config_psa.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/constant_time.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ctr_drbg.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/debug.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/des.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/dhm.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ecdh.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ecdsa.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ecjpake.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ecp.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ecp_internal.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/entropy.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/entropy_poll.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/error.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/gcm.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/havege.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/hkdf.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/hmac_drbg.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/md.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/md2.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/md4.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/md5.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/md_internal.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/memory_buffer_alloc.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/net.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/net_sockets.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/nist_kw.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/oid.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/padlock.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/pem.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/pk.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/pk_internal.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/pkcs11.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/pkcs12.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/pkcs5.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/platform.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/platform_time.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/platform_util.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/poly1305.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/psa_util.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ripemd160.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/rsa.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/rsa_internal.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/sha1.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/sha256.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/sha512.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ssl.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ssl_cache.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ssl_ciphersuites.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ssl_cookie.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ssl_internal.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/ssl_ticket.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/threading.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/timing.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/version.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/x509.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/x509_crl.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/x509_crt.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/x509_csr.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/mbedtls/xtea.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/psa" TYPE FILE MESSAGE_NEVER PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ FILES
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_builtin_composites.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_builtin_primitives.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_compat.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_config.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_driver_common.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_driver_contexts_composites.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_driver_contexts_primitives.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_extra.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_platform.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_se_driver.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_sizes.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_struct.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_types.h"
    "/root/pico/pico-sdk/lib/mbedtls/include/psa/crypto_values.h"
    )
endif()


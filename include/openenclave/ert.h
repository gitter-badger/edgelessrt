// Copyright (c) Edgeless Systems GmbH.
// Licensed under the MIT License.

#pragma once

#include <openenclave/enclave.h>
#include <openenclave/ert_args.h>

OE_EXTERNC_BEGIN

/**
 * The enclave entry point used by erthost.
 *
 * The enclave can implement this function instead of linking against one of the
 * ert*entry libraries.
 *
 * @return Exit code that erthost should return to the system.
 */
int emain(void);

/**
 * Get pointers to commandline arguments, environment variables, and auxiliary
 * vector from host.
 *
 * The caller is responsible to securely copy the arrays to enclave memory. This
 * can be done using ert_copy_strings_from_host_to_enclave().
 *
 * @param[out] retval Pointer to an ert_args_t object that will be filled with
 * pointers to host memory.
 *
 * @return OE_OK if the ocall succeeded
 */
oe_result_t ert_get_args_ocall(ert_args_t* retval);

/**
 * Securely deep-copy an array of strings from the host to the enclave.
 *
 * @param host_array An array in host memory.
 * @param[out] enclave_array An array that will be allocated on the enclave
 * heap. The array will include an additional terminating nullptr element. Free
 * with free().
 * @param count Number of elements to copy.
 */
void ert_copy_strings_from_host_to_enclave(
    const char* const* host_array,
    char*** enclave_array,
    size_t count);

OE_EXTERNC_END

#ifdef __cplusplus

#include <memory>
#include <string>

namespace ert
{
namespace memfs
{
class Filesystem;
}

/**
 * In-enclave-memory filesystem.
 *
 * Usage:
 *
 * const Memfs memfs("my_fs");
 * mount("/", "/my/mount/point", "my_fs", 0, nullptr);
 *
 * Folders are not supported for now. The app may open a file by passing a full
 * path and the fs behaves as if all folders in the path exist.
 */
class Memfs
{
  public:
    Memfs(const std::string& devname);
    ~Memfs();
    Memfs(const Memfs&) = delete;
    Memfs& operator=(const Memfs&) = delete;

  private:
    const std::unique_ptr<memfs::Filesystem> impl_;
    oe_customfs_t ops_;
    uint64_t devid_;

    static memfs::Filesystem& to_fs(void* context);
};
} // namespace ert

#endif // __cplusplus

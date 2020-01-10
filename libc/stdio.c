// Copyright (c) Edgeless Systems GmbH.
// Licensed under the MIT License.

#include <stdio.h>
#include <stdlib.h>

size_t __fread_chk(
    void* buffer,
    size_t buffersize,
    size_t size,
    size_t count,
    FILE* stream)
{
    if (count * size > buffersize)
        abort();
    return fread(buffer, size, count, stream);
}

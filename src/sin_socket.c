/*
 * Copyright (c) 2014 Sippy Software, Inc., http://www.sippysoft.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "include/libsinet.h"
#include "sin_types.h"
#include "libsinet_internal.h"
#include "sin_errno.h"
#include "sin_stance.h"

static inline struct sin_stance *
_sip_incref(struct sin_stance *sip)
{

    SIN_INCREF(sip);
    if (sip->sin_nref == 1) {
        SIN_DECREF(sip);
        return (NULL);
    }
    return (sip);
}

void *
sin_socket(void *sin_stance, int domain, int type, int protocol, int *e)
{
    struct sin_socket *ssp;
    struct sin_stance *sip;

    sip = (struct sin_stance *)sin_stance;
    sip = _sip_incref(sip);
    if (sip == NULL) {
        _SET_ERR(e, EINVAL);
        return (NULL);
    }

    if (domain != PF_INET || type != SOCK_DGRAM || protocol != 0) {
        _SET_ERR(e, EPROTONOSUPPORT);
        SIN_DECREF(sip);
        return (NULL);
    }

    ssp = malloc(sizeof(struct sin_socket));
    if (ssp == NULL) {
        _SET_ERR(e, ENOMEM);
        SIN_DECREF(sip);
        return (NULL);
    }
    memset(ssp, '\0', sizeof(struct sin_socket));
    SIN_TYPE_SET(ssp, _SIN_TYPE_SOCKET);
    ssp->sip = sip;

    SIN_INCREF(ssp);
    return ((void *)ssp);
}

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

#ifndef _SIN_WI_QUEUE_H_
#define _SIN_WI_QUEUE_H_

struct sin_wi_queue;
struct sin_list;

struct sin_wi_queue *sin_wi_queue_ctor(int *sin_err,
  const char *format, ...);
void sin_wi_queue_dtor(struct sin_wi_queue *queue);

int sin_wi_queue_put_item(void *wi, struct sin_wi_queue *, unsigned int);
void sin_wi_queue_put_items(struct sin_list *lst, struct sin_wi_queue *);
int sin_wi_queue_pump(struct sin_wi_queue *);
void *sin_wi_queue_get_item(struct sin_wi_queue *queue, int waitok,
  int return_on_wake);
unsigned int sin_wi_queue_get_items(struct sin_wi_queue *queue,
  struct sin_list *lst, int waitok, int return_on_wake);

#if 0
void rtpp_queue_pump(struct rtpp_queue *);

struct rtpp_wi *rtpp_queue_get_item(struct rtpp_queue *queue, int return_on_wake);
int rtpp_queue_get_items(struct rtpp_queue *, struct rtpp_wi **, int, int);
int rtpp_queue_get_length(struct rtpp_queue *);
#endif

#endif

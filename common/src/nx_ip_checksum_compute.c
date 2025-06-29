/***************************************************************************
 * Copyright (c) 2024 Microsoft Corporation 
 * 
 * This program and the accompanying materials are made available under the
 * terms of the MIT License which is available at
 * https://opensource.org/licenses/MIT.
 * 
 * SPDX-License-Identifier: MIT
 **************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Component                                                        */
/**                                                                       */
/**   Internet Protocol Checksum Computation                              */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#define NX_SOURCE_CODE


/* Include necessary system files.  */

#include "nx_api.h"
#include "nx_icmp.h"
#include "nx_icmpv6.h"
#include "nx_ip.h"


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _nx_ip_checksum_compute                           PORTABLE C        */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Yuxin Zhou, Microsoft Corporation                                   */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function computes the checksum from the supplied packet        */
/*    pointer and IP address fields required for the pseudo header.       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    packet_ptr                            Pointer to packet             */
/*    protocol                              Protocol type                 */
/*    data_length                           Size of the protocol payload  */
/*    src_ip_addr                           IPv4 or IPv6 address, used in */
/*                                             constructing pseudo header.*/
/*    dest_ip_addr                          IPv4 or IPv6 address, used in */
/*                                             constructing pseudo header.*/
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    computed checksum                                                   */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    NetX Duo internal routines                                          */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Yuxin Zhou               Initial Version 6.0           */
/*  09-30-2020     Yuxin Zhou               Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
USHORT  _nx_ip_checksum_compute(NX_PACKET *packet_ptr, ULONG protocol,
                                UINT data_length, ULONG *src_ip_addr,
                                ULONG *dest_ip_addr)
{

ULONG      checksum = 0;
USHORT     tmp;
USHORT    *short_ptr;
#ifndef NX_DISABLE_PACKET_CHAIN
ULONG      packet_size;
#endif /* NX_DISABLE_PACKET_CHAIN */
NX_PACKET *current_packet;
USHORT     end_ptr;
#ifdef FEATURE_NX_IPV6
UINT       i;
#endif

    /* For computing TCP/UDP/ICMPv6, we need to include the pseudo header.
       The ICMPv4 checksum does not cover the pseudo header. */
    if ((protocol == NX_PROTOCOL_UDP) ||
#ifdef FEATURE_NX_IPV6
        (protocol == NX_PROTOCOL_ICMPV6) ||
#endif /* FEATURE_NX_IPV6 */
        (protocol == NX_PROTOCOL_TCP))
    {

    USHORT *src_ip_short, *dest_ip_short;

        checksum = protocol;

        /* The addresses must not be null.  */
        NX_ASSERT((src_ip_addr != NX_NULL) && (dest_ip_addr != NX_NULL));

        /*lint -e{929} -e{740} suppress cast of pointer to pointer, since it is necessary  */
        src_ip_short = (USHORT *)src_ip_addr;

        /*lint -e{929} -e{740} suppress cast of pointer to pointer, since it is necessary  */
        dest_ip_short = (USHORT *)dest_ip_addr;


        checksum += src_ip_short[0];
        checksum += src_ip_short[1];
        checksum += dest_ip_short[0];
        checksum += dest_ip_short[1];

#ifdef FEATURE_NX_IPV6

        /* Note that the IPv6 address is 128 bits/4 words
           compared with the 32 IPv4 address.*/
        if (packet_ptr -> nx_packet_ip_version == NX_IP_VERSION_V6)
        {

            for (i = 2; i < 8; i++)
            {

                checksum += dest_ip_short[i];
                checksum += src_ip_short[i];
            }
        }
#endif /* FEATURE_NX_IPV6 */

        /* Take care of data length */
        checksum += data_length;

        /* Fold a 4-byte value into a two byte value */
        checksum = (checksum >> 16) + (checksum & 0xFFFF);

        /* Do it again in case previous operation generates an overflow */
        checksum = (checksum >> 16) + (checksum & 0xFFFF);

        /* Convert to network byte order. */
        tmp = (USHORT)checksum;
        NX_CHANGE_USHORT_ENDIAN(tmp);
        checksum = tmp;
    }

    /* Now we need to go through the payloads */

    /* Setup the pointer to the start of the packet.  */
    /*lint -e{927} -e{826} suppress cast of pointer to pointer, since it is necessary  */
    short_ptr =  (USHORT *)packet_ptr -> nx_packet_prepend_ptr;

    /* Initialize the current packet to the input packet pointer.  */
    current_packet =  packet_ptr;

#ifndef NX_DISABLE_PACKET_CHAIN
    /* Loop the packet. */
    while (current_packet)
    {

        /* Calculate current packet size. */
        /*lint -e{946} -e{947} suppress pointer subtraction, since it is necessary. */
        packet_size = (ULONG)(current_packet -> nx_packet_append_ptr - current_packet -> nx_packet_prepend_ptr);

        /* Calculate the end address in this packet. */
        if (data_length > (UINT)packet_size)
        {

            /*lint -e{927} -e{923} -e{826} suppress cast of pointer to pointer, since it is necessary  */
            end_ptr = (USHORT)((ULONG)(current_packet -> nx_packet_append_ptr)) & (~1UL);
        }
        else
        {
#endif /* NX_DISABLE_PACKET_CHAIN */
            /*lint -e{927} -e{826} suppress cast of pointer to pointer, since it is necessary  */
            end_ptr = (USHORT)((ULONG)(current_packet -> nx_packet_prepend_ptr) + (ULONG)(data_length) - 1UL);
#ifndef NX_DISABLE_PACKET_CHAIN
        }
#endif /* NX_DISABLE_PACKET_CHAIN */

        /* Set the start address in this packet. */
        /*lint -e{927} -e{826} suppress cast of pointer to pointer, since it is necessary  */
        short_ptr = (USHORT *)current_packet -> nx_packet_prepend_ptr;

        /*lint -e{946} suppress pointer subtraction, since it is necessary. */
        if ((USHORT)((ULONG)short_ptr) < end_ptr)
        {
            /* Loop to calculate the packet's checksum.  */
            /*lint -e{946} suppress pointer subtraction, since it is necessary. */
            while ((USHORT)((ULONG)short_ptr) < end_ptr)
            {
                checksum += (*short_ptr);
                short_ptr++;
                data_length -= 2;
            }
        }
#ifndef NX_DISABLE_PACKET_CHAIN

        /* Determine if we are at the end of the current packet.  */
        if ((data_length > 0) && (current_packet -> nx_packet_next))
        {
            /* We have crossed the packet boundary.  Move to the next packet
               structure.  */
            current_packet =  current_packet -> nx_packet_next;
        }
        else
        {

            /* End the loop.  */
            current_packet = NX_NULL;
        }
    }
#endif /* NX_DISABLE_PACKET_CHAIN */

    /* Determine if there is only one byte left. */
    if (data_length)
    {
        /* Check the data length.  */
        if (data_length == 1)
        {
            *((UCHAR *)short_ptr + 1) = 0;
        }

        checksum += *short_ptr;
    }

    /* Fold a 4-byte value into a two byte value */
    checksum = (checksum >> 16) + (checksum & 0xFFFF);

    /* Do it again in case previous operation generates an overflow */
    checksum = (checksum >> 16) + (checksum & 0xFFFF);

    /* Convert to host byte order. */
    tmp = (USHORT)checksum;
    NX_CHANGE_USHORT_ENDIAN(tmp);

    /* Return the computed checksum.  */
    return(tmp);
}



/* This NetX test concentrates on the UCP operation.  */

#include   "tx_api.h"
#include   "nx_api.h"    
#include   "nx_tcp.h"
#include   "nx_udp.h" 
                         
extern void    test_control_return(UINT status);
#if defined NX_NAT_ENABLE && defined __PRODUCT_NETXDUO__ && (NX_MAX_PHYSICAL_INTERFACES >= 2) && !defined (NX_DISABLE_FRAGMENTATION) && !defined(NX_DISABLE_IPV4)
#include   "nx_nat.h"

#define     DEMO_STACK_SIZE         2048
                                                 
/* Define the ThreadX and NetX object control blocks...  */

static TX_THREAD                    thread_0;  
static TX_THREAD                    thread_1;

/* Set up the NAT components. */

/* Create a NAT instance, packet pool and translation table. */
                                                 
NX_PACKET_POOL                      nat_packet_pool;   
NX_NAT_DEVICE                       nat_server;  
NX_IP                               nat_ip;      
NX_IP                               local_ip;
NX_IP                               external_ip;   
NX_UDP_SOCKET                       local_socket;
NX_UDP_SOCKET                       external_socket;

static UCHAR                        message[1024];
static UCHAR                        buffer[1024];

/* Configure the NAT network parameters. */

/* Set NetX IP packet pool packet size. This should be less than the Maximum Transmit Unit (MTU) of
   the driver (allow enough room for the Ethernet header plus padding bytes for frame alignment).  */
#define NX_NAT_PACKET_SIZE                          1536


/* Set the size of the NAT IP packet pool.  */
#define NX_NAT_PACKET_POOL_SIZE                     (NX_NAT_PACKET_SIZE * 10)

/* Set NetX IP helper thread stack size. */   
#define NX_NAT_IP_THREAD_STACK_SIZE                 2048

/* Set the server IP thread priority */
#define NX_NAT_IP_THREAD_PRIORITY                   2

/* Set ARP cache size of a NAT ip instance. */
#define NX_NAT_ARP_CACHE_SIZE                       1024 

/* Set NAT entries memory size. */
#define NX_NAT_ENTRY_CACHE_SIZE                     1024

/* Define NAT IP addresses, local host private IP addresses and external host IP address. */
#define NX_NAT_LOCAL_IPADR              (IP_ADDRESS(192, 168, 2, 1))  
#define NX_NAT_LOCAL_HOST1              (IP_ADDRESS(192, 168, 2, 3))
#define NX_NAT_LOCAL_HOST2              (IP_ADDRESS(192, 168, 2, 10)) 
#define NX_NAT_LOCAL_GATEWAY            (IP_ADDRESS(192, 168, 2, 1))    
#define NX_NAT_LOCAL_NETMASK            (IP_ADDRESS(255, 255, 255, 0))
#define NX_NAT_EXTERNAL_IPADR           (IP_ADDRESS(192, 168, 0, 10))   
#define NX_NAT_EXTERNAL_HOST            (IP_ADDRESS(192, 168, 0, 100))   
#define NX_NAT_EXTERNAL_GATEWAY         (IP_ADDRESS(192, 168, 0, 1))    
#define NX_NAT_EXTERNAL_NETMASK         (IP_ADDRESS(255, 255, 255, 0))  

/* Create NAT structures for preloading NAT tables with static 
   entries for local server hosts. */
NX_NAT_TRANSLATION_ENTRY            server_inbound_entry_udp;

/* Set up generic network driver for demo program. */             
extern void    _nx_ram_network_driver_256(struct NX_IP_DRIVER_STRUCT *driver_req); 
extern void    _nx_ram_network_driver_512(struct NX_IP_DRIVER_STRUCT *driver_req);


/* Define thread prototypes.  */

static void    thread_0_entry(ULONG thread_input); 
static void    thread_1_entry(ULONG thread_input);
                                                                        

/* Define what the initial system looks like.  */
#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_nat_udp_fragment_test_application_define(void *first_unused_memory)
#endif
{

UINT     status;
UCHAR    *pointer;    
UINT     error_counter = 0;
    
    /* Initialize the NetX system. */
    nx_system_initialize();
    
    /* Setup the pointer to unallocated memory.  */
    pointer =  (UCHAR *) first_unused_memory;
                          
    /* Create the main thread.  */
    tx_thread_create(&thread_0, "thread 0", thread_0_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;        

    /* Create the main thread.  */
    tx_thread_create(&thread_1, "thread 1", thread_1_entry, 0,  
                     pointer, DEMO_STACK_SIZE, 
                     4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);
    pointer =  pointer + DEMO_STACK_SIZE;   

    /* Create NAT packet pool. */   
    status =  nx_packet_pool_create(&nat_packet_pool, "NAT Packet Pool", 
                                    NX_NAT_PACKET_SIZE, pointer, 
                                    NX_NAT_PACKET_POOL_SIZE);

    /* Update pointer to unallocated (free) memory. */
    pointer = pointer + NX_NAT_PACKET_POOL_SIZE;    

    /* Check status.  */
    if (status)
        return;
                            
    /* Create IP instances for NAT server (global network) */
    status = nx_ip_create(&nat_ip, "NAT IP Instance", NX_NAT_EXTERNAL_IPADR, NX_NAT_EXTERNAL_NETMASK, 
                          &nat_packet_pool, _nx_ram_network_driver_256, pointer, 
                          NX_NAT_IP_THREAD_STACK_SIZE, NX_NAT_IP_THREAD_PRIORITY);

    /* Update pointer to unallocated (free) memory. */
    pointer =  pointer + NX_NAT_IP_THREAD_STACK_SIZE;

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }
                 
    /* Set the private interface(private network).  */
    status += nx_ip_interface_attach(&nat_ip, "Private Interface", NX_NAT_LOCAL_IPADR, NX_NAT_LOCAL_NETMASK, _nx_ram_network_driver_512);
             
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }                
                                     
    /* Create IP instances for Local network IP instance */
    status = nx_ip_create(&local_ip, "Local IP Instance", NX_NAT_LOCAL_HOST1, NX_NAT_LOCAL_NETMASK, 
                          &nat_packet_pool, _nx_ram_network_driver_256, pointer, 
                          NX_NAT_IP_THREAD_STACK_SIZE, NX_NAT_IP_THREAD_PRIORITY);

    /* Update pointer to unallocated (free) memory. */
    pointer =  pointer + NX_NAT_IP_THREAD_STACK_SIZE;

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }
                  
    /* Create IP instances for external network IP instance */
    status = nx_ip_create(&external_ip, "External IP Instance", NX_NAT_EXTERNAL_HOST, NX_NAT_EXTERNAL_NETMASK, 
                          &nat_packet_pool, _nx_ram_network_driver_512, pointer, 
                          NX_NAT_IP_THREAD_STACK_SIZE, NX_NAT_IP_THREAD_PRIORITY);

    /* Update pointer to unallocated (free) memory. */
    pointer =  pointer + NX_NAT_IP_THREAD_STACK_SIZE;

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Set the global network gateway for NAT IP instance.  */
    status = nx_ip_gateway_address_set(&nat_ip, NX_NAT_EXTERNAL_GATEWAY);
                       
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }                     
    
    /* Set the global network gateway for Local IP instance.  */
    status = nx_ip_gateway_address_set(&local_ip, NX_NAT_LOCAL_GATEWAY);
                       
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }                     
    
    /* Set the global network gateway for External IP instance.  */
    status = nx_ip_gateway_address_set(&external_ip, NX_NAT_EXTERNAL_GATEWAY);
                       
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }                     

    
    /* Enable ARP and supply ARP cache memory for NAT IP isntance. */
    status =  nx_arp_enable(&nat_ip, (void **) pointer, 
                            NX_NAT_ARP_CACHE_SIZE);
                         
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }           
    
    /* Update pointer to unallocated (free) memory. */
    pointer = pointer + NX_NAT_ARP_CACHE_SIZE;
                                              
    /* Enable ARP and supply ARP cache memory for Local IP isntance. */
    status =  nx_arp_enable(&local_ip, (void **) pointer, 
                            NX_NAT_ARP_CACHE_SIZE);
                         
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }           
    
    /* Update pointer to unallocated (free) memory. */
    pointer = pointer + NX_NAT_ARP_CACHE_SIZE;
                                             
    /* Enable ARP and supply ARP cache memory for External IP isntance. */
    status =  nx_arp_enable(&external_ip, (void **) pointer, 
                            NX_NAT_ARP_CACHE_SIZE);
                         
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }           
    
    /* Update pointer to unallocated (free) memory. */
    pointer = pointer + NX_NAT_ARP_CACHE_SIZE;
                                     
    /* Enable UDP traffic.  */
    status =  nx_udp_enable(&nat_ip);
    status += nx_udp_enable(&local_ip);
    status += nx_udp_enable(&external_ip);
                                           
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Enable the fragment function.  */
    status = nx_ip_fragment_enable(&nat_ip);       
    status += nx_ip_fragment_enable(&local_ip);
    status += nx_ip_fragment_enable(&external_ip);  
                                           
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Create a NetX NAT server and cache with a global interface index.  */
    status =  nx_nat_create(&nat_server, &nat_ip, 0, pointer, NX_NAT_ENTRY_CACHE_SIZE);
                             
    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Update pointer to unallocated (free) memory. */
    pointer = pointer + NX_NAT_ENTRY_CACHE_SIZE;

    /* Enable the NAT service.  */
    nx_nat_enable(&nat_server);
}

/* Define the test threads.  */

static void    thread_0_entry(ULONG thread_input)
{

UINT        status;
UINT        i;
NX_PACKET   *my_packet;


    /* Print out test information banner.  */
    printf("NetX Test:   NAT UDP Fragment Processing Test..........................");
                                      
    /* Create a UDP local socket.  */
    status = nx_udp_socket_create(&local_ip, &local_socket, "Local Socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

    /* Check status.  */
    if (status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
                                 
    /* Bind the UDP socket to the IP port 0x88.  */
    status =  nx_udp_socket_bind(&local_socket, 0x88, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }        

    /* Create a UDP External socket.  */
    status = nx_udp_socket_create(&external_ip, &external_socket, "External Socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

    /* Check status.  */
    if (status)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Bind the UDP socket to the IP port 0x89.  */
    status =  nx_udp_socket_bind(&external_socket, 0x89, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Clear the message.  */
    for (i = 0; i < 1024; i ++)
        message[i] = (UCHAR)rand();

    /***********************************************************************/
    /*         Local Socket sends udp packet to External Socket            */    
    /***********************************************************************/

    /* Let other threads run again.  */
    tx_thread_relinquish();

    /* Allocate a packet.  */
    status =  nx_packet_allocate(&nat_packet_pool, &my_packet, NX_UDP_PACKET, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {        
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Write ABCs into the packet payload!  */
    memcpy(my_packet -> nx_packet_prepend_ptr, &message[0], 400);

    /* Adjust the write pointer.  */
    my_packet -> nx_packet_length = 400;
    my_packet -> nx_packet_append_ptr =  my_packet -> nx_packet_prepend_ptr + 400;

    /* Send the UDP packet.  */
    status =  nx_udp_socket_send(&local_socket, my_packet, NX_NAT_EXTERNAL_HOST, 0x89);

    /* Check status.  */
    if (status)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Calling NAT API to preload a static entry.  */
    status = nx_nat_inbound_entry_create(&nat_server, &server_inbound_entry_udp, NX_NAT_LOCAL_HOST1, 0x88, 0x88, NX_PROTOCOL_UDP);    

    if (status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /***********************************************************************/
    /*         External Socket sends udp packet to Local Socket            */    
    /***********************************************************************/
                                   
    /* Let other threads run again.  */
    tx_thread_relinquish();
    
    /* Allocate a packet.  */
    status =  nx_packet_allocate(&nat_packet_pool, &my_packet, NX_UDP_PACKET, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Write ABCs into the packet payload!  */
    memcpy(my_packet -> nx_packet_prepend_ptr, &message[0], 800);

    /* Adjust the write pointer.  */
    my_packet -> nx_packet_length =  800;
    my_packet -> nx_packet_append_ptr =  my_packet -> nx_packet_prepend_ptr + 800;

    /* Send the UDP packet.  */
    status =  nx_udp_socket_send(&external_socket, my_packet, NX_NAT_EXTERNAL_IPADR, 0x88);

    /* Check status.  */
    if (status)
    {

        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Let other threads run again.  */
    tx_thread_sleep(NX_IP_PERIODIC_RATE);

    /* Output success.  */
    printf("SUCCESS!\n");
    test_control_return(0);
}


static void    thread_1_entry(ULONG thread_input)
{

UINT        status;
ULONG       bytes_copied;
NX_PACKET   *my_packet;


    /***********************************************************************/
    /*         External Socket receives the udp packet from Local Socket   */    
    /***********************************************************************/

    /* Receive a UDP packet.  */
    status =  nx_udp_socket_receive(&external_socket, &my_packet, NX_IP_PERIODIC_RATE);

    /* Check status.  */
    if ((status != NX_SUCCESS) || (my_packet -> nx_packet_length != 400))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Retrieve the data.  */
    status = nx_packet_data_retrieve(my_packet, buffer, &bytes_copied);

    /* Check status.  */
    if ((status != NX_SUCCESS) || (bytes_copied != 400))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Compare the data.  */
    if (memcmp(buffer, &message[0], 400))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Release the packet.  */
    status =  nx_packet_release(my_packet);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /***********************************************************************/
    /*         Local Socket receives the udp packet from External Socket   */    
    /***********************************************************************/

    /* Receive a UDP packet.  */
    status =  nx_udp_socket_receive(&local_socket, &my_packet, NX_IP_PERIODIC_RATE);

    /* Check status.  */
    if ((status != NX_SUCCESS) || (my_packet ->nx_packet_length != 800))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Retrieve the data.  */
    status = nx_packet_data_retrieve(my_packet, buffer, &bytes_copied);

    /* Check status.  */
    if ((status != NX_SUCCESS) || (bytes_copied != 800))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Compare the data.  */
    if (memcmp(buffer, &message[0], 800))
    {
        printf("ERROR!\n");
        test_control_return(1);
    }

    /* Release the packet.  */
    status =  nx_packet_release(my_packet);

    /* Check status.  */
    if (status != NX_SUCCESS)
    {
        printf("ERROR!\n");
        test_control_return(1);
    }
}

#else

extern void    test_control_return(UINT status);

#ifdef CTEST
VOID test_application_define(void *first_unused_memory)
#else
void    netx_nat_udp_fragment_test_application_define(void *first_unused_memory)
#endif
{
    printf("NetX Test:   NAT UDP Fragment Processing Test..........................N/A\n");
    test_control_return(3);
}
#endif

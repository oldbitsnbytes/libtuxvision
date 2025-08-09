/***************************************************************************
 *   Copyright (C) 1965/1987/2023 by Serge luxsier                         *
 *   serge.luxsier@oldbitsnbytes.club                                       *
 *                                                                         *
 *                                                                         *
 *   Unless otherwise specified, all code in this project is written       *
 *   by the author (Serge luxsier)                                         *
 *   and no one else then not even {copilot, chatgpt, or any other AI}     *
 *   --------------------------------------------------------------------- *
 *   Copyrights from authors other than Serge luxsier also apply here      *
 ***************************************************************************/


#include <tuxvision/utility/io/tcp_socket.h>
#include <tuxvision/utility/string.h>


constexpr const char* IpV4Fmt="%d.%d.%d.%d";

namespace tux::io {

/**
 * @brief Constructs a tcp_socket object and initializes it with the given parent and identifier.
 *
 * This constructor initializes the tcp_socket object with a parent object and a string
 * identifier. It also sets the internal `m_ifd` pointer to nullptr, indicating it is not
 * associated with a file descriptor object at this moment.
 *
 * @param parent A pointer to the parent object of this tcp_socket.
 * @param ii A string identifier for the tcp_socket instance.
 */
tcp_socket::tcp_socket(object* parent, const std::string& ii): object(parent,ii)
{
    m_ifd = nullptr;
}

tcp_socket::tcp_socket()
{
    m_ifd = nullptr;
}

tcp_socket::~tcp_socket()
{
}


/**
 * @brief Creates a TCP socket and initializes its corresponding file descriptor and internal structures.
 *
 * This method creates a new TCP socket using the `::socket` system call with PF_INET and SOCK_STREAM.
 * It assigns the resulting socket file descriptor to the member variable `m_fd` and associates it with
 * an `lfd` object with specific configurations.
 *
 * @return The file descriptor of the newly created socket. Returns -1 if the socket creation fails.
 */
int tcp_socket::create()
{
    m_fd = ::socket(PF_INET,SOCK_STREAM, 0);
    //u_int32_t options = ifd::O_BLOCK|ifd::O_BUF|ifd::O_READ|ifd::O_WRITE|ifd::O_WINDOWED;
    tux::string str;
    str << id() << ':' << m_fd;

    m_ifd = new lfd(str(), m_fd, 4096, lfd::IMM|lfd::READ|lfd::WRITE, POLLIN|POLLOUT|POLLHUP|POLLERR, nullptr);
    return m_fd;
}


/**
 * @brief Sets the file descriptor for the TCP socket and initializes its internal structures.
 *
 * This method assigns the provided file descriptor to the member variable `m_fd`, constructs a string
 * identifier for the socket based on its `id()` and file descriptor, and creates a new `lfd` object
 * with specified configurations, including buffer size, flags, and epoll event settings.
 *
 * @param fd The file descriptor to be assigned to the TCP socket.
 */
void tcp_socket::set_sockfd(int fd)
{
    m_fd = fd;
    tux::string str;
    str << id() << ':' << m_fd;

    m_ifd = new lfd(str(), m_fd, 4096, lfd::IMM|lfd::READ|lfd::WRITE, POLLIN|POLLOUT|POLLHUP|POLLERR, nullptr);
}


/**
 * @brief Resolves a hostname to its corresponding IP address and optionally fills a sockaddr_in structure.
 *
 * This method retrieves the host information for a given hostname, extracts its IPv4 address, and formats
 * it into a string. Optionally, it fills a sockaddr_in structure with the resolved IP and port information.
 *
 * @param node The hostname to resolve (e.g., "example.com").
 * @param port The port number to include in the sockaddr_in structure (if provided).
 * @param _addr_in A pointer to a sockaddr_in structure to be filled with the resolved IP and port
 *                 information. Pass nullptr if no sockaddr_in structure is needed.
 * @param NodeIP A reference to a string to store the resolved IP address in textual dotted form (e.g., "192.168.1.1").
 *
 * @return A pointer to a hostent structure containing information about the host. Returns nullptr if
 *         the hostname resolution fails.
 */
hostent* tcp_socket::host(const char* node, uint port, sockaddr_in* _addr_in, std::string& NodeIP)
{
    hostent* he = gethostbyname(node);

    int a,b,c,d;

    if (!he ) {
        //sys::error() << "gethostbyname failed!!"  << //sys::endl;
        ///@todo traiter l'erreur du DNS
        return nullptr;
    }

    // Pas de IPV6... c'est pas demain que cette version va arriver.... bon gre - malgre ...
    a = (int)((unsigned char)he->h_addr_list[0][0]);
    b = (int)((unsigned char)he->h_addr_list[0][1]);
    c = (int)((unsigned char)he->h_addr_list[0][2]);
    d = (int)((unsigned char)he->h_addr_list[0][3]);


    NodeIP = std::format(IpV4Fmt, a,b,c,d);

    if (_addr_in) {
        memset(_addr_in, 0, sizeof(sockaddr_in));
        _addr_in->sin_family = AF_INET;
        _addr_in->sin_port   = htons(port);
        _addr_in->sin_addr.s_addr = inet_addr(NodeIP.c_str());
    }

    return he;
}


/**
 * @brief Retrieves the hostname of the local machine.
 *
 * This method uses the `gethostname` system call to fetch the hostname of the local machine.
 * If the call is successful, it returns a dynamically allocated string containing the hostname.
 * If the call fails, it returns a null pointer.
 *
 * @return A dynamically allocated C-string containing the machine's hostname if successful.
 *         Returns nullptr if the hostname retrieval fails.
 *         The caller is responsible for freeing the allocated memory using `free`.
 */
char* tcp_socket::machine_hostname()
{
    char h [128];
    int R = gethostname(h,127);
    if(!R){
        return strdup(h);
    }
    return 0;
}


/**
 * @brief Creates and configures a socket address structure based on the provided address string and protocol.
 *
 * This method parses the given address string and protocol to populate the provided
 * socket address structure. It supports both hostname and IP address formats.
 * Additionally, it sets the appropriate port number based on the input.
 *
 * @param addr A pointer to the address structure (sockaddr) to be initialized.
 * @param addr_len A pointer to an integer specifying the size of the address structure.
 *                 This will be updated to the actual size of the populated structure.
 * @param addr_str A string containing the address and port information in the format "address:port".
 *                 Address can be a hostname, IP address, or "*".
 * @param proto A string specifying the protocol to use (e.g., "tcp" or "udp").
 *              If empty, the method defaults to "tcp".
 * @return 0 if the address structure is successfully created, -1 for parsing or address resolution errors,
 *         or -2 if the port number is invalid.
 */
int tcp_socket::mkaddr(void* addr, int* addr_len, const char* addr_str, const char* proto)
{
    tux::string saddress = addr_str;
    tux::string::word::list htokens{};

    auto n = saddress.words(htokens, false, ":");

    if (n!=2) return -1;
    std::string inaddress = **htokens.begin();
    std::string serviceport = *htokens.back();
    std::string protoc = proto;
    sockaddr_in* ap = (sockaddr_in*)addr;
    hostent* hp = 0;
    servent* sp = 0;
    char *cp;
    long lv;


    // Attribuer les valeurs par default

    protoc = protoc.empty() ? protoc : "tcp";


    // Initialiser la structure de l'adresse
    memset(ap,0, *addr_len);
    ap->sin_family  = AF_INET;
    ap->sin_port    = 0;
    ap->sin_addr.s_addr = INADDR_ANY;

    // integrer l'adresse hôte
    if ( inaddress != "*" ) {
        if (isdigit(inaddress[0])) {
            ap->sin_addr.s_addr = inet_addr(inaddress.c_str());
            if (ap->sin_addr.s_addr == INADDR_NONE) return -1;
        }
        else {
            // Assumer le host name
            if ( hp = gethostbyname(inaddress.c_str()); !hp ) return  -1;
            if (hp->h_addrtype != AF_INET) return -1;
            ap->sin_addr = *reinterpret_cast<in_addr *>((hp)->h_addr_list[0]);
        }
    }

    // Traiter le # du port

    if (serviceport != "*") {
        if ( isdigit(serviceport[0])) {
            lv=strtol(serviceport.c_str(),&cp,10);
            if (cp && *cp) return -1; // for now do nothing with cp. I  may use it later if detailed error is needed.
            if (lv<0 || lv>=65536) return -2;
            ap->sin_port = htons((short)lv);
        }
        else
        {
            std::cerr << "sp=" << serviceport << std::endl;
            sp = getservbyname(serviceport.c_str(), proto);
            if (sp) ap->sin_port = sp->s_port;
        }
    }

    *addr_len = sizeof(*ap);

    //sys::info() << "finalized socket addr infos [port]::" << ntohs(ap->sin_port) << //sys::endl;
    return 0;
}

} // namespace tux::io

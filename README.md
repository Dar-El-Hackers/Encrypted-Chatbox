# IPv6 Round-based Chaterminal

With the adoption of IPv6 by most ISPs in the world, it is now possible to implement a true P2P communication. This chaterminal establish a direct TCP connection with any device assigned to a globally routable IPv6 address. This opens the possibilty of creating a fully decentralized chat applications that doesn't rely on any middleman server such as STUN server or RELAY server. This is still a very simple round based chat where only 2 devices can chat together.  

##  Run the code
- cc 1.1-Renelo_chaterminalv6.c -o Renelo
- ./Renelo [IPv6 ADDRESS] ([SOURCE PORT] [DESTINATION PORT] OPTIONAL)

[IPv6 address] is the address of the device you would like to establish a connection with.

[SOURCE PORT] [DESTINATION PORT] are optional inputs.

  
##  Future improvements
- Encrypt messages.
- Allow multi-user chat.
